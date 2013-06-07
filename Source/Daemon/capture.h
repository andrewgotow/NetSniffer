//
//  capture.h
//  NetSniffer
//
//  Created by Andrew on 2/20/13.
//  Copyright (c) 2013 Andrew. All rights reserved.
//

#ifndef NetSniffer_capture_h
#define NetSniffer_capture_h

#include "pcap.h"

#define CAPTURE_PROMISCUOUS 1
#define CAPTURE_MAX_PACKET_SIZE 1024
#define CAPTURE_PACKET_COUNT 0 // value of 0 or -1 will be treated as infinite
#define CAPTURE_TIMEOUT 1024
#define CAPTURE_OPTIMIZE_FILTER 0

#define CAPTURE_DEVICE_ALL NULL // doesn't work on older versions
#define CAPTURE_DEVICE_AIRPORT "en1"
#define CAPTURE_DEVICE_EITHERNET "en0"

#define CAPTURE_ACTIVE_DEVICE CAPTURE_DEVICE_AIRPORT

char capture_error_buffer[PCAP_ERRBUF_SIZE];

pcap_t* initialize_capture_instance ();
int begin_capture ( pcap_t* handle, pcap_handler callback );
int end_capture ( pcap_t* handle );
void apply_capture_filter ( pcap_t* handle, char* filter );

#endif
