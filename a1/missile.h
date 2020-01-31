// Missile class


#ifndef MISSILE_H
#define MISSILE_H


#include "headers.h"
#include "gpuProgram.h"


class Missile {
 public:

  // Constructors

  Missile() {}

  Missile( vec3 p, vec3 v, float y, vec3 c ) {
    pos0 = p;
    pos1 = p;
    velocity = v;
    colour = c;
    destY = y;
  }

  // Draw the missile and its trail

  void draw( GPUProgram *gpuProgram) {
    vec3 verts[8] = {
        vec3( pos0.x, pos0.y, 0 ),
        vec3( pos1.x, pos1.y, 0 ),
        vec3( pos1.x + 0.015, pos1.y, 0 ),
        vec3( pos1.x - 0.015, pos1.y, 0 ),
        vec3( pos1.x - 0.020, pos1.y, 0 ),
        vec3( pos1.x, pos1.y - 0.03, 0 ),
        vec3( pos1.x + 0.020, pos1.y, 0 ),
        vec3( pos1.x, pos1.y - 0.03, 0 )
      };
    if (velocity[1] > 0)
    {
      verts[5] = vec3( pos1.x, pos1.y + 0.03, 0 );
      verts[7] = vec3( pos1.x, pos1.y + 0.03, 0 );
    }
    gpuProgram->drawVertices( &verts[0], 8, GL_LINES, colour );
  }

  // Move the missile over a time interval, deltaT

  void move( float deltaT ) {
    pos1 = pos1 + deltaT * velocity;
  }

  // Return the current position 

  vec3 position() {
    return pos1;
  }

  // Has the missile reached its destination?

  bool hasReachedDestination() { 
    // if the missile is moving upward, check if it is past destination
    if (velocity.y > 0){
  	return pos1.y >= destY;
    }
    else // in the case that the missile is moving downward
	return pos1.y < destY;
    }

 private:

  vec3  pos0;			// start position
  vec3  pos1;			// current position
  vec3  velocity;		// velocity
  vec3  colour;			// colour of missile trail
  float destY;			// y position at destination
};


#endif
