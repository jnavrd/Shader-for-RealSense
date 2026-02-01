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


DepthData realsense_capture::get_depth_data() {

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

DepthDataFloat realsense_capture::get_depth_data_float() {
    rs2::frameset frameset = pipeline_.wait_for_frames();
    rs2::depth_frame depthFrame = frameset.get_depth_frame();

    if (!depthFrame){
        std::cerr << "No depth frame";
        return {};
    }

    int width = depthFrame.get_width();
    int height = depthFrame.get_height();

    DepthDataFloat depth;
    depth.width = width;
    depth.height = height;
    depth.frame.resize(width * height);

    //float vector, to be used in shader
    const uint16_t* raw_data = reinterpret_cast<const uint16_t*>(depthFrame.get_data());

    for(int i = 0; i < width * height; i++) {
        depth.frame[i] = static_cast<float>(raw_data[i]) * depth_scale_; //depth en metros
    }

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

float realsense_capture::getDepthScale() const {
    return depth_scale_;
}
