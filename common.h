#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <cutils/log.h>

#define HAL_LOGE(fmt, args...) do { \
    ALOGE("%s, %d: ",fmt, __func__, __LINE__, ##args);\
}while(0)

#define HAL_LOGV(fmt, args...) do{ \
    ALOGV("%s, %d: ", fmt, __func__, __LINE__, ##args);\
}while(0)

#define HAL_LOG_ENTER() HAL_LOGV("enter")
#define HAL_LOG_EXIT() HAL_LOGV("exit")

#endif // COMMON_H_INCLUDED
