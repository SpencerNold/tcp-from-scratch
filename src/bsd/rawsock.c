#include <stdio.h>
#if defined(__APPLE__) || defined(__MACH__) || defined(__FreeBSD__)

#include "rawsocket.h"

#include "network.h"
#include <pcap.h>

int listening = 0;

uint8_t src_mac[6];
uint32_t src_addr;

void (*raw_handler)(void*, const unsigned char*, int) = NULL;
void (*ethernet_handler)(void*, const unsigned char*, int) = NULL;
void (*ipv4_handler)(void*, const unsigned char*, int) = NULL;
void (*arp_handler)(void*, const unsigned char*, int) = NULL;

void packet_ipv4(pcap_t* handle, const unsigned char* data, int length) {
    if (ipv4_handler != NULL) {
        ipv4_handler(handle, data, length);
    }
}

void packet_arp(pcap_t* handle, const unsigned char* data, int length) {
    if (arp_handler != NULL) {
        arp_handler(handle, data, length);
    }
}

void packet_ethernet(pcap_t* handle, const unsigned char* data, int length) {
    if (ethernet_handler != NULL) {
        ethernet_handler(handle, data, length);
    }
    uint16_t type = sys_htons(((uint16_t*) (data + 12))[0]);
    // TODO Maybe support more types in the future
    if (type == 0x0800) {
        packet_ipv4(handle, data, length);
    } else if (type == 0x0806) {
        packet_arp(handle, data, length);
    }
}

void packet_handler(u_char* user_data, const struct pcap_pkthdr* header, const u_char* packet) {
    pcap_t* handle = (pcap_t*) user_data;
    int length = header->caplen;
    if (raw_handler != NULL) {
        raw_handler(handle, (const unsigned char*) packet, length);
    }
    for (int i = 0; i < 6; i++) {
        if (packet[i] != src_mac[i])
            return;
    }
    packet_ethernet(handle, (const unsigned char*) packet, length);
}

void* rawsock_open(const char* interface) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(interface, 1500 /* general mtu size */, 1, 1000, errbuf);
    if (handle == NULL) {
        return NULL;
    }
    net_get_src_mac(interface, src_mac);
    src_addr = net_get_src_addr(interface);
    return handle;
}

void rawsock_send(void* handle, unsigned char* data, int length) {
    pcap_sendpacket((pcap_t*) handle, (u_char*) data, length);
}

void rs_listen(pcap_t* handle) {
    if (!listening) {
        pcap_loop(handle, -1, packet_handler, (u_char*) handle);
        listening = 1;
    }
}

void rawsock_listen_ethernet(void* handle, void (*handler)(void*, const unsigned char*, int)) {
    ethernet_handler = handler;
    rs_listen((pcap_t*) handle);
}

void rawsock_listen_ipv4(void* handle, void (*handler)(void*, const unsigned char*, int)) {
    ipv4_handler = handler;
    rs_listen((pcap_t*) handle);
}

void rawsock_listen_arp(void* handle, void (*handler)(void*, const unsigned char*, int)) {
    arp_handler = handler;
    rs_listen((pcap_t*) handle);
}

void rawsock_listen_raw(void* handle, void (*handler)(void*, const unsigned char*, int)) {
    raw_handler = handler;
    rs_listen((pcap_t*) handle);
}

void rawsock_break(void* handle) {
    if (listening) {
        pcap_breakloop((pcap_t*) handle);
        listening = 0;
    }
}

void rawsock_close(void* handle) {
    pcap_close((pcap_t*) handle);
}

#endif