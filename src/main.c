#include "rawsocket.h"
#include "network.h"
#include "tcp.h"

#include <stdio.h>

int main() {
    const char* interface = "en0";
    
    struct network_data addrs;
    net_load_addrs(interface, &addrs);
    addrs.dst_addr = sys_inet_addr("127.0.0.1");

    void* handle = rawsock_open(interface);
    if (handle == NULL) {
        return -1;
    }

    tcp_send_syn(handle, &addrs, 25565, 80);

    rawsock_close(handle);
}