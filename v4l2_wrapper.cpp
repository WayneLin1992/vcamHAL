#include "v4l2_wrapper.h"

V4L2Wrapper* V4L2Wrapper::NewV4L2Wrapper(const std::string device_path) {
  return new V4L2Wrapper(device_path);
}

V4L2Wrapper::V4L2Wrapper(const std::string device_path)
    : device_path_(std::move(device_path)), connection_count_(0) {}

V4L2Wrapper::~V4L2Wrapper() {}

int V4L2Wrapper::IoctlLock(unsigned long request, T data){
    std::lock_guard<std::mutex> lock(device_lock_);
    return ioctl(device_fd_.get(), request, data);
}

int V4L2Wrapper::StreamOn() {
  
  int32_t type = format_->type();
  if (IoctlLocked(VIDIOC_STREAMON, &type) < 0) {
    HAL_LOGE("STREAMON fails (%d): %s", errno, strerror(errno));
    return -ENODEV;
  }

  HAL_LOGV("Stream turned on.");
  return 0;
}

int V4L2Wrapper::GetFormats(std::set<uint32_t>* v4l2_formats) {
    
  v4l2_fmtdesc format_query;
  memset(&format_query, 0, sizeof(format_query));
    
  format_query.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  while (IoctlLocked(VIDIOC_ENUM_FMT, &format_query) >= 0) {
    v4l2_formats->insert(format_query.pixelformat);
    ++format_query.index;
  }
    
  return 0;
}
