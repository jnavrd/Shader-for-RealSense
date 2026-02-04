#include <iostream>
#include "realsense_capture.h"
#include "depth_converter.h"
#include "renderer.h"

int main() {
    const std::string SERIAL_D435I = "841612071255";
    //const std::string SERIAL_D415 = "844612060543";

  try{

        //Start and configure realsense camera
        realsense_capture camera = realsense_capture(SERIAL_D435I);
        camera.start();

        //Render
        renderer r(1280, 960, "GPU proccessing");
        if(!r.init())
        {
            std::cerr << "Render init failed \n";
            return EXIT_FAILURE;
        }

        r.set_shader_params(0.1, 1.0);
        while (!r.should_close())
        {
            auto depth_data = camera.get_depth_data_float();
            r.update_texture(depth_data);
            r.render();
        }
        r.close();

    } catch (const std::exception& e)
    {
        std::cerr << "" << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}