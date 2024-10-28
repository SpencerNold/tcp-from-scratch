#include "rawsocket.h"
#include "network.h"
#include "tcp.h"
#include "thread.h"

#include <stdio.h>
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