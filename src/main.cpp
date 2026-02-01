#include <iostream>
#include "realsense_capture.h"
#include "depth_converter.h"
#include "renderer.h"

int main() {
    const std::string SERIAL_D435I = "841612071255";
    const std::string SERIAL_D415 = "844612060543";

    //Start and configure realsense camera
    realsense_capture camera = realsense_capture(SERIAL_D435I);
    camera.start();

    //Convert depth frame data to grayscale
    depth_converter depthConverter = depth_converter();

    //Render
    renderer gpuRrenderer = renderer(1280, 960, "GPU proccessing");

    gpuRrenderer.init();
    while (!gpuRrenderer.should_close())
    {
        gpuRrenderer.update_texture(camera.get_depth_data_float());
        gpuRrenderer.render_with_quad();
    }
    gpuRrenderer.close();
/*
    renderer cpuRenderer = renderer(1280, 960, "CPU proccessing");
    cpuRenderer.init();
    while(!cpuRenderer.should_close())
    {
        GrayscaleImg cameraImg = depthConverter.depth_frame_to_grayscale(camera.get_depth_data());
        cpuRenderer.update_texture(cameraImg);
        cpuRenderer.render();
    }
    cpuRenderer.close();*/


    return 0;
}