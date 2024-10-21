#ifndef ARP_H
#define ARP_H

#include "inttypes.h"

struct arp_req {
    uint16_t htype;
    uint16_t ptype;
    uint8_t hlen;
    uint8_t plen;
    uint16_t oper;
    uint8_t sha[6];
    uint32_t spa;
    uint8_t tha[6];
    uint32_t tpa;
} __attribute__((packed));

int arp_request(const char*, const char*);

#endif