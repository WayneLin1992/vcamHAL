#include <vcamHAL.h>
#include <string.h>

int vcam_hal::openDev(const hw_module_t* module, const char* name, hw_device_t** device){
    if (!name){
        HAL_LOGE("Invalid module %p", module);
        return -EINVAL;
    }
    int id = atoi(name);
    return mCameras[id]->openDevice(module, device); //not working;
}

int vcam_hal::get_number_of_cameras(){
    HAL_LOGV("return %zu", NumOfCameras);
    return NumOfCameras;
}

int vcam_hal::get_camera_info(int id, camera_info_t* info){
    HAL_LOG_ENTER();
    return mCameras[id]->getInfo(info);
}

int vcam_hal::set_callbacks(const camera_module_callbacks_t* callbacks){
    HAL_LOG_ENTER();
    mCallbacks = callbacks;
    return 0;
}

int vcam_hal::set_torch_mode(const char* camera_id, bool on){
    return 0;
}



static hw_module_methods_t v4l2_module_methods = {
    .open = vcam_hal::openDev};

static hw_module_t camera_common = {
    .tag                    = HARDWARE_MODULE_TAG,
    .module_api_version     =  CAMERA_DEVICE_API_VERSION_2_4,
    .hal_api_version        = HARDWARE_HAL_API_VERSION,
    .id                     = CAMERA_HARDWARE_MODULE_ID,
    .name                   = "Vcam Module",
    .author                 = "WayneLin1992",
    .methods                = &v4l2_module_methods,
    .dso                    = nullptr,
    .reserved               = {0}
};

camera_module_t HAL_MODULE_INFO_SYM  = {
    .common                 = camera_common,
    .get_number_of_cameras  = vcam_hal::get_number_of_cameras,
    .get_camera_info        = vcam_hal::get_camera_info,
    .set_callbacks          = vcam_hal::set_callbacks,
    .get_vendor_tag_ops     = nullptr,
    .open_legacy            = nullptr,
    .set_torch_mode         = vcam_hal::set_torch_mode,
    .init                   = nullptr,
    .get_physical_camera_info = nullptr,
    .reserved               = {0}
};
