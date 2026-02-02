#ifndef CUSTOMSHADERSRS_FRAME_TYPES_H
#define CUSTOMSHADERSRS_FRAME_TYPES_H


#include <vector>

//used in realsense_capture
//unused
struct DepthData {
    std::vector<uint16_t > frame;
    float depth_scale;
    int width;
    int height;
};

struct DepthDataFloat {
    std::vector<float> frame;
    int width;
    int height;
};

//used in depth_converter
//unused
struct GrayscaleImg {
    std::vector<uint8_t> data;
    int width;
    int height;
};

#endif //CUSTOMSHADERSRS_FRAME_TYPES_H
