
// state.cpp


#include "headers.h"

#include "state.h"


// Draw each of the objects in the world


void State::draw() 

{
  int i;

  gpuProgram->activate();

  glUniformMatrix4fv( glGetUniformLocation( gpuProgram->id(), "M"), 1, GL_TRUE, &M[0][0] );

  silos[0].draw( gpuProgram );
  
  for (i=0; i<silos.size(); i++)
    silos[i].draw( gpuProgram );

  for (i=0; i<cities.size(); i++)
    cities[i].draw( gpuProgram );

  for (i=0; i<missilesIn.size(); i++)
    missilesIn[i].draw( gpuProgram );

  for (i=0; i<missilesOut.size(); i++)
    missilesOut[i].draw( gpuProgram );

  for (i=0; i<explosions.size(); i++)
    explosions[i].draw( gpuProgram );

  gpuProgram->deactivate();
}


// Update the state of the world after some time interval, deltaT
//
// CHANGE ALL OF THIS FUNCTION


bool State::updateState( float deltaT )

{
  int i;
  bool dead = false;

  // Update the time

  currentTime += deltaT;

  // Generate some new missiles.  The rate of missile generation
  // should increase with time.
  //
  // CHANGE THIS
  // std::cout << "Time: " << currentTime << std::endl;
  // Part 4a: Increase number of incoming missiles with time
  if (randIn01()+currentTime/1000 > 0.99) {	// New missile 

    missilesIn.add( Missile( vec3( randIn01(), worldTop, 0), // source
			     vec3( randIn01()*0.2 - 0.1, -0.1, 0 ),         // velocity
			     0,                              // destination y
			     vec3( 1,0.5,0) ) );              // colour
  }

  // Look for terminating missiles
  // Part 4b: Add explosions
  for (i=0; i<missilesIn.size(); i++)
    if (missilesIn[i].hasReachedDestination()) {
      // CHANGE THIS: ADD AN EXPLOSION
      // light red explosions indicate enemy missiles
      explosions.add(Circle(missilesIn[i].position(), 0.07, 0.05, vec3(1,0.5,0.5)));
      missilesIn.remove(i);
      i--;
    }

  for (i=0; i<missilesOut.size(); i++)
    if (missilesOut[i].hasReachedDestination()) {
      // CHANGE THIS: ADD AN EXPLOSION
      // Light blue explosions indicate friendly explosions
      explosions.add(Circle(missilesOut[i].position(), 0.07, 0.05, vec3(0.5,0.5,1)));
      missilesOut.remove(i);
      i--;
    }

  // Look for terminating explosions

  for (i=0; i<explosions.size(); i++) {
    
    // CHANGE THIS: CHECK FOR DESTROYED CITY OR SILO
    
    // if a city is hit, remove it
    for(int city = 0; city < cities.size(); city++) {
      if(cities[city].isHit(explosions[i].position(),explosions[i].radius()))
        cities.remove(city);
    }

    // NOTE: AS SPECIFIED, REMOVE A SILO AT THAT INDEX
    // UN-COMMENT THE LINES BELOW TO SEE THE BEHAVIOUR AS SPECIFIED
    // FOR PART 4c
    // for(int silo = 0; silo < silos.size(); silo++) {
    //   if(silos[silo].isHit(explosions[i].position(), explosions[i].radius()))
    //     silos.remove(silo);
    // }

    // NOVEL FEATURE #1: "Destroy" a silo
    // This makes the silo that is hit become a smaller "wedge" and turn red to indicate damage
    for(int silo = 0; silo < silos.size(); silo++) {
      if(silos[silo].isHit(explosions[i].position(), explosions[i].radius()))
        silos[silo].destroy();
    }

    // Part 4d: Remove a missile if destroyed by an explosion
    for(int missIn=0; missIn < missilesIn.size(); missIn++){
      if((missilesIn[missIn].position() - explosions[i].position()).length() <= explosions[i].radius())
      { // added chain reaction explosions, missile collisions are bright red
        explosions.add(Circle(missilesIn[missIn].position(), 0.07, 0.05, vec3(1,0,0)));
        missilesIn.remove(missIn);
      }
    }
      if (explosions[i].radius() >= explosions[i].maxRadius()) {
      explosions.remove(i);
      i--;
    }
  }

  bool surviving = false;

  for(i=0;i<silos.size();i++){
    if(silos[i].isAlive())
      surviving = true;
  }
  if (cities.size() == 0 && !surviving){
    dead = true;
  }

  // Look for incoming missiles that hit an explosion and are
  // destroyed

  // ADD CODE HERE
  
  // Update all the moving objects

  for (i=0; i<missilesIn.size(); i++)
    missilesIn[i].move( deltaT );

  for (i=0; i<missilesOut.size(); i++)
    missilesOut[i].move( deltaT );

  for (i=0; i<explosions.size(); i++)
    explosions[i].expand( deltaT );

  return dead;
}


// Fire a missile

void State::fireMissile( int siloIndex, float x, float y )

{
  const float speed = 0.3;
    
  if (silos[siloIndex].canShoot()) {

    silos[siloIndex].decrMissiles();

    // CHANGE THIS
    // Part 3: Add an outgoing missile at location of mouse click
    missilesOut.add( Missile( silos[siloIndex].position(),           				  // source
			      speed * (vec3(x,y,0) - silos[siloIndex].position()).normalize(),  // velocity
			      y,		                     			                                	// destination y
			      vec3( 0,0.5,1 ) ) );                     		                    	// colour
 }
}



// Create the initial state of the world


void State::setupWorld()

{
  // Keep track of the time

  currentTime = 0;

  timeOfIncomingFlight = 6;	// 6 seconds for incoming missile to arrive

  // Seed the random number generator

#ifdef _WIN32
  srand( 12345 );
#else
  struct timeb t;
  ftime( &t );
  srand( t.time );
#endif

  // Silos are added in order left, middle, right so that they have
  // indices 0, 1, and 2 (respectively) in the "silos" array.

  silos.add(  Silo( vec3( 0.1, 0, 0 ) ) );
  silos.add(  Silo( vec3( 0.5, 0, 0 ) ) );
  silos.add(  Silo( vec3( 0.9, 0, 0 ) ) );

  // Cities on the left

  cities.add( City( vec3( 0.2, 0, 0 ) ) );
  cities.add( City( vec3( 0.3, 0, 0 ) ) );
  cities.add( City( vec3( 0.4, 0, 0 ) ) );

  // Cities on the right

  cities.add( City( vec3( 0.6, 0, 0 ) ) );
  cities.add( City( vec3( 0.7, 0, 0 ) ) );
  cities.add( City( vec3( 0.8, 0, 0 ) ) );
}
