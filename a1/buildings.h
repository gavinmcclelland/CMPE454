// Types of building


#ifndef BUILDING_H
#define BUILDING_H


#include "headers.h"
#include "gpuProgram.h"
#include <cmath>


class Building {
 public:

  // Constructors

  Building() {}

  Building( vec3 p ) {
    pos = p;
  }

  // Draw the building

  virtual void draw( GPUProgram *gpuProgram ) = 0;

  bool isHit(vec3 missilePos, float radius) {
    return (missilePos - pos).length() <= radius;
  }

 protected:

  vec3 pos;			// centre
};



#endif
