//
//  capture.c
//  NetSniffer
//
//  Created by Andrew on 2/20/13.
//  Copyright (c) 2013 Andrew. All rights reserved.
//

#include <stdio.h>
#include "capture.h"

pcap_t* initialize_capture_instance ()
{
    printf( "Capture instance established with device: %s\n", CAPTURE_ACTIVE_DEVICE );
    pcap_t* handle = pcap_open_live( CAPTURE_ACTIVE_DEVICE, CAPTURE_MAX_PACKET_SIZE, CAPTURE_PROMISCUOUS, CAPTURE_TIMEOUT, capture_error_buffer );
    
    if (handle == NULL)
        fprintf( stderr, "ERROR: Capture instance could not be initialized with device handle: %s. %s\n", CAPTURE_ACTIVE_DEVICE, (char*)capture_error_buffer );

    return handle;
}

int begin_capture ( pcap_t* handle, pcap_handler callback )
{
    printf( "Capture loop started\n" );
    pcap_loop( handle, CAPTURE_PACKET_COUNT, callback, NULL );
    return 0;
}

int end_capture ( pcap_t* handle )
{
    printf( "Capture loop finished\n" );
    pcap_breakloop( handle );
    pcap_close( handle );
    return 0;
}

void apply_capture_filter ( pcap_t* handle, char* filter )
{
    // Example capture filters here http://wiki.wireshark.org/CaptureFilters
    struct bpf_program filterProgram;
    
    // Netmask is ignored for now.
    if ( pcap_compile( handle, &filterProgram, filter, CAPTURE_OPTIMIZE_FILTER, PCAP_NETMASK_UNKNOWN ) )
        fprintf( stderr, "ERROR: Could not compile filter %s: \t%s\n", filter, pcap_geterr(handle) );
    if ( pcap_setfilter( handle, &filterProgram ) )
        fprintf( stderr, "ERROR: Could not set filter %s: \t%s\n", filter, pcap_geterr(handle) );
}