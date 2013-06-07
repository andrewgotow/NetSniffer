//
//  NodeObject.h
//  NetSniffer
//
//  Created by Andrew on 2/27/13.
//  Copyright (c) 2013 Andrew. All rights reserved.
//

#ifndef __NetSniffer__GraphNode__
#define __NetSniffer__GraphNode__

#include <iostream>
#include <string.h>
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#include <math.h>

#define NODE_LIFESPAN 1000

class GraphNode {
    std::string _key;
    std::string _description;
    float _mass;
    float _x;
    float _y;
    float _velX;
    float _velY;
    float _drag;
    float _timeSinceAccess;
    
public:
    GraphNode();
    
    std::string getKey ();
    std::string getDescription ();
    float getMass();
    float getX();
    float getY();
    float getVelX();
    float getVelY();
    float getDrag();
    float getTimeSinceAccess();

    std::string setKey ( std::string key );
    std::string setDescription ( std::string desc );
    float setMass( float mass );
    float setX( float x );
    float setY( float y );
    float setVelX( float x );
    float setVelY( float y );
    float setDrag( float d );
    float setTimeSinceAccess( float t );
    
    void addImpulse( float x, float y );
    void driftToOrigin ();
    void updateNode ();
    void drawNode ( float radius, int segments, float scale );
};

#endif /* defined(__NetSniffer__GraphNode__) */
