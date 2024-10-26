#ifndef TCP_H
#define TCP_H

#include <inttypes.h>
#include "network.h"

#define SYN 2

struct tcp_h {
    uint16_t src_port;
    uint16_t dst_port;
    uint32_t seq;
    uint32_t ack;
    uint8_t offs;
    uint8_t flags;
    uint16_t win;
    uint16_t checksum;
    uint16_t urp;
} __attribute__((packed));

void tcp_send_syn(void*, struct network_data*, uint16_t, uint16_t);

#endif