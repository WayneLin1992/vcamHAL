#ifndef V4L2L_CAMERA_H_INCLUDED
#define V4L2L_CAMERA_H_INCLUDED

#include "camera.h"
#include "common.h"

class V4L2Camera : public default_camera_hal::Camera
{
public:
    static V4L2Camera *NewV4L2Camera(int id, const std::string path);
    ~V4L2Camera();
};

#endif  // V4L2L_CAMERA_H_INCLUDED
