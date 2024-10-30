#include "os.h"
#ifdef BSD_OS

#include "rawsocket.h"

#include "network.h"
#include "ip.h"
#include <pcap.h>

// TODO When adding win/nux support, move a lot of the functionality into a joined uni source file
// TODO Also add support for binding a listening socket to filter only the wanted TCP ports
// TODO Also add options/flags for filtering
int listening = 0;

uint8_t src_mac[6];
uint32_t src_addr;

void (*raw_handler)(void*, const unsigned char*, int) = NULL;
void (*ethernet_handler)(void*, const unsigned char*, int) = NULL;
void (*ipv4_handler)(void*, const unsigned char*, int) = NULL;
void (*arp_handler)(void*, const unsigned char*, int) = NULL;
void (*tcp_handler)(void*, const unsigned char*, int) = NULL;

void packet_ipv4(pcap_t*, const unsigned char*, int);
void packet_arp(pcap_t*, const unsigned char*, int);
void packet_tcp(pcap_t*, const unsigned char*, int);
void packet_ethernet(pcap_t*, const unsigned char*, int);

void packet_ipv4(pcap_t* handle, const unsigned char* data, int length) {
    if (length < 20)
        return;
    if ((*((uint32_t*) (data + 16))) != src_addr) {
        return;
    }
    if (ipv4_handler != NULL) {
        ipv4_handler(handle, data, length);
    }
    size_t h_len = ip_get_ihl(data[0]) * 4;
    if (length < h_len)
        return;
    if (data[9] == 6) {
        packet_tcp(handle, data + h_len, length - h_len);
    }
}

void packet_arp(pcap_t* handle, const unsigned char* data, int length) {
    if (arp_handler != NULL) {
        arp_handler(handle, data, length);
    }
}

void packet_tcp(pcap_t* handle, const unsigned char* data, int length) {
    if (tcp_handler != NULL) {
        tcp_handler(handle, data, length);
    }
}

void packet_ethernet(pcap_t* handle, const unsigned char* data, int length) {
    if (length < 6)
        return;
    for (int i = 0; i < 6; i++) {
        if (data[i] != src_mac[i]) {
            return;
        }
    }
    if (ethernet_handler != NULL) {
        ethernet_handler(handle, data, length);
    }
    uint16_t type = sys_htons(((uint16_t*) (data + 12))[0]);
    size_t h_len = 14;
    if (type == 0x8100) {
        h_len = 16;
    }
    if (length < h_len)
        return;
    // TODO Maybe support more types in the future
    if (type == 0x0800) {
        packet_ipv4(handle, data + h_len, length - h_len);
    } else if (type == 0x0806) {
        packet_arp(handle, data + h_len, length - h_len);
    }
}

void packet_handler(u_char* user_data, const struct pcap_pkthdr* header, const u_char* packet) {
    pcap_t* handle = (pcap_t*) user_data;
    int length = header->caplen;
    if (raw_handler != NULL) {
        raw_handler(handle, (const unsigned char*) packet, length);
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

void rawsock_listen_tcp(void* handle, void (*handler)(void*, const unsigned char*, int)) {
    tcp_handler = handler;
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