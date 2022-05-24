#include "camera.h"

#include <hardware/camera3.h>

extern "C" {
// passed to the framework to close an opened device.
static int close_device(hw_device_t* dev)
{
    camera3_device_t* cam_dev = reinterpret_cast<camera3_device_t*>(dev);
    Camera* cam = static_cast<Camera*>(cam_dev->priv);
    return cam->close();
}
} // extern "C"

namespace default_camera_hal
{
Camera::Camera(int id) : mId(id), mbusy(false), mCallbackOps(NULL)
{
    memset(&mDevice, 0, sizeof(mDevice));
    mDevice.common.tag = HARDWARE_DEVICE_TAG;
    mDevice.common.version = CAMERA_DEVICE_API_VERSION_3_4;
    mDevice.common.close  = close_device;
    mDevice.ops = const_cast<camera3_device_ops_t *>(&sOps);
    mDevice.priv = this;
}
}  // namespace default_camera_hal

Camera::~Camera() {}

int Camera::openDevice(const hw_module_t *module, hw_device_t **device)
{
    if (mbusy) {
        return -EBUSY;
    }
    int connectResult = connect();
    if (connectResult) {
        return connectResult;
    }
    mbusy = true;
    mDevice.common.module = const_cast<hw_module_t *>(module);
    *device = &mDevice.common;
    return 0;
}

int Camera::getInfo(struct camera_info *info)
{
    info->device_version = mDevice.common.version;

    /* init info */
    info->resource_cost = 100;
    info->conflicting_devices = nullptr;
    info->conflicting_devices_length = 0;

    /* TODO info*/
    return 0;
}
