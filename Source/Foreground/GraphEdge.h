//
//  GraphEdge.h
//  NetSniffer
//
//  Created by Andrew on 2/27/13.
//  Copyright (c) 2013 Andrew. All rights reserved.
//

#ifndef __NetSniffer__GraphEdge__
#define __NetSniffer__GraphEdge__

#include <iostream>
#include <math.h>
#include "GraphNode.h"

class GraphEdge {
    GraphNode* _from;
    GraphNode* _to;
    float _springForce;
    
public:
    GraphEdge( float springForce );
    
    GraphNode* getFrom();
    GraphNode* getTo();
    
    GraphNode* setFrom( GraphNode* from );
    GraphNode* setTo( GraphNode* to );
    
    float getSpringForce();
    float setSpringForce( float force );
        
    void updateEdge ();
    void drawEdge ( int segments, float sendTime );
};

#endif /* defined(__NetSniffer__GraphEdge__) */
