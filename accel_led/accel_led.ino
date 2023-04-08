

#include <Adafruit_MMA8451.h>
#include <Arduino.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();
sensors_event_t event; 

float x, y, z;
int degreesX = 0;
int degreesY = 0;
int count = 0;
int countY = 0;
int savedX = 0;
int savedY = 0;
int OFFSET = 10;


//starts accel, configures LED pins to output mode.
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");
  if (!mma.begin()){
    Serial.println("Could not start MMA8451");
    while(1);
  }
  Serial.println("MMA8451 Started");
  mma.setRange(MMA8451_RANGE_2_G);
  Serial.println("setting pin 6");
  pinMode(6, OUTPUT);  
  Serial.println("setting pin 9");
  pinMode(9, OUTPUT);  
}
//tracks accel data to note changes w/ a 3 degree buffer.
//if no changes have happened within 20 seconds, flash LEDs.
void loop() {
  mma.getEvent(&event);
  Serial.println(event.acceleration.x*100);
  Serial.println(event.acceleration.y*100);
  if (event.acceleration.x > 0.1) {
    degreesX= 100 * event.acceleration.x;
    // degreesX = map(x, 0, 97, 0, 90);
    if (degreesX >= savedX - OFFSET && degreesX <= savedX + OFFSET){
      count++;
    }
    else{
      count = 0;
      digitalWrite(6,HIGH);
      digitalWrite(9,HIGH);
    }
    Serial.print("Tilting up ");
    Serial.print(degreesX);
    Serial.println("  degrees");
    savedX = degreesX;
  }

  if (event.acceleration.x < -0.1){
    degreesX = 100 * event.acceleration.x;
    // degreesX = map(x, 0, -100, 0, 90);
    if (degreesX >= savedX - OFFSET && degreesX <= savedX + OFFSET){
      count = count + 1;
    }
    else{
      Serial.println("LEDs OFF");
      count = 0;
      digitalWrite(6,HIGH);
      digitalWrite(9,HIGH);
    }
    Serial.print("Tilting up ");
    Serial.print(degreesX);
    Serial.println("  degrees");
    savedX = degreesX;
  }

  if (event.acceleration.y > 0.1) {
    degreesY = 100 * event.acceleration.y;
    // degreesY = map(y, 0, 97, 0, 90);
    if (degreesY >= savedY - OFFSET && degreesY <= savedY + OFFSET){
      count++;
    }
    else{
      countY = 0;
      Serial.println("LEDs OFF");
      digitalWrite(6,HIGH);
      digitalWrite(9,HIGH);
    }
    savedY = degreesY;
    Serial.print("Tilting left ");
    Serial.print(degreesY);
    Serial.println("  degrees");
  }

  if (event.acceleration.y < -0.1) {
    degreesY = 100 * event.acceleration.y;
    // degreesY = map(y, 0, -100, 0, 90);
    if (degreesY >= savedY - OFFSET && degreesY <= savedY + OFFSET){
      countY++;
    }
    else{
      countY = 0;
      Serial.println("LEDs OFF");
      digitalWrite(6,HIGH);
      digitalWrite(9,HIGH);
    }
    savedY = degreesY;
    Serial.print("Tilting right ");
    Serial.print(degreesY);
    Serial.println("  degrees");
  }

  if(event.acceleration.x == 0 || event.acceleration.y == 0){
    Serial.print("Neutral");
    countY++;
  }
  
  if(count == 20 || countY == 20){
    Serial.println("LEDs ON");
    digitalWrite(6,LOW);
    digitalWrite(9,LOW);
  }

  Serial.print("Count X:");
  Serial.print(count);
  Serial.print(" Count Y:");
  Serial.println(countY);
  delay(1000);
}
