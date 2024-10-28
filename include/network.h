#ifndef NETWORK_H
#define NETWORK_H

#include <inttypes.h>

struct network_data {
    uint32_t default_gateway;
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint32_t dst_addr;
    uint32_t src_addr;
    uint16_t dst_port;
    uint16_t src_port;
};

void net_load_addrs(const char*, struct network_data*);

int net_get_src_mac(const char*, uint8_t*);
uint32_t net_get_src_addr(const char*);
uint32_t net_get_default_gateway();

uint16_t sys_htons(uint16_t);
uint32_t sys_htonl(uint32_t);
uint32_t sys_inet_addr(const char*);
char* sys_inet_ntoa(uint32_t);

void mac_cpy(uint8_t*, const uint8_t*);

#endif