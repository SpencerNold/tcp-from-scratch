#ifndef ETH_H
#define ETH_H

#include "iputils.h"

struct eth_h {
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t type;
} __attribute__((packed));

void eth_fill_header(struct eth_h*, uint8_t*, uint8_t*, uint16_t);

#endif