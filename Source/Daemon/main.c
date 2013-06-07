//
//  main.c
//  NetSniffer
//
//  Created by Andrew on 2/6/13.
//  Copyright (c) 2013  Andrew. All rights reserved.
//

// Take a look at this.
// http://www.tcpdump.org/pcap.html

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pcap.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/ip.h>

#include "capture.h"
#include "parsePacket.h"
#include "socketInterface.h"


void applyFilterToCapture ( char* device, pcap_t* handle, char* filter );
void cleanExit ();

int main(int argc, const char * argv[])
{
    signal(SIGTERM, cleanExit);
	signal(SIGINT, cleanExit);
    
    // initialize the sockets
    initialize_server_socket();
    
    // begin the capture
    pcap_t* captureHandle = initialize_capture_instance();
    
    // Pull the filter information from the first command line arguement
    char* filter = "port 80";
    if ( argc > 1 )
        filter = (char*)argv[1];
    
    apply_capture_filter( captureHandle, filter );//"port 80" );
    
    begin_capture( captureHandle, handlePacket );
    end_capture( captureHandle );
    
    cleanExit();
}

void cleanExit ()
{
    close_sockets();
    exit(0);
}