#ifndef NETWORK_H
#define NETWORK_H

#include "os.h"
#ifdef WIN_OS
    #include <stdint.h>
#else
    #include <inttypes.h>
#endif

struct network_data {
    uint32_t default_gateway;
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint32_t dst_addr;
    uint32_t src_addr;
    uint16_t dst_port;
    uint16_t src_port;
};

// windows only
int get_best_interface(uint32_t, void*);
// windows only, char* needs to be at least 256 bytes long
void get_interface_from_index(uint32_t, char*);

void net_load_addrs(const void*, struct network_data*);

int net_get_src_mac(const void*, uint8_t*);
uint32_t net_get_src_addr(const void*);
uint32_t net_get_default_gateway(const void*);

int sys_arp_table_lookup(uint32_t, uint8_t*);

uint16_t sys_htons(uint16_t);
uint32_t sys_htonl(uint32_t);
uint32_t sys_inet_addr(const char*);
char* sys_inet_ntoa(uint32_t);

void mac_cpy(uint8_t*, const uint8_t*);

#endif