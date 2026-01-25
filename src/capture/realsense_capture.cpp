//
// Created by juana on 23/1/2026.
//

#include <iostream>
#include "realsense_capture.h"

realsense_capture::realsense_capture(const std::string &serial_number) : serial_number_(serial_number), depth_scale_(0.0f) {
}

realsense_capture::~realsense_capture() = default;

void realsense_capture::start() { //initalize camera pipeline
    try{
        profile_ = pipeline_.start(configure_pipeline());

        auto sensor = profile_.get_device().first<rs2::depth_sensor>();
        depth_scale_ = sensor.get_depth_scale();
    }   catch (const rs2::error& e) {
        std::cerr << "RealSense Error while configuring Pipeline: " << e.what() << std::endl;
    }
    }

DepthData realsense_capture::get_depth_frame() {

    rs2::frameset frameset = pipeline_.wait_for_frames();
    rs2::depth_frame depthFrame = frameset.get_depth_frame();

    if (!depthFrame){
        std::cerr << "No depth frame";
        return {};
    }

    int width = depthFrame.get_width();
    int height = depthFrame.get_height();

    DepthData depth;
    depth.width = width;
    depth.height = height;
    depth.depth_scale = depth_scale_;
    depth.frame.resize(width * height);

    std::memcpy(depth.frame.data(), depthFrame.get_data(),width * height * sizeof(uint16_t));

    return depth;
}

rs2::config realsense_capture::configure_pipeline() { //configure pipeline

    rs2::config cfg;
    try {
        cfg.enable_device(serial_number_);
        cfg.enable_stream(RS2_STREAM_DEPTH, STREAM_WIDTH, STREAM_HEIGHT, RS2_FORMAT_ANY, STREAM_FPS); //depth, 640x480, formato any (elige librealsense), 30fps
    }  catch (const rs2::error& e) {
        std::cerr << "RealSense Error while configuring Pipeline: " << e.what() << std::endl;
    }
    return cfg;
}