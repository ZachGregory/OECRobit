#include <Servo.h>


Servo myServo;  // create servo object to control a servo
const int servoPin = 7; 
const int trigPin = 5;  
const int echoPin = 4; 

int directionPinA = 12;
int pwmPinA = 3;
int brakePinA = 9;

int directionPinB = 13;
int pwmPinB = 11;
int brakePinB = 8;

float pointCloud[180];

struct visionObj{
  float dist;
  int angle;
  bool isValid = false;
};

visionObj myObjects[30];

float duration, distance;


void setup() {
  myServo.attach(servoPin);  // attaches the servo on pin 7
  myServo.write(90);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(directionPinA, OUTPUT);
  pinMode(pwmPinA, OUTPUT);
  pinMode(brakePinA, OUTPUT);
  pinMode(directionPinB, OUTPUT);
  pinMode(pwmPinB, OUTPUT);
  pinMode(brakePinB, OUTPUT);

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  delay(3000);
  scan();
  objectDetection();
}

void scan(){
  myServo.write(0);
  delay(500);
  for (int i = 0; i<180; i++){
    myServo.write(i);
    delay(15);
    digitalWrite(trigPin, LOW);  
	  delayMicroseconds(2);  
	  digitalWrite(trigPin, HIGH);  
	  delayMicroseconds(10);  
	  digitalWrite(trigPin, LOW); 
    duration = pulseIn(echoPin, HIGH);
    distance = (duration*.0343)/2;
    Serial.print("Distance: ");
    Serial.println(distance);
    Serial.println(i);

    if (distance > 200){  //Throw out any distance over 2m as innacurate
      pointCloud[i] = NULL;
    } else{
      pointCloud[i] = distance;
    }
  }
  myServo.write(90);

}

void fwd(int dist, int trim){ //Dist in cm
  float distTrim = 1.00;
  digitalWrite(directionPinA, HIGH);
  digitalWrite(directionPinB, HIGH);
  digitalWrite(brakePinA, LOW);
  digitalWrite(brakePinB, LOW);
  if (trim < 0){
    analogWrite(pwmPinA, 100);
    analogWrite(pwmPinB, 100+trim);
  } else{
    analogWrite(pwmPinA, 100-trim);
    analogWrite(pwmPinB, 100);
  }

  delay(dist * distTrim);
  
  digitalWrite(brakePinA, HIGH);
  digitalWrite(brakePinB, HIGH);
  analogWrite(pwmPinA, 0);
  analogWrite(pwmPinB, 0);
}

void left(int angle, int trim){
  float angleTrim = 1.00;
  digitalWrite(directionPinA, LOW);
  digitalWrite(directionPinB, HIGH);
  digitalWrite(brakePinA, LOW);
  digitalWrite(brakePinB, LOW);

  if (trim < 0){
    analogWrite(pwmPinA, 100);
    analogWrite(pwmPinB, 100+trim);
  } else{
    analogWrite(pwmPinA, 100-trim);
    analogWrite(pwmPinB, 100);
  }
  
  delay(angle * angleTrim);
  
  digitalWrite(brakePinA, HIGH);
  digitalWrite(brakePinB, HIGH);
  analogWrite(pwmPinA, 0);
  analogWrite(pwmPinB, 0);
}

void right(int angle, int trim){
  float angleTrim = 1.00;
  digitalWrite(directionPinA, HIGH);
  digitalWrite(directionPinB, LOW);
  digitalWrite(brakePinA, LOW);
  digitalWrite(brakePinB, LOW);

  if (trim < 0){
    analogWrite(pwmPinA, 100);
    analogWrite(pwmPinB, 100+trim);
  } else{
    analogWrite(pwmPinA, 100-trim);
    analogWrite(pwmPinB, 100);
  }
  
  delay(angle * angleTrim);
  
  digitalWrite(brakePinA, HIGH);
  digitalWrite(brakePinB, HIGH);
  analogWrite(pwmPinA, 0);
  analogWrite(pwmPinB, 0);
}

void objectDetection(){
  int threshold = 10;  //5cm difference registers as an edge
  int risingEdge[30];
  int countRise = 0;
  int fallingEdge[30];
  int countFall = 0;

  for (int i = 0; i < 30; i++){ //Invalidate past objects
    myObjects[i].isValid = false;
  }

  for (int i = 0; i < 179; i++){  //For each angle detect rising/falling edges (rising is actually when the number gets smaller aka it begins to detect an object)
    if (pointCloud[i]==NULL && pointCloud[i+1] != NULL && countRise==countFall){  //NULL to not NULL
      risingEdge[countRise] = i;
      countRise++;
    }
    else if (pointCloud[i]!=NULL && pointCloud[i+1] == NULL && countRise-1==countFall){ //Not NULL to NULL
      fallingEdge[countFall] = i;
      countFall++;
    }
    else if (pointCloud[i]!=NULL && pointCloud[i+1] != NULL && countRise==countFall){
      if (pointCloud[i]-threshold > pointCloud[i+1]){
        risingEdge[countRise] = i;
        countRise++;
      }
      else if (pointCloud[i]+threshold < pointCloud[i+1] && countRise-1==countFall){
        fallingEdge[countFall] = i;
        countFall++;
      }
    }
  }

  for (int i = 0; i < countRise; i++){
    if (risingEdge[i]!=NULL && fallingEdge[i]!=NULL){
      myObjects[i].angle = int(((fallingEdge[i]-risingEdge[i])/2)+risingEdge[i]); //Middle of the object
      myObjects[i].dist = pointCloud[myObjects[i].angle]; //Distance to the middle of the object
      myObjects[i].isValid = true;
      //Serial.println(myObjects[i].angle);
    }
  }

}
                                                                                                                         
