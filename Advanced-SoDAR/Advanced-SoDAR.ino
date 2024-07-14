

// Coords maintain the current aproximate position of the system based on acceleration data and calculating displacement
int xCoord = 0;
int yCoord = 0;
int zCoord = 0;

// The angle of the system with respect to its starting position and the ground
int roll; //Calc initial values in setup
int pitch;
int yaw;

// Approximate velocity vector of the system
int xVel = 0;
int yVel = 0;
int zVel = 0;

int lastMeasure = 0;



void updateAll() {
  updatePosition();
  updateAngles();
  updateVelocities();
  lastMeasure = millis();
}

void updatePosition() {
  xCoord += calcDisplacement(accel.acceleration.x);
  yCoord += calcDisplacement(accel.acceleration.y);
  zCoord += calcDisplacement(accel.acceleration.z);
}

void updateAngles() {
  roll += calcAngVelocity(gyro.gyro.x);
  pitch += calcAngVelocity(gyro.gyro.y);
  yaw += calcAngVelocity(gyro.gyro.z);
}

void updateVelocities() {
  xVel += calcVelocity(accel.acceleration.x, xVel);
  yVel += calcVelocity(accel.acceleration.y, yVel);
  zVel += calcVelocity(accel.acceleration.z, zVel);
}

int calcVelocity(int acceleration) {
  // acceleration * elapsed time = velocity
  int elapsedTime = millis() - lastMeasure;
  return (acceleration * elapsedTime);
}

int calcAngVelocity(int gyro) {
  // gyro * elapsed time = angular velocity
  int elapsedTime = millis() - lastMeasure;
  return (gyro * elapsedTime);
}

int calcDisplacement(int acceleration, int vel) {
  // ( initial velocity * elapsed time ) PLUS ( 1/2 * acceleration * elapsed time^2 ) = displacement
  int elapsedTime = millis() - lastMeasure;
  return ( (vel*elapsedTime) + (0.5*acceleration*pow(elapsedTime, 2)) );
}
