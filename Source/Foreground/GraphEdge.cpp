//
//  GraphEdge.cpp
//  NetSniffer
//
//  Created by Andrew on 2/27/13.
//  Copyright (c) 2013 Andrew. All rights reserved.
//

#include "GraphEdge.h"

GraphEdge::GraphEdge( float springForce )
{
    setSpringForce( springForce );
}

GraphNode* GraphEdge::getFrom() { return _from; }
GraphNode* GraphEdge::getTo() { return _to; }

GraphNode* GraphEdge::setFrom( GraphNode* from ) { return _from=from; }
GraphNode* GraphEdge::setTo( GraphNode* to ) { return _to=to; }

float GraphEdge::getSpringForce() { return _springForce; }
float GraphEdge::setSpringForce( float force ) { return _springForce=force; }

void GraphEdge::updateEdge ()
{
    float xdiff = _to->getX() - _from->getX();
    float ydiff = _to->getY() - _from->getY();
    float dist = sqrtf( xdiff*xdiff + ydiff*ydiff );
    float stretch = dist;
    
    float angle = atan2f(ydiff, xdiff);
    float forceX = cosf( angle ) * stretch * _springForce;
    float forceY = sinf( angle ) * stretch * _springForce;
    
    _to->addImpulse(-forceX, -forceY);
    _from->addImpulse(forceX, forceY);
}

void GraphEdge::drawEdge ( int segments, float sendTime )
{
    glBegin(GL_LINES);
    
    float fromEndFade = 1.0f - (getFrom()->getTimeSinceAccess()/NODE_LIFESPAN);
    float toEndFade = 1.0f - (getTo()->getTimeSinceAccess()/NODE_LIFESPAN);
    float minEndFade = fromEndFade;
    if ( toEndFade < minEndFade )
        minEndFade = toEndFade;
    
    
    for ( int i = 0; i < segments; i ++ ) {
        float val = (float)i / (float)segments;
        float nextVal = (float)(i+1) / (float)segments;
        
        float sinVal = sinf( sendTime + val*3.14 );
        float color = fmax( sinVal * sinVal * sinVal, 0.0f )*0.5f + 0.5f;//(1.0f - fabs(sinf( sendTime + val ))) + 0.5f;
        color *= minEndFade;
        
        float Ax = getFrom()->getX() * val + getTo()->getX() * (1.0f-val);
        float Ay = getFrom()->getY() * val + getTo()->getY() * (1.0f-val);
        
        float Bx = getFrom()->getX() * nextVal + getTo()->getX() * (1.0f-nextVal);
        float By = getFrom()->getY() * nextVal + getTo()->getY() * (1.0f-nextVal);
        
        glColor3f( color, color, color );
        glVertex2f( Ax, Ay );
        glVertex2f( Bx, By );
        //glVertex2f( getFrom()->getX(), getFrom()->getY() );
        //glVertex2f( getTo()->getX(), getTo()->getY() );
    }
    
    glEnd();
}