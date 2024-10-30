#ifndef OS_H
#define OS_H

#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    #define WIN_OS
#endif

#if defined(__APPLE__) || defined(__MACH__) || defined(__FreeBSD__)
    #define BSD_OS
#endif

#if defined(unix) || defined(__unix) || defined(__unix__) && !defined(__APPLE__) && !defined(__MACH__) && !defined(__FreeBSD__)
    #define LINUX_NON_BSD_OS
#endif

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__) || defined(__FreeBSD__)
    #define LINUX_OS
#endif

#endif