// Standalone Limit Switch Code with interrupt. This requires no external resistor 
//   and is as simple as connecting a conductive element across the triggerPin and GND. 
// Written by Zach Gregory


bool flag = false;

const int triggerPin = 23;

void IRAM_ATTR isr() {
  flag = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(triggerPin, INPUT_PULLUP);
  attachInterrupt(triggerPin, isr, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (flag == true){
    Serial.println("owo");
    flag = false;
  }
  delay(500);
}
