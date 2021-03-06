//
//  socketInterface.cpp
//  NetSniffer_daemon
//
//  Created by Andrew on 3/1/13.
//  Copyright (c) 2013 Andrew. All rights rethised.
//

#include "socketInterface.h"

char local_buffer[SOCKET_BUFFER_SIZE];
int local_fd = 0; // The socket pointer you'll use
int remote_fd = 0;
struct sockaddr_un local_addr; // The thiser's address
struct sockaddr_un remote_addr; // The otherent's address


void initialize_server_socket()
{
    // clear socket address.
    memset( &local_addr, 0, sizeof(local_addr) );
    
    // fill address structure with
    local_addr.sun_family = SOCKET_FAMILY;
    strcpy( local_addr.sun_path, SOCKET_ADDRESS );
    int addr_len = sizeof(local_addr.sun_path) + sizeof(local_addr.sun_family);
    
    // if the socket already exists, call unlink. This is ignored if it doesn't exist.
    unlink( local_addr.sun_path );
    printf("ERROR: socket unlink failed with code: %d\n",errno);
    
    local_fd = socket( SOCKET_FAMILY, SOCKET_TYPE, SOCKET_PROTOCOL );
    printf("ERROR: socket initialization failed with code: %d\n",errno);
    
    bind(local_fd, (struct sockaddr*)&local_addr, addr_len );
    printf("ERROR: socket bind failed with code: %d\n",errno);
    listen( local_fd, 5 );
    printf("ERROR: socket listen failed with code: %d\n",errno);
    
    // Accept others
    unsigned int remote_addr_len = sizeof(struct sockaddr_un);
    remote_fd = accept( local_fd, (struct sockaddr*)&remote_addr, &remote_addr_len);
    
    //printf("err %d\n",errno);
}

void initialize_client_socket()
{
    printf("errno: %d\n", errno);
    memset( &remote_addr, 0, sizeof(remote_addr) );
    
    remote_addr.sun_family = SOCKET_FAMILY;
    strcpy( remote_addr.sun_path, SOCKET_ADDRESS );
    int addr_len = sizeof(remote_addr.sun_path) + sizeof(remote_addr.sun_family);
    
    local_fd = socket( SOCKET_FAMILY, SOCKET_TYPE, SOCKET_PROTOCOL );
    printf("ERROR: socket initialization failed with code: %d\n",errno);
    
    // try connecting until we succeed.
    int connectionStatus = -1;
    //while ( connectionStatus != 0 ) {
    connectionStatus = connect( local_fd, (struct sockaddr*)&remote_addr, addr_len);
    printf("Attempting Connection, ERRNO: %d\n",errno);
    //}
    //printf("CLIENT SOCKET CONNECTION ESTABLISHED\n");
}

void write_to_socket ( char* data )
{
    int count = (int)send( remote_fd, data, strlen(data), 0);
    if ( count <= 0 )
        printf( "ERROR: Could not write to socket.\n" );
}

void read_from_socket ( char* outbuff )
{
    //if (
    recv( local_fd, outbuff, SOCKET_BUFFER_SIZE, 0 );// > 0 )
        //printf( "MESSAGE FROM OTHER: %s\n", outbuff );
}

void close_sockets ()
{
    if ( local_fd != 0 )
        close(local_fd);
    if ( remote_fd != 0 )
        close(remote_fd);
}

/*
int client_is_connected ()
{
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    
    fd_set socketFDs;
    FD_ZERO(&socketFDs);
    FD_SET( remote_fd, &socketFDs );
    
    int status = select(1,&socketFDs, NULL, NULL, &timeout);
    
    if ( status >= 0 )
        return 1;
    else
        return 0;
}
*/