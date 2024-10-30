#include "rawtcp.h"

#include <stdio.h>

#ifdef WIN_OS

int main() {
    uint32_t iface_idx;
    get_best_interface(sys_inet_addr("128.119.245.12"), &iface_idx);

    uint8_t mac[6];
    net_get_src_mac(&iface_idx, mac);
    uint32_t addr = net_get_src_addr(&iface_idx);
    uint32_t gateway = net_get_default_gateway(&iface_idx);
    uint8_t dst_mac[6];
    sys_arp_table_lookup(gateway, dst_mac);

    printf("IPv4=%s\n", sys_inet_ntoa(addr));
    printf("Mac=%02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    printf("Default-Gateway=%s\n", sys_inet_ntoa(gateway));
    printf("DST Mac=%02X:%02X:%02X:%02X:%02X:%02X\n", dst_mac[0], dst_mac[1], dst_mac[2], dst_mac[3], dst_mac[4], dst_mac[5]);
}
    
#else

#include <arpa/inet.h>

void ipv4_handle(void* handle, const unsigned char* data, int length) {
    if (length < 40)
        return;
    struct tcp_h* header = (struct tcp_h*) (data + 20);
    if (sys_htons(header->src_port) == 80 && header->flags == (SYN | ACK)) {
        uint32_t addr = *((uint32_t*) (data + 12));
        printf("%s\n", sys_inet_ntoa(addr));
    }
}

void* ipv4_rs_listen(void* handle) {
    rawsock_listen_ipv4(handle, ipv4_handle);
    return NULL;
}

// temp for sleep function and testing
#include <unistd.h>

int main() {
    const char* interface = "en0";
    
    struct network_data addrs;
    net_load_addrs(interface, &addrs);
    addrs.dst_addr = sys_inet_addr("128.119.245.12");

    void* handle = rawsock_open(interface);
    if (handle == NULL) {
        return -1;
    }
    sys_run_async(handle, ipv4_rs_listen);

    sleep(1);
    tcp_send_syn(handle, &addrs, 8192, 80);

    while (1) {}

    rawsock_close(handle);
}

#endif

