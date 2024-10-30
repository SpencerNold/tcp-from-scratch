#include "ip.h"
#include "endian.h"

void ip_set_ihl_ver(struct ip_h* header, unsigned char ihl, unsigned char ver) {
    if (endian()) {
        header->ihl_ver = (ver << 4) | ihl;
    } else {
        header->ihl_ver = (ihl << 4) | ver;
    }
}

int ip_get_ihl(unsigned char x) {
    if (endian()) {
        return (unsigned char) (x & 0x0F);
    } else {
        return (unsigned char) (x & 0xF0) >> 4;
    }
}

int ip_get_ver(unsigned char x) {
    if (endian()) {
        return (unsigned char) (x & 0xF0) >> 4;
    } else {
        return (unsigned char) (x & 0x0F);
    }
}

void ip_update_check(uint32_t* sum, uint16_t* buffer, int offset, int size) {
    if (offset)
        buffer = (uint16_t*) (((char*) buffer) + offset);
    register uint32_t value = *sum;
    while (size > 1) {
        value += *buffer++;
        size -= sizeof(uint16_t);
    }
    if (size)
        value += *(uint8_t*) buffer;
    *sum = value;
}

void ip_update_check_num(uint32_t* sum, uint16_t num) {
    ip_update_check(sum, &num, 0, 2);
}

uint16_t ip_finish_check(uint32_t* sum) {
    register uint32_t value = *sum;
    while (value >> 16)
        value = (value & 0xffff) + (value >> 16);
    return (uint16_t) (~value);
}