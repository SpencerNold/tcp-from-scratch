#ifndef RAWSOCK_H
#define RAWSOCK_H

void* rawsock_open(const char*);
void rawsock_send(void*, unsigned char*, int);
void rawsock_listen_raw(void*, void (*)(void*, const unsigned char*, int));
void rawsock_listen_ethernet(void*, void (*)(void*, const unsigned char*, int));
void rawsock_listen_ipv4(void*, void (*)(void*, const unsigned char*, int));
void rawsock_listen_arp(void*, void (*)(void*, const unsigned char*, int));
void rawsock_listen_tcp(void*, void (*)(void*, const unsigned char*, int));
void rawsock_break(void*);
void rawsock_close(void*);

#endif