#ifndef CUSTOMSHADERSRS_DEPTH_CONVERTER_H
#define CUSTOMSHADERSRS_DEPTH_CONVERTER_H

#include <vector>
#include <algorithm>
#include "frame_types.h"

class depth_converter {
    static constexpr float MIN_RANGE = 0.1f; //100 mm
    static constexpr float MAX_RANGE = 0.6f; //600 mm

public:
    depth_converter();
    virtual ~depth_converter();

    static GrayscaleImg depth_frame_to_grayscale(DepthData depth);

private:

    static uint8_t normalize_depth(float depth_value);

};

#endif //CUSTOMSHADERSRS_DEPTH_CONVERTER_H
