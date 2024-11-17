#include <Wire.h>
#include <Adafruit_TCS34725.h>

// Create an instance of the TCS34725 sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// Thresholds for interrupt
uint16_t thresholdLow = 100;
uint16_t thresholdHigh = 1000;

void setup() {
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Found TCS34725 sensor");
  } else {
    Serial.println("No TCS34725 found, check connections");
    while (1); // halt program if sensor is not found
  }

  // Enable interrupts on the TCS34725
  tcs.setInterrupt(false); // Disable interrupt initially

  // Set interrupt thresholds
  //setInterruptThresholds(thresholdLow, thresholdHigh);

  // Enable the INT pin interrupt
  pinMode(2, INPUT);  // INT pin of TCS34725 connected to Arduino pin 2
  attachInterrupt(digitalPinToInterrupt(2), handleInterrupt, FALLING);
  tcs.setIntLimits(thresholdLow, thresholdHigh);

  tcs.setInterrupt(true); // Enable interrupt
}

void loop() {
  // Normal sensor operation
  uint16_t red, green, blue, clear;
  tcs.getRawData(&red, &green, &blue, &clear);


  // Print sensor data to serial
  printSensorData(red, green, blue, clear);

  delay(1000); // Delay for readability
}

// Interrupt handler when the INT pin goes LOW
void handleInterrupt() {
  Serial.println("Interrupt triggered: Sensor values exceeded thresholds!");

  // Clear the interrupt on the sensor
  tcs.clearInterrupt();
}

// Function to set the interrupt thresholds for the sensor
void setInterruptThresholds(uint16_t low, uint16_t high) {
  tcs.write8(TCS34725_PERS, TCS34725_PERS_NONE);  // Set persistence filter to trigger on every RGBC cycle
  tcs.setIntLimits(low, high);  // Set the low and high threshold for clear data
}

// Function to print all sensor data
void printSensorData(uint16_t red, uint16_t green, uint16_t blue, uint16_t clear) {
  Serial.print("Red: "); Serial.print(red);
  Serial.print("\tGreen: "); Serial.print(green);
  Serial.print("\tBlue: "); Serial.print(blue);
  Serial.print("\tClear: "); Serial.println(clear);
}
