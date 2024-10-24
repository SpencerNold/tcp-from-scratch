#include <stdio.h>
#if defined(__APPLE__) || defined(__MACH__) || defined(__FreeBSD__)

#include "network.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if_dl.h>
#include <string.h>

uint8_t stored_src_mac_address[6];
uint32_t stored_src_ipv4_address = 0;
int is_src_mac_stored = 0, is_src_ipv4_stored = 0;
char stored_interface[16] = {0};

void store_inet_addrs(const char* interface) {
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) {
        return;
    }
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_LINK) {
            continue;
        }
        if (strcmp(ifa->ifa_name, interface) == 0) {
            // MAC Addr
            struct sockaddr_dl* sdl = (struct sockaddr_dl*) ifa->ifa_addr;
            unsigned char* mac = (unsigned char*) LLADDR(sdl);
            for (int i = 0; i < 6; i++) {
                stored_src_mac_address[i] = mac[i];
            }
            // IPv4 Addr
            struct sockaddr_in* sin = (struct sockaddr_in*) ifa->ifa_addr;
            stored_src_ipv4_address = sin->sin_addr.s_addr;
        }
    }
    freeifaddrs(ifaddr);
    is_src_mac_stored = 1;
    is_src_ipv4_stored = 1;
    strcpy((char*) stored_interface, interface);
}

int net_get_src_mac(const char* interface, uint8_t* mac) {
    if (!is_src_mac_stored || strcmp((char*) stored_interface, interface) != 0) {
        store_inet_addrs(interface);
    }
    for (int i = 0; i < 6; i++)
        mac[i] = stored_src_mac_address[i];
    return 0;
}

uint32_t net_get_src_addr(const char* interface) {
    if (!is_src_ipv4_stored || strcmp((char*) stored_interface, interface) != 0) {
        store_inet_addrs(interface);
    }
    return stored_src_ipv4_address;
}

uint16_t sys_htons(uint16_t n) {
    return htons(n);
}

uint32_t sys_htonl(uint32_t n) {
    return htonl(n);
}

uint32_t sys_inet_addr(const char* addr) {
    return (uint32_t) inet_addr(addr);
}

#endif