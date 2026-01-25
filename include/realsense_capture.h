#ifndef CUSTOMSHADERSRS_REALSENSE_CAPTURE_H
#define CUSTOMSHADERSRS_REALSENSE_CAPTURE_H

#include <librealsense2/rs.hpp>
#include "frame_types.h"

/*struct DepthData {

    rs2::depth_frame frame;
    float depth_scale;

    const uint16_t* get_buffer() const {
        return (const uint16_t*)frame.get_data();
    }

    int get_width() const {
        return frame.get_width();
    }

    int get_height() const {
        return frame.get_height();
    }
};*/

class realsense_capture {
    static constexpr int STREAM_WIDTH = 640;
    static constexpr int STREAM_HEIGHT = 480;
    static constexpr int STREAM_FPS = 30;

public:
    explicit realsense_capture(const std::string& serial_number);

    virtual ~realsense_capture();

    void start();

    DepthData get_depth_frame();

private:
    rs2::pipeline pipeline_;
    rs2::pipeline_profile profile_;
    float depth_scale_;
    std::string serial_number_;

    rs2::config configure_pipeline();
};


#endif //CUSTOMSHADERSRS_REALSENSE_CAPTURE_H
