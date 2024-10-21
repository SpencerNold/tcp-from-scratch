#include "eth.h"

void eth_fill_header(struct eth_h* header, uint8_t* dst_mac, uint8_t* src_mac, uint16_t type) {
    for (int i = 0; i < 6; i++) {
        header->dst_mac[i] = dst_mac[i];
        header->src_mac[i] = src_mac[i];
    }
    header->type = type;
}