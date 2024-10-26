#ifndef ETH_H
#define ETH_H

#include <inttypes.h>

#define IPV4 0x0800
#define ARP 0x0806

struct eth_h {
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t type;
} __attribute__((packed));

void eth_fill_header(struct eth_h*, uint8_t*, uint8_t*, uint16_t);
uint32_t eth_crc32(uint8_t*, int);

#endif