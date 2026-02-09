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

# --- RealSense (IMPORTED, manual) ---
set(REALSENSE_ROOT "D:/instalaciones/Intel RealSense SDK 2.0")

add_library(realsense2 SHARED IMPORTED)

set_target_properties(realsense2 PROPERTIES
        IMPORTED_LOCATION
        "${REALSENSE_ROOT}/bin/x64/realsense2.dll"
        IMPORTED_IMPLIB
        "${REALSENSE_ROOT}/lib/x64/realsense2.lib"
        INTERFACE_INCLUDE_DIRECTORIES
        "${REALSENSE_ROOT}/include"
)

# --- Módulo Capture ---
add_library(capture STATIC
        src/capture/realsense_capture.cpp
        include/realsense_capture.h
        include/frame_types.h
)
target_include_directories(capture PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(capture PUBLIC realsense2)

# --- Módulo Processing ---
add_library(processing STATIC
        src/processing/depth_converter.cpp
        include/depth_converter.h
        include/frame_types.h
)
target_include_directories(processing PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(processing PUBLIC realsense2)

# --- Módulo Rendering ---
add_library(rendering STATIC
        src/rendering/renderer.cpp
        include/renderer.h
        include/frame_types.h
)
target_include_directories(rendering PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(rendering PUBLIC raylib)

# --- Shaders ---
set(SHADER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders)
set(SHADER_DST_DIR $<TARGET_FILE_DIR:customShadersRS>/shaders)

add_custom_target(copy_shaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SHADER_SRC_DIR}
        ${SHADER_DST_DIR}
        COMMENT "Copying shaders to runtime directory"
)

# --- Ejecutable ---
add_executable(customShadersRS
        src/main.cpp
)

add_custom_command(TARGET customShadersRS POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${REALSENSE_ROOT}/bin/x64/realsense2.dll"
        $<TARGET_FILE_DIR:customShadersRS>
)

target_include_directories(customShadersRS PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(customShadersRS PRIVATE
        capture
        processing
        rendering
        realsense2
)

add_dependencies(customShadersRS copy_shaders)