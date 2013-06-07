//
//  NodeObject.cpp
//  NetSniffer
//
//  Created by Andrew on 2/27/13.
//  Copyright (c) 2013 Andrew. All rights reserved.
//

#include "GraphNode.h"

GraphNode::GraphNode()
{
    setMass(1);
    setKey("No Key");
    setDescription("");
    setX(0);
    setY(0);
    setVelX(0);
    setVelY(0);
    setDrag(0.05f);
    setTimeSinceAccess( 0 );
}

std::string GraphNode::getKey () { return _key; }
std::string GraphNode::getDescription() { return _description; }
float GraphNode::getMass() { return _mass; }
float GraphNode::getX() { return _x; }
float GraphNode::getY() { return _y; }
float GraphNode::getVelX() { return _velX; }
float GraphNode::getVelY() { return _velY; }
float GraphNode::getDrag() { return _drag; }
float GraphNode::getTimeSinceAccess() { return _timeSinceAccess; }

std::string GraphNode::setKey ( std::string key ) { return _key=key; }
std::string GraphNode::setDescription( std::string desc ) { return _description=desc; }
float GraphNode::setMass( float mass ) { return _mass=mass; }
float GraphNode::setX( float x ) { return _x=x; }
float GraphNode::setY( float y ) { return _y=y; }
float GraphNode::setVelX( float x ) { return _velX=x; }
float GraphNode::setVelY( float y ) { return _velY=y; }
float GraphNode::setDrag( float d ) { return _drag=d; }
float GraphNode::setTimeSinceAccess( float t ) { return _timeSinceAccess=t; };

void GraphNode::addImpulse( float x, float y ) {
    _velX += x / _mass;
    _velY += y / _mass;
}

void GraphNode::updateNode ()
{
    _x += _velX;
    _y += _velY;
    
    _velX *= (1.0f - _drag);
    _velY *= (1.0f - _drag);
    
    _timeSinceAccess ++;
}

void GraphNode::drawNode ( float radius, int segments, float scale )
{
    glBegin(GL_LINES);
    glColor3f( 0.0f, 1.1f - (getTimeSinceAccess()/NODE_LIFESPAN), 0.f );
    
    float nodeRadius = getMass() * radius * scale;
    float delta = 6.2831f / segments;
    for ( float theta = 0; theta < 360.0f; theta += delta ) {
        float xA = getX()+ cosf(theta) * nodeRadius;
        float yA = getY()+ sinf(theta) * nodeRadius;
        float xB = getX()+ cosf(theta+delta) * nodeRadius;
        float yB = getY()+ sinf(theta+delta) * nodeRadius;
        
        glVertex2f( xA, yA );
        glVertex2f( xB, yB );
    }
    glEnd();
    
    // Draw the node label
    glRasterPos2d( getX() + getMass() * radius * scale + 4, getY() - 4 );
    char textBuffer[48];
    sprintf(textBuffer, getKey().c_str() );
    for ( int i = 0; i < strlen((char*)textBuffer); i ++ )
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, textBuffer[i] );
}