#include <stdio.h>
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)

#include "rawsocket.h"

#pragma comment(lib, "Ws2_32.lib")

void* rawsock_open(const char*) {
    WSAData data;
}

#endif