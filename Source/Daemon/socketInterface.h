//
//  socketInterface.h
//  NetSniffer_daemon
//
//  Created by Andrew on 3/1/13.
//  Copyright (c) 2013 Andrew. All rights reserved.
//

#ifndef NetSniffer_daemon_socketInterface_h
#define NetSniffer_daemon_socketInterface_h

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <string.h>
#include <errno.h>


#define SOCKET_FAMILY AF_UNIX
#define SOCKET_TYPE SOCK_STREAM
#define SOCKET_PROTOCOL 0
#define SOCKET_ADDRESS "/NetSniffer_Socket"
#define SOCKET_MAX_CONNECTION_QUEUE 5
#define SOCKET_BUFFER_SIZE 512

void initialize_server_socket ();
void initialize_client_socket ();
void write_to_socket ( char* data );
void read_from_socket ( char* outbuff );
void close_sockets ();
//int client_is_connected ();

#endif
