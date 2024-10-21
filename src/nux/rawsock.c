#include <stdio.h>
#if defined(unix) || defined(__unix) || defined(__unix__) && !defined(__APPLE__) && !defined(__MACH__) && !defined(__FreeBSD__)

#include "rawsocket.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <unistd.h>

int running = 0;

void* rawsock_open() {
    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd == -1) {
        return NULL;
    }
    return sockfd;
}

void rawsock_send(void* handle, char* data, int length) {
    send((int) handle, data, length, 0);
}

void rawsock_listen(void* handle, void (*handler)(void*, const char*, int)) {
    running = 1;
    int size = 1500;
    char buffer[size];
    while (running) {
        int read = recv((int) handle, buffer, size, 0);
        handler(handle, buffer, read);
    }
}

void rawsock_break(void* handle) {
    running = 0;
}

void rawsock_close(void* handle) {
    close((int) handle);
}

#endif