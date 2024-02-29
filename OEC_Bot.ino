/*
* This code is a cleaned up version of the code that Zach Gregory, Liamm Mirza, Charlie Brockmann, and Riley Rogers 
* created for OEC 2023. To remember the memories we made and the accomplishments we achieved. 
* 
* This program controls a simple arduino robot that won us second place at OEC. 
* 
*/

#define DECODE_NEC          // Includes Apple and Onkyo
#define m1En 3  //PWM
#define m1Dr 11
#define m1Dr2 4

#define m2En 5  //PWM
#define m2Dr 6
#define m2Dr2 7

#define m3En 9  //PWM
#define m3Dr 8
#define m3Dr2 10

#define IR 2
#define RELAY 12

#include <Arduino.h>
#include <IRremote.hpp>

bool magnet = false;
unsigned long currentMillis;
unsigned long startMillis;
int speeds[] = {150, 200, 255};
int speedSet = 2;
int leftOffset = 0; //Gyro offsets correct unwanted movement
int rightOffset = 0;

void setup() {
    Serial.begin(115200);
    IrReceiver.begin(IR, ENABLE_LED_FEEDBACK);

    pinMode(m1En, OUTPUT);  //Motor 1
    pinMode(m1Dr, OUTPUT);
    pinMode(m1Dr2, OUTPUT);

    pinMode(m2En, OUTPUT);  //Motor 2
    pinMode(m2Dr, OUTPUT);
    pinMode(m2Dr2, OUTPUT);

    pinMode(m3En, OUTPUT);  //Motor 3
    pinMode(m3Dr, OUTPUT);
    pinMode(m3Dr2, OUTPUT);

    pinMode(RELAY, OUTPUT);

    digitalWrite(m1Dr, LOW);
    digitalWrite(m1Dr2, HIGH);

    digitalWrite(m2Dr, LOW);
    digitalWrite(m2Dr2, HIGH);

    digitalWrite(m3Dr, LOW);
    digitalWrite(m3Dr2, HIGH);

    digitalWrite(RELAY, LOW); //magnet starts off
}

void loop() {
  /*
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  gyro = g.gyro.z+0.02;   //+0.02 offset since it reads -0.02 when stable
  if (gyro > 0.07){
    rightOffset = 100*gyro;
    leftOffset = 0;
    Serial.print("GyroZ:");
    Serial.print(gyro,4);
    Serial.println("");
  } else if (gyro < -0.07){
    leftOffset = 100*gyro*-1;
    rightOffset = 0;
    Serial.print("GyroZ:");
    Serial.print(gyro,4);
    Serial.println("");
  } else{
    rightOffset = 0;
    leftOffset = 0;
  }*/

  //Check if IR message has been received 
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.protocol);

    //Print a short summary of received data
    if (IrReceiver.decodedIRData.protocol == NEC) {
      Serial.println();
      IrReceiver.printIRResultShort(&Serial);
      IrReceiver.printIRSendUsage(&Serial);
    }

    IrReceiver.resume(); // Enable receiving of the next value

    //Finally, check the received data and perform actions according to the received command
    if (IrReceiver.decodedIRData.command == 0x18) {             //Drive Forward
      analogWrite(m1En, speeds[speedSet]-leftOffset); // Send PWM signal to L298N Enable pin
      digitalWrite(m1Dr, HIGH); //Set motor1 direction
      digitalWrite(m1Dr2, LOW);
      analogWrite(m2En, speeds[speedSet]-rightOffset);
      digitalWrite(m2Dr, LOW);  //Set motor2 direction
      digitalWrite(m2Dr2, HIGH);
      Serial.println("forwards"); //Print message
      delay(100);
    } else if (IrReceiver.decodedIRData.command == 0x52) {      //Drive Backward
      analogWrite(m1En, speeds[speedSet]-leftOffset);
      digitalWrite(m1Dr, LOW);
      digitalWrite(m1Dr2, HIGH);
      analogWrite(m2En, speeds[speedSet]-rightOffset);
      digitalWrite(m2Dr, HIGH);
      digitalWrite(m2Dr2, LOW);
      Serial.println("backwards");
      delay(100);
    } else if (IrReceiver.decodedIRData.command == 0x8) {      //Drive Left
      analogWrite(m1En, speeds[speedSet]-leftOffset);
      digitalWrite(m1Dr, HIGH);
      digitalWrite(m1Dr2, LOW);
      analogWrite(m2En, speeds[speedSet]-rightOffset);
      digitalWrite(m2Dr, HIGH);
      digitalWrite(m2Dr2, LOW);
      Serial.println("left");
      delay(100);
    }else if (IrReceiver.decodedIRData.command == 0x5A) {      //Drive Right
      analogWrite(m1En, speeds[speedSet]-leftOffset);
      digitalWrite(m1Dr, LOW);
      digitalWrite(m1Dr2, HIGH);
      analogWrite(m2En, speeds[speedSet]-rightOffset);
      digitalWrite(m2Dr, LOW);
      digitalWrite(m2Dr2, HIGH);
      Serial.println("right");
      delay(100);
    }else if (IrReceiver.decodedIRData.command == 0x15) {      //Crane Up
      analogWrite(m3En, 255);
      digitalWrite(m3Dr, HIGH);
      digitalWrite(m3Dr2, LOW);
      Serial.println("up");
      delay(100);
    }else if (IrReceiver.decodedIRData.command == 0x19) {      //Crane Down
      analogWrite(m3En, 255);
      digitalWrite(m3Dr, LOW);
      digitalWrite(m3Dr2, HIGH);
      Serial.println("down");
      delay(100);
    }else if (IrReceiver.decodedIRData.command == 0x1C) {      //Magnet toggle
      if (magnet){  //If on turn off
        digitalWrite(RELAY, LOW);
      }else{
        digitalWrite(RELAY, HIGH);
      }
      magnet=!magnet;
      Serial.println("Mag toggle");
      delay(100);
    } else if (IrReceiver.decodedIRData.command == 0xD) {      //Speed up
      if (speedSet<2){
        speedSet++;
      }
      Serial.println("Speed up");
      delay(100);
    } else if (IrReceiver.decodedIRData.command == 0x16) {      //Speed down
      if (speedSet>0){
        speedSet--;
      }
      Serial.println("Slow down");
      delay(500);
    }
    startMillis = millis();
  }

  if (millis() >= 10){ //Stop
    analogWrite(m1En, 0);
    analogWrite(m2En, 0);
    analogWrite(m3En, 0);
  }
  delay(10);
}
