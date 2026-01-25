#include <iostream>
#include "realsense_capture.h"
#include "depth_converter.h"
#include "renderer.h"

int main() {

    std::cout << "debug 1 " << std::endl;

    const std::string SERIAL_D435I = "841612071255";
    const std::string SERIAL_D415 = "844612060543";

    //Start and configure realsense camera
    realsense_capture camera = realsense_capture(SERIAL_D435I);
    camera.start();

    std::cout << "debug 2 " << std::endl;
    //Convert depth frame data to grayscale
    depth_converter grayscaleConverter = depth_converter();

    //Render
    renderer windowRenderer = renderer(1280, 960, "Window");

    std::cout << "debug 3 " << std::endl;

    windowRenderer.init();
    while(!windowRenderer.should_close())
    {
        std::cout << "debug 4 " << std::endl;
        GrayscaleImg cameraImg = grayscaleConverter.depth_frame_to_grayscale(camera.get_depth_frame());
        windowRenderer.update_texture(cameraImg);
        windowRenderer.render();
    }

    windowRenderer.close();

    return 0;
}