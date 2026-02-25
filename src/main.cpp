#include <iostream>
#include "realsense_capture.h"
#include "depth_converter.h"
#include "renderer.h"
#include "simple_pass.h"
#include "feedback_pass.h"

int main() {
    const std::string SERIAL_D435I = "841612071255";
    //const std::string SERIAL_D415 = "844612060543";

  try{
        //Start and configure realsense camera
        realsense_capture camera = realsense_capture(SERIAL_D435I);
        camera.start();

        //Render
        int w_width = 1280;
        int w_height = 960;
        renderer r(w_width, w_height, "GPU proccessing");

        //feedback shader pass type
        auto passType = std::make_unique<feedback_pass>("shaders/ripple/feedback_display_shader.glsl",
                                                      "shaders/ripple/simulation_shader.glsl",
                                                      w_width, w_height);
        r.set_pass(std::move(passType));

        //init render
        if(!r.init())
        {
            std::cerr << "Render init failed \n";
            return EXIT_FAILURE;
        }

        r.set_shader_params(0.1, 3.0);
        while (!r.should_close())
        {
            auto depth_data = camera.get_depth_data_float();
            //r.update_texture(depth_data);
            r.render(depth_data);
        }
        r.close();

    } catch (const std::exception& e)
    {
        std::cerr << "" << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}