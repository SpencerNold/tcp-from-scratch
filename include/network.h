#ifndef NETWORK_H
#define NETWORK_H

#include <inttypes.h>

int net_get_src_mac(const char*, uint8_t*);
uint32_t net_get_src_addr(const char*);

uint16_t sys_htons(uint16_t);
uint32_t sys_htonl(uint32_t);
uint32_t sys_inet_addr(const char*);

#endif