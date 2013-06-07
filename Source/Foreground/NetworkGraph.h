//
//  NetworkGraph.h
//  NetSniffer
//
//  Created by Andrew on 2/27/13.
//  Copyright (c) 2013 Andrew. All rights reserved.
//

#ifndef NetSniffer_NetworkGraph_h
#define NetSniffer_NetworkGraph_h

#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

#include <stdio.h>
#include <string.h>
#include <vector>
#include "GraphNode.h"
#include "GraphEdge.h"


class NetworkGraph {
    std::vector<GraphNode*> _nodes;
    std::vector<GraphEdge*> _edges;
    float _elapsedTime;
    
public:
    NetworkGraph();
    GraphNode* getNodeWithKey( std::string key, bool createIfNull = false );
    GraphEdge* getEdgeWithNodes ( GraphNode* from, GraphNode* to,  bool createIfNull = false );
    int getConnectionCount ( GraphNode* to );
    void makeConnection ( std::string key1, std::string key2 );
    
    void deleteNode( int index );
    void deleteEdge( int index );
    
    void updateNetworkGraph ();
    void drawNetworkGraph ();
};

#endif
