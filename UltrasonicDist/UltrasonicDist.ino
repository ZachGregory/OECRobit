// Standalone SR-04 Ultrasonic Distance Sensor Code
// Written by Zach Gregory

// Define pins
const int trigPin = 13;
const int echoPin = 14;
// Variables to store duration and distance
long duration;
float distance;

void setup() {
  // Set trigPin as OUTPUT and echoPin as INPUT
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize serial communication
  Serial.begin(115200);
}

void loop() {
  pulse();

  // Small delay to prevent reading too fast
  delay(50);
}

float pulse(){
  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Trigger the sensor by setting the trigPin high for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin and calculate the pulse duration in microseconds
  duration = pulseIn(echoPin, HIGH, 30000); // Timeout of 30ms (30000us)

  // Calculate distance in centimeters (speed of sound = 343 m/s)
  distance = duration * 0.034 / 2; // 0.034 cm/us is the speed of sound in air
  
  // Check for out-of-range readings
  if (duration == 0) {
    Serial.println("Out of range");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    return distance;
  }
  return 0;
}
