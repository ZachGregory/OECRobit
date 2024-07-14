

// Coords maintain the current aproximate position of the system based on acceleration data and calculating displacement
int xCoord;
int yCoord;
int zCoord;

// The angle of the system with respect to its starting position and the ground
int roll;
int pitch;
int yaw;

// Approximate velocity vector of the system
int xVel;
int yVel;
int zVel;

void updateAll(){
  updatePosition();
  updateAngles();
  updateVelocities();
}

void updatePosition(){

}

void updateAngles(){

}

void updateVelocities(){

}

int calcVelocity(){
  // Average acceleration * elapsed time = velocity

}

int calcDisplacement(){
  // ( initial velocity * elapsed time ) PLUS ( 1/2 * average acceleration * elapsed time^2 ) = displacement

}

