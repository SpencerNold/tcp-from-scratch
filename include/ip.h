#ifndef IP_H
#define IP_H

#include <inttypes.h>

#define TCP 0x06

struct ip_h {
    uint8_t ihl_ver;
    uint8_t tos;
    uint16_t len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t src_addr;
    uint32_t dst_addr;
} __attribute__((packed));

void ip_set_ihl_ver(struct ip_h*, unsigned char, unsigned char);

void ip_update_check(uint32_t*, uint16_t*, int, int);
void ip_update_check_num(uint32_t*, uint16_t);
uint16_t ip_finish_check(uint32_t*);

#endif