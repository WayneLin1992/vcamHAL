#ifndef VCAMHAL_H_INCLUDED
#define VCAMHAL_H_INCLUDED

#include <vector>

#include <hardware/hardware.h>
#include <hardware/camera3.h>

#include "common.h"
#include "camera.h"
class vcam_hal {
    public:
        vcam_hal();
        ~vcam_hal();
        int openDev(const hw_module_t* module, const char* name, hw_device_t** device);
        int get_number_of_cameras();
        int get_camera_info(int id, camera_info_t* info);
        int set_callbacks(const camera_module_callbacks_t* callbacks);
        int set_torch_mode(const char* camera_id, bool on);

    private:
        const camera_module_callbacks_t* mCallbacks;
        std::vector<std::unique_ptr<default_camera_hal::Camera>> mCameras;
};

extern camera_module_t HAL_MODULE_INFO_SYM;

#endif // VCAMHAL_H_INCLUDED
