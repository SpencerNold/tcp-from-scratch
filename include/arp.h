#ifndef ARP_H
#define ARP_H

#include "os.h"
#ifdef WIN_OS
    #include <stdint.h>
    #define PACKED
    #pragma pack(push, 1)
#else
    #include <inttypes.h>
    #define PACKED __attribute__((packed))
#endif

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
} PACKED;

#ifdef WIN_OS
    #pragma pack(pop)
#endif

int arp_request(const char*, uint32_t);


#endif