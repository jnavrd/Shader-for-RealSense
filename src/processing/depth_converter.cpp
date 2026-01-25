
#include "depth_converter.h"

depth_converter::depth_converter() {}

GrayscaleImg depth_converter::depth_frame_to_grayscale(DepthData depth) {

    GrayscaleImg image;
    image.width = depth.width;
    image.height = depth.height;
    image.data.resize(image.width*image.height);

    for(int i = 0; i < image.height; i++) {

        for(int j = 0; j < image.width; j++) {

            int index = i * image.width + j;
            float depth_value = depth.frame[index] * depth.depth_scale;
            image.data[index] = normalize_depth(depth_value); //agregar al vector el valor del pixel
        }
    }

    return image;
}

uint8_t depth_converter::normalize_depth(float depth_value) {

    if (depth_value <= MIN_RANGE)
        return 0;
    if (depth_value >= MAX_RANGE)
        return 255;

    float normalized =
            (depth_value - MIN_RANGE) / (MAX_RANGE - MIN_RANGE); //normalizar a 0-1

    return static_cast<uint8_t>(std::clamp(normalized, 0.0f, 1.0f) * 255.0f);

}

depth_converter::~depth_converter() = default;
