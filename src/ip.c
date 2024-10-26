#include "ip.h"

int endian() {
    int i = 1;
    return (int) *((unsigned char*) &i) == 1;
}

void ip_set_ihl_ver(struct ip_h* header, unsigned char ihl, unsigned char ver) {
    if (endian()) {
        header->ihl_ver = (ver << 4) | ihl;
    } else {
        header->ihl_ver = (ihl << 4) | ver;
    }
}

void ip_update_check(uint32_t* sum, uint16_t* buffer, int offset, int size) {
    if (offset)
        buffer = (uint16_t*) (((char*) buffer) + offset);
    while (size > 1) {
        *sum += *buffer++;
        size -= sizeof(uint16_t);
    }
    if (size)
        *sum += *(uint8_t*) buffer;
}

void ip_update_check_num(uint32_t* sum, uint16_t num) {
    ip_update_check(sum, &num, 0, 2);
}

uint16_t ip_finish_check(uint32_t* sum) {
    uint32_t value = *sum;
    value = (value >> 16) + (value & 0xffff);
    value += (value >> 16);
    return (uint16_t) (~value);
}