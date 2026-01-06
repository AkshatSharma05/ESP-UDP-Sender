#pragma once   

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

void initSocket( int* upd_sock, struct sockaddr_in* addr,  in_addr_t recv_addr );

void sendPacket(int* udp_sock, const uint8_t* message, size_t len, struct sockaddr_in* addr);