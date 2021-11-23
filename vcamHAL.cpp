#include "vcamHAL.h"
#include <string.h>

#include <dirent.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <cstdlib>
#include <unordered_set>

#include <android-base/parseint.h>

vcam_hal::vcam_hal() : mCameras(), mCallbacks(NULL)
{
    HAL_LOG_ENTER();

    DIR *dir = opendir("/dev");
    if (dir == NULL) {
        HAL_LOGE("Fail to open /dev");
        return;
    }
    // find /dev/videox nodes
    dirent *ent;
    std::vector<std::string> nodes;
    while ((ent = readdir(dir))) {
        std::string desired = "video";
        size_t len = desired.size();
        if (strncmp(desired.c_str(), ent->d_name, len) == 0) {
            if (strlen(ent->d_name) > len && isdigit(ent->d_name[len])) {
                nodes.push_back(std::string("/dev/") + ent->d_name);
                HAL_LOGV("Find video node %s.", nodes.back().c_str());
            }
        }
    }
    /* V4L2  */
    std::unordered_set<std::string> buses;
    std::string bus;
    v4l2_capability cap;
    int fd;
    int id = 0;
    for (const auto &node : nodes) {
        // Open the node.
        fd = TEMP_FAILURE_RETRY(open(node.c_str(), O_RDWR));
        if (fd < 0) {
            HAL_LOGE("failed to open %s (%s).", node.c_str(), strerror(errno));
            continue;
        }
        // Read V4L2 capabilities.
        if (TEMP_FAILURE_RETRY(ioctl(fd, VIDIOC_QUERYCAP, &cap)) != 0) {
            HAL_LOGE("VIDIOC_QUERYCAP on %s fail: %s.", node.c_str(),
                     strerror(errno));
        } else if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
            HAL_LOGE("%s is not a V4L2 video device.", node.c_str());
        } else {
            // If the node is unique, add a camera for it.
            bus = reinterpret_cast<char *>(cap.bus_info);
            if (buses.insert(bus).second) {
                HAL_LOGV("Found unique bus at %s.", node.c_str());
                std::unique_ptr<V4L2Camera> cam(V4L2Camera::NewV4L2Camera(
                    id++, node));  // need TODO define V4L2Camera
                if (cam) {
                    mCameras.push_back(std::move(cam));
                } else {
                    HAL_LOGE("Failed to initialize camera at %s.",
                             node.c_str());
                }
            }
        }
        close(fd);
    }
}

vcam_hal::~vcam_hal()
{
    HAL_LOG_ENTER();
}

int vcam_hal::openDev(const hw_module_t *module,
                      const char *name,
                      hw_device_t **device)
{
    if (!name) {
        HAL_LOGE("Invalid module %p", module);
        return -EINVAL;
    }
    int id = mCameras.size();
    return mCameras[id]->openDevice(module, device);
}

int vcam_hal::get_number_of_cameras()
{
    HAL_LOGV("return %zu", mCameras.size());
    return mCameras.size();
}

int vcam_hal::get_camera_info(int id, camera_info_t *info)
{
    HAL_LOG_ENTER();
    return mCameras[id]->getInfo(info);
}

int vcam_hal::set_callbacks(const camera_module_callbacks_t *callbacks)
{
    HAL_LOG_ENTER();
    mCallbacks = callbacks;
    return 0;
}

int vcam_hal::set_torch_mode(const char *camera_id, bool on)
{
    return 0;
}

static hw_module_methods_t v4l2_module_methods = {.open = vcam_hal::openDev};

static hw_module_t camera_common = {
    .tag = HARDWARE_MODULE_TAG,
    .module_api_version = CAMERA_DEVICE_API_VERSION_2_4,
    .hal_api_version = HARDWARE_HAL_API_VERSION,
    .id = CAMERA_HARDWARE_MODULE_ID,
    .name = "Vcam Module",
    .author = "WayneLin1992",
    .methods = &v4l2_module_methods,
    .dso = nullptr,
    .reserved = {0}};

camera_module_t HAL_MODULE_INFO_SYM = {
    .common = camera_common,
    .get_number_of_cameras = vcam_hal::get_number_of_cameras,
    .get_camera_info = vcam_hal::get_camera_info,
    .set_callbacks = vcam_hal::set_callbacks,
    .get_vendor_tag_ops = nullptr,
    .open_legacy = nullptr,
    .set_torch_mode = vcam_hal::set_torch_mode,
    .init = nullptr,
    .get_physical_camera_info = nullptr,
    .reserved = {0}};
