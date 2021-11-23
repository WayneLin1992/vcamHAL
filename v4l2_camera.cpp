
#include "v4l2_camera.h"
#include "v4l2_wrapper.h"

V4L2Camera* V4L2Camera::NewV4L2Camera(int id, const std::string path) {
  HAL_LOG_ENTER();

  std::shared_ptr<V4L2Wrapper> v4l2_wrapper(V4L2Wrapper::NewV4L2Wrapper(path));
  if (!v4l2_wrapper) {
    HAL_LOGE("Failed to initialize V4L2 wrapper.");
    return nullptr;
  }

  return new V4L2Camera(id, std::move(v4l2_wrapper));
}

V4L2Camera::V4L2Camera(int id,
                       std::shared_ptr<V4L2Wrapper> v4l2_wrapper)
    : default_camera_hal::Camera(id),
      device_(std::move(v4l2_wrapper)),
      max_input_streams_(0),
      max_output_streams_({{0, 0, 0}}) {
  HAL_LOG_ENTER();
}

V4L2Camera::~V4L2Camera(){
}
