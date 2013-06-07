//
//  parsePacket.h
//  NetSniffer
//
//  Created by Andrew on 2/20/13.
//  Copyright (c) 2013 Andrew. All rights reserved.
//

#ifndef NetSniffer_parsePacket_h
#define NetSniffer_parsePacket_h

#include <pcap.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/ip.h>

#include <netinet/in.h>
#include <netinet/if_ether.h>

void handlePacket (u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

#endif
