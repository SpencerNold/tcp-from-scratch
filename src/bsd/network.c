#include "os.h"
#ifdef BSD_OS

#include "network.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if_dl.h>
#include <sys/sysctl.h>
#include <net/route.h>
#include <string.h>
#include <netinet/if_ether.h>

uint8_t stored_src_mac_address[6];
uint32_t stored_src_ipv4_address = 0;
int is_src_mac_stored = 0, is_src_ipv4_stored = 0;
char stored_interface[16] = {0};

void store_inet_addrs(const void* interface) {
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) {
        return;
    }
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) {
            continue;
        }
        if (strcmp(ifa->ifa_name, (const char*) interface) == 0) {
            // MAC Addr
            if (ifa->ifa_addr->sa_family == AF_LINK) {
                struct sockaddr_dl* sdl = (struct sockaddr_dl*) ifa->ifa_addr;
                unsigned char* mac = (unsigned char*) LLADDR(sdl);
                for (int i = 0; i < 6; i++) {
                    stored_src_mac_address[i] = mac[i];
            }
            }
            // IPv4 Addr
            if (ifa->ifa_addr->sa_family == AF_INET) {
                struct sockaddr_in* sin = (struct sockaddr_in*) ifa->ifa_addr;
                stored_src_ipv4_address = sin->sin_addr.s_addr;
            }
        }
    }
    freeifaddrs(ifaddr);
    is_src_mac_stored = 1;
    is_src_ipv4_stored = 1;
    strcpy((char*) stored_interface, (const char*) interface);
}

int net_get_src_mac(const void* interface, uint8_t* mac) {
    if (!is_src_mac_stored || strcmp((char*) stored_interface, (const char*) interface) != 0) {
        store_inet_addrs(interface);
    }
    for (int i = 0; i < 6; i++)
        mac[i] = stored_src_mac_address[i];
    return 0;
}

uint32_t net_get_src_addr(const void* interface) {
    if (!is_src_ipv4_stored || strcmp((char*) stored_interface, (const char*) interface) != 0) {
        store_inet_addrs(interface);
    }
    return stored_src_ipv4_address;
}

int get_best_interface(uint32_t addr, void* dst_iface) {
    return 0; // TODO Implement
}

char* get_interface_from_index(void* iface) {
    return NULL;
}

#define ROUNDUP(a, size) (((a) & ((size) - 1)) ? (1 + ((a) | ((size) - 1))) : (a))

uint32_t net_get_default_gateway(const void* interface) {
    int mib[6] = { CTL_NET, PF_ROUTE, 0, AF_INET, NET_RT_FLAGS, RTF_GATEWAY };
    size_t needed;
    if (sysctl(mib, 6, NULL, &needed, NULL, 0) < 0) {
        return 0;
    }
    char data[needed];
    if (sysctl(mib, 6, data, &needed, NULL, 0)) {
        return 0;
    }
    char* buf = (char*) data;
    char* lim = buf + needed;
    char* next;
    struct rt_msghdr* rtm;
    struct sockaddr* sa;
    struct sockaddr_in* sin;
    struct sockaddr_dl* sdl;
    int found = 0;
    for (next = buf; next < lim; next += rtm->rtm_msglen) {
        rtm = (struct rt_msghdr*) next;
        sa = (struct sockaddr *)(rtm + 1);
        for (int i = 0; i < RTAX_MAX; i++) {
            if (rtm->rtm_addrs & (1 << i)) {
                if (i == RTAX_DST) {
                    sin = (struct sockaddr_in*) sa;
                    if (sin->sin_addr.s_addr == 0) {
                        found = 1;
                    }
                }
                if (found && i == RTAX_GATEWAY) {
                    if (sa->sa_family == AF_INET) {
                        sin = (struct sockaddr_in*) sa;
                        return sin->sin_addr.s_addr;
                    }
                }
                sa = (struct sockaddr *)((char *)sa + ROUNDUP(sa->sa_len, sizeof(long)));
            }
        }
    }
    return 0;
}

int sys_arp_table_lookup(uint32_t address, uint8_t* mac) {
    int mib[6] = { CTL_NET, PF_ROUTE, 0, AF_INET, NET_RT_FLAGS, RTF_LLINFO };
    size_t needed;
    if (sysctl(mib, 6, NULL, &needed, NULL, 0) < 0) {
        return -2;
    }
    char data[needed];
    if (sysctl(mib, 6, data, &needed, NULL, 0) < 0) {
        return -3;
    }
    char* buf = (char*) data;
    char* lim = buf + needed;
    char* next;
    struct rt_msghdr* rtm;
    struct sockaddr_inarp* sin;
    struct sockaddr_dl* sdl;
    for (next = buf; next < lim; next += rtm->rtm_msglen) {
        rtm = (struct rt_msghdr*) next;
        sin = (struct sockaddr_inarp*) (rtm + 1);
        sdl = (struct sockaddr_dl*) (sin + 1);
        if (address != sin->sin_addr.s_addr) {
            continue;
        }
        unsigned char* mac_addr = (unsigned char*) LLADDR(sdl);
        for (int i = 0; i < 6; i++) {
            mac[i] = mac_addr[i];
        }
        return 0;
    }
    return -1;
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

char* sys_inet_ntoa(uint32_t addr) {
    struct in_addr in_addr;
    in_addr.s_addr = addr;
    return inet_ntoa(in_addr);
}

#endif