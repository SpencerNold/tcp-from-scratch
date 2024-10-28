#include <stdio.h>
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__) || defined(__FreeBSD__)

#include "thread.h"

#include <pthread.h>

void sys_run_async(void* handle, void* (*func)(void*)) {
    pthread_t pid;
    pthread_create(&pid, NULL, func, handle);
}

#endif