#include "os.h"
#ifdef LINUX_OS

#include "thread.h"

#include <pthread.h>

void sys_run_async(void* handle, void* (*func)(void*)) {
    pthread_t pid;
    pthread_create(&pid, NULL, func, handle);
}

#endif