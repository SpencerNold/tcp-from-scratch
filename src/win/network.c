#include "os.h"
#ifdef WIN_OS

#include "network.h"

#include "endian.h"

#include <winsock2.h>
#include <iphlpapi.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

uint8_t stored_src_mac_address[6];
uint32_t stored_src_ipv4_address = 0;
uint32_t stored_default_gateway = 0;
int is_src_mac_stored = 0, is_src_ipv4_stored = 0, is_default_gateway_stored = 0;
uint32_t stored_iface_index;

int get_best_interface(uint32_t dst_addr, void* dst_iface) {
    DWORD index;
    DWORD result = GetBestInterface(dst_addr, &index);
    if (result != NO_ERROR) {
        return result;
    }
    *((uint32_t*) dst_iface) = index;
    return 0;
}

void store_inet_addrs(const void* iface) {
    DWORD size = 0;
    DWORD value = GetAdaptersInfo(NULL, &size);
    if (value != ERROR_BUFFER_OVERFLOW) {
        return;
    }
    IP_ADAPTER_INFO* adapter_info = (IP_ADAPTER_INFO*) malloc(size);
    if (adapter_info == NULL) {
        return;
    }
    value = GetAdaptersInfo(adapter_info, &size);
    IP_ADAPTER_INFO* next;
    if (value == NO_ERROR) {
        for (next = adapter_info; next != NULL; next = next->Next) {
            if (next->Index == *((uint32_t*) iface)) {
                stored_iface_index = next->Index;
                mac_cpy(stored_src_mac_address, next->Address);
                is_src_mac_stored = 1;

                const char* ipv4_str = next->IpAddressList.IpAddress.String;
                stored_src_ipv4_address = inet_addr(ipv4_str);
                is_src_ipv4_stored = 1;

                const char* default_gateway_str = next->GatewayList.IpAddress.String;
                if (default_gateway_str[0] != 0) {
                    stored_default_gateway = inet_addr(default_gateway_str);
                    is_default_gateway_stored = 1;
                }
            }
        }
    }
    free(adapter_info);
}

int net_get_src_mac(const void* iface, uint8_t* mac) {
    if (!is_src_mac_stored || *((uint32_t*) iface) != stored_iface_index) {
        store_inet_addrs(iface);
    }
    mac_cpy(mac, stored_src_mac_address);
    return 0;
}

uint32_t net_get_src_addr(const void* iface) {
    if (!is_src_ipv4_stored || *((uint32_t*) iface) != stored_iface_index) {
        store_inet_addrs(iface);
    }
    return stored_src_ipv4_address;
}

uint32_t net_get_default_gateway(const void* iface) {
    if (!is_default_gateway_stored || *((uint32_t*) iface) != stored_iface_index) {
        store_inet_addrs(iface);
    }
    return stored_default_gateway;
}

int sys_arp_table_lookup(uint32_t address, uint8_t* mac) {
    ULONG dst_addr[2];
    ULONG dst_addr_len = 6;
    memset(dst_addr, 0xff, sizeof(dst_addr));
    DWORD result = SendARP(address, 0, dst_addr, &dst_addr_len);
    if (result != NO_ERROR) {
        return result;
    }
    mac_cpy(mac, (uint8_t*) dst_addr);
    return 0;
}

uint16_t sys_htons(uint16_t x) {
    return htons(x);
}

uint32_t sys_htonl(uint32_t x) {
    return htonl(x);
}

uint32_t sys_inet_addr(const char* addr) {
    return inet_addr(addr);
}

char* sys_inet_ntoa(uint32_t addr) {
    struct in_addr in_addr;
    in_addr.s_addr = addr;
    return inet_ntoa(in_addr);
}

#endif