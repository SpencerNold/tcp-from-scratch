#ifndef RAWSOCK_H
#define RAWSOCK_H

void* rawsock_open();
void rawsock_send(void*, unsigned char*, int);
void rawsock_listen(void*, void (*)(void*, const unsigned char*, int));
void rawsock_break(void*);
void rawsock_close(void*);

#endif