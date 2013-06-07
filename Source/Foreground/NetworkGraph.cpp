//
//  NetworkGraph.cpp
//  NetSniffer
//
//  Created by Andrew on 2/27/13.
//  Copyright (c) 2013 Andrew. All rights reserved.
//

#include "NetworkGraph.h"

#define NODE_SIZE 20
#define NODE_CIRCLE_SEGMENTS 20
#define EDGE_PULSE_RATE 50
#define EDGE_LINE_SEGMENTS 5
#define NODE_ORIGIN_FORCE (1.0f/1000.0f)
#define NODE_REPULSION_FORCE 1000
#define EDGE_SPRING (1.0f/1000.0f)

NetworkGraph::NetworkGraph()
{
    _elapsedTime = 0;
}

GraphNode* NetworkGraph::getNodeWithKey( std::string key, bool createIfNull )
{
    // search through the node list.
    for ( int i = (int)_nodes.size()-1; i >= 0; i -- ) {
        if ( _nodes[i] != NULL ) {
            if ( strcmp(_nodes[i] ->getKey().c_str(), key.c_str() ) == 0 ) {
                _nodes[i] ->setTimeSinceAccess(0);
                return _nodes[i];
            }
        }
    }
    // if we haven't returned by now, we couldn't find the key.
    if ( createIfNull ) {
        GraphNode* newNode = new GraphNode();
        newNode->setKey(key);
        
        // Give the node a random location
        newNode->setX( rand()%500-250 );
        newNode->setY( rand()%500-250 );
        
        _nodes.push_back(newNode);
        
        return newNode;
    }
    
    return NULL;
}

GraphEdge* NetworkGraph::getEdgeWithNodes ( GraphNode* from, GraphNode* to, bool createIfNull )
{
    for ( int i = (int)_edges.size()-1; i >= 0; i -- ) {
        if ( _edges[i]->getFrom() == from && _edges[i]->getTo() == to )
            return _edges[i];
    }
    // if we haven't returned by now, we couldn't find the key.
    if ( createIfNull ) {
        GraphEdge* newEdge = new GraphEdge( EDGE_SPRING );
        newEdge->setFrom( from );
        newEdge->setTo( to );
        _edges.push_back(newEdge);
        
        return newEdge;
    }
    
    return NULL;
}

int NetworkGraph::getConnectionCount ( GraphNode* to )
{
    int count = 0;
    for ( int i = (int)_edges.size()-1; i >= 0; i -- ) {
        // Get incoming connections
        if ( _edges[i]->getTo() == to )
            count ++;
        // Get outgoing connections
        if ( _edges[i]->getFrom() == to )
            count ++;
    }
    return count;
}

void NetworkGraph::makeConnection ( std::string key1, std::string key2 )
{
    GraphNode* nodeA = getNodeWithKey( key1, true );
    GraphNode* nodeB = getNodeWithKey( key2, true );
    
    getEdgeWithNodes( nodeA, nodeB, true );
}

void NetworkGraph::deleteNode( int index )
{
    for ( int i = (int)_edges.size()-1; i >= 0; i -- ) {
        if ( _edges[i]->getTo() == _nodes[index] || _edges[i]->getFrom() == _nodes[index] ) {
            deleteEdge(i);
        }
    }
    
    GraphNode* toDelete = _nodes[index];
    _nodes.erase(_nodes.begin()+index);
    delete( toDelete ); // MASSIVE memory leak... but yeah... I don't really care at this point.
}

void NetworkGraph::deleteEdge( int index )
{
    GraphEdge* toDelete = _edges[index];
    _edges.erase(_edges.begin()+index);
    delete(toDelete);
}

void NetworkGraph::updateNetworkGraph ()
{
    for ( int i = (int)_nodes.size()-1; i >= 0; i -- ) {
        // repel each node from its neighbors.
        for ( int j = (int)_nodes.size()-1; j >= 0; j -- ) {
            if ( i != j ) {
                // calculate the square distance between the two nodes.
                float xdiff = _nodes[i]->getX() - _nodes[j]->getX();
                float ydiff = _nodes[i]->getY() - _nodes[j]->getY();
                float sqrDist = xdiff*xdiff + ydiff*ydiff;
                            
                // calculate an angle, and then calculate the force vector on the node due to the other.
                float angle = atan2f(ydiff, xdiff);
                float forceX = cosf( angle ) * (NODE_REPULSION_FORCE / sqrDist);
                float forceY = sinf( angle ) * (NODE_REPULSION_FORCE / sqrDist);
                
                _nodes[i]->addImpulse(forceX, forceY);
            }
        }
    
        // Pull towards the origin.
        float xdiff = _nodes[i]->getX();
        float ydiff = _nodes[i]->getY();
        float dist = sqrtf( xdiff*xdiff + ydiff*ydiff );
        
        float angle = atan2f(ydiff, xdiff);
        float forceX = cosf( angle ) * dist * NODE_ORIGIN_FORCE;
        float forceY = sinf( angle ) * dist * NODE_ORIGIN_FORCE;
        
        _nodes[i]->addImpulse(-forceX, -forceY);

        // Update nodes.
        _nodes[i]->updateNode();
        
        int connectionCount = getConnectionCount(_nodes[i]);
        _nodes[i]->setMass( connectionCount );
        
        if ( _nodes[i]->getTimeSinceAccess() > NODE_LIFESPAN ) {
            deleteNode(i);
            break;
        }
    }
    
    // Now update edges to act as attractors.
    for ( int i = (int)_edges.size()-1; i >= 0; i -- ) {
        _edges[i]->updateEdge();
    }
}

void NetworkGraph::drawNetworkGraph ()
{
    _elapsedTime += 1.0f / 30.0f;
    
    // Loop through all graph edges
    for ( std::vector<GraphEdge*>::iterator edge = _edges.begin(); edge != _edges.end(); edge ++ ) {
        (*edge)->drawEdge( EDGE_LINE_SEGMENTS, _elapsedTime );
    }
    
    // loop through all graph nodes
    for ( std::vector<GraphNode*>::iterator node = _nodes.begin(); node != _nodes.end(); node ++ ) {
        (*node)->drawNode( NODE_SIZE, NODE_CIRCLE_SEGMENTS, 1.0f/_nodes.size() );
    }
}