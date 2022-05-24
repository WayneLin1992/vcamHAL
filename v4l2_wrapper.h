#ifndef V4L2_WRAPPER_H_INCLUDED
#define V4L2_WRAPPER_H_INCLUDED

#include <array>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include "common.h"
class V4L2Wrapper
{
public:
    static V4L2Wrapper *NewV4L2Wrapper(const std::string device_path);
    ~V4L2Wrapper();
    
    // stream on or off
    virtual int StreamOn();
    virtual int StreamOff();
    
    // Manage controls.
    virtual int QueryControl(uint32_t control_id, v4l2_query_ext_ctrl* result);
    virtual int GetControl(uint32_t control_id, int32_t* value);
    virtual int SetControl(uint32_t control_id, int32_t desired, int32_t* result = nullptr);
    
    // Manage format
    virtual int GetFormats(std::set<uint32_t>* v4l2_formats);
    
    
private:
    V4L2Wrapper(const std::string device_path);
    int connection_count_;
    template<typename T>
    int IoctlLock(unsigned long request, T data);
    
    // The camera device path For example, /dev/videox
    const std::string device_path_;
    // open device fd
    android::base::unique_fd device_fd_;
    // Lock protect device
    std::mutex device_lock_;
    
};

#endif  // V4L2_WRAPPER_H_INCLUDED
