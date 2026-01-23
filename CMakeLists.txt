cmake_minimum_required(VERSION 3.26)
project(customShadersRS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# --- Raylib ---
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# --- RealSense ---
add_library(realsense2::realsense2 SHARED IMPORTED)
set_target_properties(realsense2::realsense2 PROPERTIES
        IMPORTED_LOCATION "D:/instalaciones/Intel RealSense SDK 2.0/bin/realsense2.dll"
        IMPORTED_IMPLIB  "D:/instalaciones/Intel RealSense SDK 2.0/lib/x64/realsense2.lib"
        INTERFACE_INCLUDE_DIRECTORIES "D:/instalaciones/Intel RealSense SDK 2.0/include"
)

# --- Módulos ---
add_library(capture STATIC
        src/capture/realsense_capture.cpp
)
target_include_directories(capture PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(capture PUBLIC realsense2::realsense2)

add_library(processing STATIC
        src/processing/depth_converter.cpp
)
target_include_directories(processing PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(rendering STATIC
        src/rendering/texture_renderer.cpp
)
target_include_directories(rendering PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(rendering PUBLIC raylib)

# --- Ejecutable ---
add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE capture processing rendering)