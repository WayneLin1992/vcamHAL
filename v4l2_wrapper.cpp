#include "v4l2_wrapper.h"

V4L2Wrapper* V4L2Wrapper::NewV4L2Wrapper(const std::string device_path) {
  return new V4L2Wrapper(device_path);
}

V4L2Wrapper::V4L2Wrapper(const std::string device_path)
    : device_path_(std::move(device_path)), connection_count_(0) {}

V4L2Wrapper::~V4L2Wrapper() {}
