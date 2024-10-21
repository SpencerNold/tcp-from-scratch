#include "iputils.h"

uint16_t ip_checksum(uint16_t* buffer, int size) {
    unsigned long sum = 0;
    while(size > 1) {
        sum += *buffer++;
        size -= sizeof(uint16_t);
    }
    if(size)
        sum += *(uint8_t*)buffer;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (uint16_t)(~sum);
}