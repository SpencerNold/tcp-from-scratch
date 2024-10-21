#include <stdio.h>
#if defined(__APPLE__) || defined(__MACH__) || defined(__FreeBSD__)

#include "rawsocket.h"

#include <pcap.h>

void (*packet_handler0)(void*, const unsigned char*, int) = NULL;

void packet_handler(u_char* user_data, const struct pcap_pkthdr* header, const u_char* packet) {
    if (packet_handler0 != NULL) {
        packet_handler0((pcap_t*) user_data, (const unsigned char*) packet, header->caplen);
    }
}

void* rawsock_open() {
    char errbuf[PCAP_ERRBUF_SIZE];
    // TODO research how safe it is to use en0 here
    pcap_t* handle = pcap_open_live("en0", 1500 /* general mtu size */, 1, 1000, errbuf);
    if (handle == NULL) {
        return NULL;
    }
    return handle;
}

void rawsock_send(void* handle, unsigned char* data, int length) {
    pcap_sendpacket((pcap_t*) handle, (u_char*) data, length);
}

void rawsock_listen(void* handle, void (*handler)(void*, const unsigned char*, int)) {
    packet_handler0 = handler;
    pcap_loop(handle, -1, packet_handler, (u_char*) handle);
}

void rawsock_break(void* handle) {
    pcap_breakloop((pcap_t*) handle);
}

void rawsock_close(void* handle) {
    pcap_close((pcap_t*) handle);
}

#endif