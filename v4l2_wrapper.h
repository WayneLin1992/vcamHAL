#ifndef V4L2_WRAPPER_H_INCLUDED
#define V4L2_WRAPPER_H_INCLUDED

#include <array>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include "common.h"
class V4L2Wrapper {
    public:
        static V4L2Wrapper* NewV4L2Wrapper(const std::string device_path);
        ~V4L2Wrapper();

    private:
        V4L2Wrapper(const std::string device_path);
        int connection_count_;
};

#endif // V4L2_WRAPPER_H_INCLUDED
