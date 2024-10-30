#ifndef ETH_H
#define ETH_H

#include "os.h"
#ifdef WIN_OS
    #include <stdint.h>
    #define PACKED
    #pragma pack(push, 1)
#else
    #include <inttypes.h>
    #define PACKED __attribute__((packed))
#endif

struct eth_h {
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t type;
} PACKED;

#ifdef WIN_OS
    #pragma pack(pop)
#endif

#define IPV4 0x0800
#define ARP 0x0806

void eth_fill_header(struct eth_h*, uint8_t*, uint8_t*, uint16_t);
uint32_t eth_crc32(uint8_t*, int);

#endif