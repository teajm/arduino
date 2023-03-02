
#include <Arduino_LSM6DS3.h>
#include <Arduino.h>

float x, y, z;
int degreesX = 0;
int degreesY = 0;
int count = 0;
int savedX = 0;
int savedY = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");
  pinMode(13, OUTPUT);  
}

void loop() {

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

  }
  //  digitalWrite(13, HIGH); // sets the digital pin 13 on
  // delay(1000);  

  if (x > 0.1) {
    x = 100 * x;
    degreesX = map(x, 0, 97, 0, 90);
    if (degreesX == savedX){
      count = count + 1;
    }
    else{
      count = 0;
      digitalWrite(6,LOW);
      digitalWrite(9,LOW);
    }

    Serial.print("Tilting up ");
    Serial.print(degreesX);
    Serial.println("  degrees");
  }
  if (x < -0.1) {
    x = 100 * x;
    degreesX = map(x, 0, -100, 0, 90);
    Serial.print("Tilting down ");
    Serial.print(degreesX);
    Serial.println("  degrees");
  }
  if (y > 0.1) {
    y = 100 * y;
    degreesY = map(y, 0, 97, 0, 90);
    Serial.print("Tilting left ");
    Serial.print(degreesY);
    Serial.println("  degrees");
  }
  if (y < -0.1) {
    y = 100 * y;
    degreesY = map(y, 0, -100, 0, 90);
    Serial.print("Tilting right ");
    Serial.print(degreesY);
    Serial.println("  degrees");
  }
  if(count == 20){
    digitalWrite(6,HIGH);
    digitalWrite(9,HIGH);
  }
  delay(1000);
}