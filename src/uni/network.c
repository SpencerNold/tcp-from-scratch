#include "network.h"

#include "arp.h"

void net_load_addrs(const char* interface, struct network_data* data) {
    data->default_gateway = net_get_default_gateway();
    arp_table_lookup(data->default_gateway, data->dst_mac);
    net_get_src_mac(interface, data->src_mac);
    data->src_addr = net_get_src_addr(interface);
}


void mac_cpy(uint8_t* dst, const uint8_t* src) {
    int length = 6;
    char* d = (char*) dst;
    const char* s = (const char*) src;
    while (length--)
        *d++ = *s++;
}