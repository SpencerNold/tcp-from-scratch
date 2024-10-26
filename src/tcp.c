#include "tcp.h"

#include "rawsocket.h"
#include "network.h"
#include "eth.h"
#include "ip.h"

#include <stdio.h>

void tcp_send_syn(void* handle, struct network_data* addrs, uint16_t src_port, uint16_t dst_port) {
    // no options involved yet
    // eth = 14, ipv4h = 20, tcph = 20
    int size = 14 + 20 + 20;
    char buf[size];
    uint8_t* data = (uint8_t*) buf;

    struct eth_h* eth = (struct eth_h*) data;
    mac_cpy(eth->dst_mac, addrs->dst_mac);
    mac_cpy(eth->src_mac, addrs->src_mac);
    eth->type = sys_htons(IPV4);

    struct ip_h* ip = (struct ip_h*) (data + 14);
    ip_set_ihl_ver(ip, 5, 4);
    ip->tos = 0;
    ip->len = sys_htons(40);
    ip->id = sys_htons(69);
    ip->frag_off = sys_htons(0x4000);
    ip->ttl = 64;
    ip->protocol = TCP;
    ip->checksum = 0;
    ip->src_addr = addrs->src_addr;
    ip->dst_addr = addrs->dst_addr;
    uint32_t sum = 0;
    ip_update_check(&sum, (uint16_t*) ip, 0, 20);
    ip->checksum = ip_finish_check(&sum);

    struct tcp_h* tcp = (struct tcp_h*) (data + 14 + 20);
    tcp->src_port = sys_htons(src_port);
    tcp->dst_port = sys_htons(dst_port);
    tcp->seq = sys_htonl(0);
    tcp->ack = sys_htonl(0);
    tcp->offs = 0x50;
    tcp->flags = SYN;
    tcp->win = sys_htons(65535);
    tcp->urp = 0;
    sum = 0;
    ip_update_check(&sum, (uint16_t*) ip, 12, 8);
    ip_update_check_num(&sum, sys_htons(6));
    ip_update_check_num(&sum, sys_htons(20));
    ip_update_check(&sum, (uint16_t*) tcp, 0, 20);
    tcp->checksum = ip_finish_check(&sum);

    rawsock_send(handle, data, size);
}