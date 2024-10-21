/*
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tcp_pseudo_header {
    uint32_t src_addr;
    uint32_t dst_addr;
    uint8_t res;
    uint8_t proto;
    uint16_t len;
} __attribute__((packed));



int main() {
    int sockfd_in = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd_in == -1) {
        return 3;
    }
    char buffer[65535];
    int size = 65535;
    while (1) {
        int length = recv(sockfd_in, buffer, size, 0);
        printf("%d\n", length);
    }
}

int main0() {
    struct sockaddr_in src_addr;
    src_addr.sin_family = AF_INET;
    src_addr.sin_port = htons(49656);
    src_addr.sin_addr.s_addr = inet_addr("10.117.47.73");
    
    struct sockaddr_in dst_addr;
    dst_addr.sin_family = AF_INET;
    dst_addr.sin_port = htons(80);
    dst_addr.sin_addr.s_addr = inet_addr("128.119.245.12");

    int sockfd_out = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sockfd_out == -1) {
        return 1;
    }
    int one = 1;
    if (setsockopt(sockfd_out, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        return 2;
    }

    int sockfd_in = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd_in == -1) {
        return 3;
    }

    char* data = (char*) malloc(sizeof(char) * 64);

    struct ip* iph = (struct ip*) data;
    iph->ip_v = 4;
    iph->ip_hl = 5;
    iph->ip_tos = 0;
    iph->ip_len = (64);  // possible that it must be host order for mac and network order for linux
    iph->ip_id = htons(0);
    iph->ip_off = 0x4000; // possible that it must be host order for mac and network order for linux
    iph->ip_ttl = 64;
    iph->ip_p = 6;
    iph->ip_sum = 0;
    iph->ip_src.s_addr = src_addr.sin_addr.s_addr; // ??? 
    iph->ip_dst.s_addr = dst_addr.sin_addr.s_addr;
    iph->ip_sum = checksum((unsigned short*) iph, 20);

    struct tcphdr* tcph = (struct tcphdr*) (data + 20);
    tcph->th_sport = htons(49656);
    tcph->th_dport = htons(80);
    tcph->th_seq = htonl(4129428372);
    tcph->th_ack = htonl(0);
    tcph->th_off = 11;
    tcph->th_x2 = 0; // May be non-zero, so needs to be manually set
    tcph->th_flags = TH_SYN;
    tcph->th_sum = 0;
    tcph->th_win = htons(65535);
    tcph->th_urp = htons(0);

    char array[] = { 0x02, 0x04, 0x05, 0xb4, 0x01, 0x03, 0x03, 0x06, 0x01, 0x01, 0x08, 0x0a, 0xde, 0xf5, 0x86, 0x92, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00 };
    memcpy(data + 40, array, 24);
    
    struct tcp_pseudo_header tcpph;
    tcpph.src_addr = htonl(inet_addr(src_addr));
    tcpph.dst_addr = htonl(inet_addr(dst_addr));
    tcpph.res = 0;
    tcpph.proto = 6;
    tcpph.len = htons(44);

    char* pseudo = (char*) malloc(56);
    memcpy(pseudo, &tcph, 12);
    memcpy(pseudo + 12, data + 20, 44);
    unsigned short sum = checksum((unsigned short*) pseudo, 56);
    //printf("%04X\n", (sum));

    tcph->th_sum = htons(0x5632);



    sendto(sockfd_out, data, 64, 0, (struct sockaddr*) &dst_addr, sizeof(dst_addr));

    char buffer[4096];
    int size = 4096;
    while (1) {
        int length = recv(sockfd_in, buffer, size, 0);
        printf("%d\n", length);
    }
}
*/

#include "rawsocket.h"
#include "network.h"
#include "eth.h"

#include "arp.h"

#include <stdio.h>

void function(void* handle, const unsigned char* data, int length) {
}

int main() {
    /*
    unsigned char mac[6];
    eth_get_src_mac("en0", (unsigned char*) mac);
    printf("%02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);


    void* handle = rawsock_open();

    //rawsock_listen(handle, function);
    rawsock_close(handle);
    */
   arp_request("en0", "128.119.245.12");
}