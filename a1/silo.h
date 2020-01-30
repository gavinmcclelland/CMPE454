// A Silo


#ifndef SILO_H
#define SILO_H

#include "headers.h"
#include "gpuProgram.h"
#include "buildings.h"


class Silo : public Building {
 public:

  // Constructors 

  Silo() {}

  Silo( vec3 pos ) : Building( pos ) {
    alive = true;
    roundsLeft = 15;
  }
  void destroy(){
    alive = false;
    roundsLeft = 0;
  }
  bool canShoot() {
    return (roundsLeft > 0);
  }

  bool isAlive(){
    return alive;
  }

  void decrMissiles() {
    roundsLeft--;
  }

  vec3 position() {
    return pos;
  }

  // Draw the silo

  void draw( GPUProgram *gpuProgram ) {

    const int NUM_SEGMENTS = 45; // number of pieces of hemisphere perimeter

    vec3 *verts = new vec3[NUM_SEGMENTS+1];

    verts[0] = pos;
    
    if (alive){
      for (int i=0; i<NUM_SEGMENTS; i++) {
        float theta = (i/(float)(NUM_SEGMENTS-1)) * M_PI;
        verts[i+1] = vec3( pos.x + 0.04 * cos(theta), pos.y + 0.04 * sin(theta), 0 );
      }
      gpuProgram->drawVertices( verts, NUM_SEGMENTS+1, GL_TRIANGLE_FAN, vec3(1,1,1) );
    }else{
      for (int i=0; i<NUM_SEGMENTS; i++) {
        float theta = (i/(float)(NUM_SEGMENTS*3-1)) * M_PI;
        verts[i+1] = vec3( pos.x + 0.04 * cos(theta), pos.y + 0.04 * sin(theta), 0 );
      }
      gpuProgram->drawVertices( verts, NUM_SEGMENTS+1, GL_TRIANGLE_FAN, vec3(1,0,1) );
    }
    

    delete [] verts;
  }

 private:
  bool alive;
  int roundsLeft;
};
  

#endif
