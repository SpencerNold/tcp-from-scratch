#include "arp.h"

#include "network.h"
#include "rawsocket.h"
#include "eth.h"

#include <stdio.h>
#include <string.h>

int arp_request(const char* interface, const char* dst_addr) {
    // arp table lookup here

    char buffer[56] = {0};
    
    unsigned char dst_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    unsigned char src_mac[6];
    net_get_src_mac(interface, (unsigned char*) src_mac);

    struct eth_h* ethh = (struct eth_h*) buffer;
    eth_fill_header(ethh, dst_mac, src_mac, sys_htons(0x0806));

    struct arp_req* arpr = (struct arp_req*)(((unsigned char*) buffer) + sizeof(ethh));
    arpr->htype = sys_htons(1);
    arpr->ptype = sys_htons(0x0800);
    arpr->hlen = 6;
    arpr->plen = 4;
    arpr->oper = sys_htons(1);
    memcpy(arpr->sha, src_mac, 6);
    arpr->spa = net_get_src_addr(interface);
    // skip tha as it should remain 0
    arpr->tpa = sys_inet_addr(dst_addr);

    for (int i = 0; i < 56; i++) {
        printf("%02X ", (unsigned char) buffer[i]);
    }
    printf("\n");

    void* handle = rawsock_open();
    if (!handle) {
        return -1;
    }
    rawsock_send(handle, (unsigned char*) buffer, 56);
    return 0;
}