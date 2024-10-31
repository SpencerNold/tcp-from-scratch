#ifndef TCP_H
#define TCP_H

#include "network.h"
#include "os.h"
#ifdef WIN_OS
    #include <stdint.h>
    #define PACKED
    #pragma pack(push, 1)
#else
    #include <inttypes.h>
    #define PACKED __attribute__((packed))
#endif

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
} PACKED;

#ifdef WIN_OS
    #pragma pack(pop)
#endif

#define SYN 2
#define ACK 16

void tcp_send_syn(void*, struct network_data*, uint16_t, uint16_t);
void tcp_send_syn_ack(void*, struct tcp_h*);
void tcp_send_ack(void*, struct tcp_h*);

#endif