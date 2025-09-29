// third_party/ispc_tasksys/tasksys.h
#pragma once
#include <stdint.h>   // za int32_t / int64_t

#ifdef __cplusplus
extern "C" {
#endif

void* ISPCAlloc(void** handle, int64_t size, int32_t alignment);

void  ISPCLaunch(void* func, void* data, int32_t count);

void  ISPCSync(void* handle);

#ifdef __cplusplus
}
#endif
