#include "tcp.h"

#include "rawsocket.h"
#include "network.h"
#include "eth.h"
#include "ip.h"

#include <stdio.h>

#define ETH_SIZE 14
#define IP_SIZE 20
#define TCP_SIZE 20
#define SIZE ETH_SIZE + IP_SIZE + TCP_SIZE

void construct_generic_eth_ip(uint8_t* data, struct network_data* addrs) {
    struct eth_h* eth = (struct eth_h*) data;
    mac_cpy(eth->dst_mac, addrs->dst_mac);
    mac_cpy(eth->src_mac, addrs->src_mac);
    eth->type = sys_htons(IPV4);

    struct ip_h* ip = (struct ip_h*) (data + ETH_SIZE);
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
    ip_update_check(&sum, (uint16_t*) ip, 0, IP_SIZE);
    ip->checksum = ip_finish_check(&sum);
}

void tcp_send_syn(void* handle, struct network_data* addrs, uint16_t src_port, uint16_t dst_port) {
    // no options involved yet
    char buf[SIZE] = {0};
    uint8_t* data = (uint8_t*) buf;
    construct_generic_eth_ip(data, addrs);

    struct tcp_h* tcp = (struct tcp_h*) (data + ETH_SIZE + IP_SIZE);
    tcp->src_port = sys_htons(src_port);
    tcp->dst_port = sys_htons(dst_port);
    tcp->seq = sys_htonl(0);
    tcp->ack = sys_htonl(0);
    tcp->offs = 0x50;
    tcp->flags = SYN;
    tcp->win = sys_htons(65535);
    tcp->checksum = 0;
    tcp->urp = 0;
    uint32_t sum = 0;
    ip_update_check(&sum, (uint16_t*) (data + ETH_SIZE), 12, 8);
    ip_update_check_num(&sum, sys_htons(6));
    ip_update_check_num(&sum, sys_htons(20));
    ip_update_check(&sum, (uint16_t*) tcp, 0, TCP_SIZE);
    tcp->checksum = ip_finish_check(&sum);

    rawsock_send(handle, data, SIZE);
}

void tcp_send_syn_ack(void* handle, struct network_data* addrs, struct tcp_h* syn) {
    char buf[SIZE] = {0};
    uint8_t* data = (uint8_t*) buf;
    construct_generic_eth_ip(data, addrs);

    struct tcp_h* tcp = (struct tcp_h*) (data + ETH_SIZE + IP_SIZE);
    tcp->src_port = syn->dst_port;
    tcp->dst_port = syn->src_port;
    tcp->seq = sys_htonl(3);
    tcp->ack = syn->seq + 1;
    tcp->offs = 0x50;
    tcp->flags = (SYN | ACK);
    tcp->win = sys_htons(65535);
    tcp->urp = 0;
    uint32_t sum = 0;
    ip_update_check(&sum, (uint16_t*) (data + ETH_SIZE), 12, 8);
    ip_update_check_num(&sum, sys_htons(6));
    ip_update_check_num(&sum, sys_htons(20));
    ip_update_check(&sum, (uint16_t*) tcp, 0, TCP_SIZE);
    tcp->checksum = ip_finish_check(&sum);

    rawsock_send(handle, data, SIZE);
}