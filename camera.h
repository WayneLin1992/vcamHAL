#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <hardware/camera3.h>
#include <hardware/hardware.h>

namespace default_camera_hal
{
class Camera
{
public:
    Camera(int id);
    ~Camera();

    // common Camera Device (see <hardware/camera_common.h>)
    int openDevice(const hw_module_t *module, hw_device_t **device);
    int getInfo(struct camera_info *info);

    // Camera v3 Device(see <hardware/camera3.h>)

private:
    camera3_device_t mDevice;
    const int mId;
    bool mBusy;
    const static camera3_device_ops_t sOps;
    const camera3_callback_ops_t *mCallbackOps;
};
}  // namespace default_camera_hal

#endif  // CAMERA_H_INCLUDED
