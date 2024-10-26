#include "eth.h"

void eth_fill_header(struct eth_h* header, uint8_t* dst_mac, uint8_t* src_mac, uint16_t type) {
    for (int i = 0; i < 6; i++) {
        header->dst_mac[i] = dst_mac[i];
        header->src_mac[i] = src_mac[i];
    }
    header->type = type;
}

uint32_t eth_crc32(uint8_t* message, int length) {
    uint32_t byte, mask;
    uint32_t crc = 0xFFFFFFFF;
    int i, j;
    for (i = 0; i < length; i++) {
        byte = message[i]; 
        crc = crc ^ byte;
        for (j = 7; j >= 0; j--) {
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
    }
    return ~crc;
}