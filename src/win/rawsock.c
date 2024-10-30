#include "os.h"
#ifdef WIN_OS

#include <pcap.h>

void* rawsock_open(const char* iface) {
    return NULL;
}

void rawsock_send(void* handle, unsigned char* data, int length) {

}

void rawsock_listen_raw(void* handle, void (*handler)(void*, const unsigned char*, int)) {

}

void rawsock_listen_ethernet(void* handle, void (*handler)(void*, const unsigned char*, int)) {

}

void rawsock_listen_ipv4(void* handle, void (*handler)(void*, const unsigned char*, int)) {

}

void rawsock_listen_arp(void* handle, void (*handler)(void*, const unsigned char*, int)) {

}

void rawsock_listen_tcp(void* handle, void (*handler)(void*, const unsigned char*, int)) {

}

void rawsock_break(void* handle) {

}

void rawsock_close(void* handle) {

}

#endif