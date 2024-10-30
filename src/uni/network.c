#include "network.h"

#include "arp.h"

void net_load_addrs(const void* iface, struct network_data* data) {
    data->default_gateway = net_get_default_gateway(iface);
    sys_arp_table_lookup(data->default_gateway, data->dst_mac);
    net_get_src_mac(iface, data->src_mac);
    data->src_addr = net_get_src_addr(iface);
}


void mac_cpy(uint8_t* dst, const uint8_t* src) {
    int length = 6;
    char* d = (char*) dst;
    const char* s = (const char*) src;
    while (length--)
        *d++ = *s++;
}