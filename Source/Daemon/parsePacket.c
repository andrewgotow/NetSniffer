//
//  parsePacket.c
//  NetSniffer
//
//  Created by Andrew on 2/20/13.
//  Copyright (c) 2013 Andrew. All rights reserved.
//

#include <stdio.h>
#include "parsePacket.h"
#include "socketInterface.h"

//http://yuba.stanford.edu/~casado/pcap/section4.html
void handlePacket (u_char *args, const struct pcap_pkthdr *header, const u_char *packet )
{
    const struct ip* ipInfo;
    
    // Jump past the eithernet header
    ipInfo = (struct ip*)(packet + sizeof(struct ether_header));
    u_int receivedLength = (int)(header->len - sizeof(struct ether_header));
    // now check to see if the packet is the right length.
    if ( receivedLength < sizeof(struct ip))
        printf("Packet Shortread\n");
    
    int ipLength = ntohs(ipInfo->ip_len);
    u_int ipHLength = ipInfo->ip_hl;
    u_int ipVersion = ipInfo->ip_v;
    
    if( ipHLength < 5 ) {
        printf("IP packet header length was invalid, expecting 5, got %d\n", ipHLength);
        return;
    }
    
    if ( ipVersion != 4 ) {
        printf("IP packet version %u found, expecting v4\n", ipVersion);
        return;
    }
    
    if ( receivedLength < ipLength ) {
        printf( "Shortread, was expecting %d, but found %d\n", ipLength, receivedLength );
        return;
    }
    
    // Get the time
    time_t timeObj;
    time(&timeObj);
    
    //printf("Pulled Packet: %d\n", (int)timeObj);
    //printf("    SOURCE ADDRESS: %s\n", inet_ntoa(ipInfo->ip_src) );
    //printf("    DESTIN ADDRESS: %s\n", inet_ntoa(ipInfo->ip_dst) );
   
    // Socket interface used here.
    char sendData[128];
    memset(sendData, 0, sizeof(sendData));
    // Beginning Token
    sendData[0] = '[';
    // the first is the source
    strcpy( sendData+strlen(sendData), inet_ntoa(ipInfo->ip_src) );
    // then delimiter
    sendData[strlen(sendData)] = '|';
    // then destination.
    strcpy( sendData+strlen(sendData), inet_ntoa(ipInfo->ip_dst) );
    // then end token
    sendData[ strlen(sendData) ] = ']';
    
    //printf( "WROTE DATA TO SOCKET : %s\n", sendData );
    //if ( client_is_connected() )
    write_to_socket( sendData );
    //else
    //   printf("WARNING, CLIENT CONNECTION NOT FOUND!\n");
}