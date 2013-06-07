//
//  main.cpp
//  NetSniffer
//
//  Created by Andrew on 2/27/13.
//  Copyright (c) 2013 Andrew. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include "NetworkGraph.h"
#include "socketInterface.h"
#include <pthread.h>
#include <sys/wait.h>

void initView ( char* viewTitle, bool fullscreen );
void setupOpenGL ();
void redrawView ( void );
void cleanExit ();
void handleSocketRead ( );

NetworkGraph* NetGraph;

int main(int argc, const char * argv[])
{
    // Launch the child process.
    int myID = fork();
    if ( myID == 0 ) {
        char* daemonArgv[2];
        daemonArgv[0] = (char*)"NetSniffer_daemon";
        daemonArgv[1] = (char*)"";//port 80";
        execv( "NetSniffer_daemon", daemonArgv );
    }
    
    sleep( 1 );
    // Initialize Socket
    initialize_client_socket();
    
    // Initialize Socket thread. (THIS CAUSED MAJOR CRASHES, CONSIDER MUTEX LOCK ON NODE INFORMATION)
    //pthread_t socketReadThread;
    //pthread_create( &socketReadThread, NULL, handleSocketRead, NULL );
    
    // Initialize Program
    glutInit(&argc, (char**)argv);
    initView( (char*)"Primary Window", false );
    
    setupOpenGL();
    
    glutMainLoop();
    
    return 0;
}

void initView ( char* viewTitle, bool fullscreen )
{
    glutInitWindowSize( 1280,800 );
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow(viewTitle);
    
    glutDisplayFunc( redrawView );
    
    if ( fullscreen )
        glutFullScreen();
}

void setupOpenGL ()
{
    int winWidth = glutGet(GLUT_WINDOW_WIDTH);
    int winHeight = glutGet(GLUT_WINDOW_HEIGHT);
    
    glMatrixMode(GL_PROJECTION);
    glOrtho(-winWidth/2, winWidth/2, -winHeight/2, winHeight/2, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    
    // setup the network graph.
    NetGraph = new NetworkGraph();
}

void redrawView ( void )
{
    // every few frames, we should check the socket for new data. (NOW IS THREADED)
    handleSocketRead( );
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    NetGraph->updateNetworkGraph();
    NetGraph->drawNetworkGraph();
    glutSwapBuffers();
    glutPostRedisplay();
}

void handleSocketRead (  )
{
    // loop indefinitely, reading the socket data.
   // while( true ) {
        // read buffer data from the socket.
        char* buffer = read_from_socket();
        
        char packet[128]; // packet buffer.
        memset(packet, 0, sizeof(packet));
        int packetSize = -1;
                
        // Read through the entire buffer.
        for ( int c = 0; c < strlen(buffer); c ++ ) {
            // if the packet is empty, search for a '[' character
            if ( packetSize == -1 ) {
                if ( buffer[c] == '[' ) {
                    //packet[packetSize] = buffer[c];
                    packetSize ++; // start the packet read
                }
            }else if ( packetSize >= 0 ) {
                // if the packet is not empty, and we reach a ']' character, flush the packet.
                if ( buffer[c] == ']' ) {
                    char* srcString = strtok( packet, "|");
                    char* dstString = strtok( NULL, "|");
                    
                    if ( srcString != NULL && dstString != NULL )
                        NetGraph->makeConnection(std::string(srcString), std::string(dstString));
                    
                    // reset the packet
                    memset( packet, 0, sizeof(packet) );
                    packetSize = -1;
                }else{
                    // otherwise, just read.
                    packet[packetSize] = buffer[c];
                    packetSize ++;
                }
            }

        }
        
        // Wait some time before re-reading.
      //  usleep( 100000 ); // 0.1 secondsx
    //}
}

void cleanExit ()
{
    close_sockets();
    exit(0);
}