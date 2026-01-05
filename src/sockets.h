#pragma once   
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

#define FREQ_MS 500

void initSocket( int* upd_sock, struct sockaddr_in* addr,  in_addr_t recv_addr );

void sendPacket(int* udp_sock, const char* message, struct sockaddr_in* addr);