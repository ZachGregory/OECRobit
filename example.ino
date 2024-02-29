// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
float gyro, angle;
float currentTime, prevTime, elapsedTime;

void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");
  delay(100);

  currentTime = millis();
  angle = 0;
}

void loop() {

  prevTime = currentTime;
  currentTime = millis();
  elapsedTime = (currentTime-prevTime)/1000;
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  gyro = g.gyro.z+0.02;   //+0.02 offset since it reads -0.02 when stable

  /* Print out the values */
  if (gyro > 0.07 || gyro <-0.07){
    angle = angle + gyro*elapsedTime*57.2958;
    Serial.print("GyroZ:");
    Serial.print(gyro,4);
    Serial.println("");
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.println("");
  }
  delay(10);
}