
#include <Arduino_LSM9DS1.h>
#include <Adafruit_MMA8451.h>
#include <Arduino.h>

float x, y, z;
int degreesX = 0;
int degreesY = 0;
int count = 0;
int countY = 0;
int savedX = 0;
int savedY = 0;
Adafruit_MMA8451 mma = Adafruit_MMA8451();
sensors_event_t event; 

//starts accel, configures LED pins to output mode.
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");
  // if (!IMU.begin()) {
  //   Serial.println("Failed to initialize IMU!");
  //   while (1);
  // }
  // Serial.print("Accelerometer sample rate = ");
  // Serial.print(IMU.accelerationSampleRate());
  // Serial.println("Hz");

  if (!mma.begin()){
    Serial.println("Could not start MMA8451");
    while(1);
  }
  Serial.println("MMA8451 Started");
  mma.setRange(MMA8451_RANGE_4_G);
  Serial.println("setting pin 6");
  pinMode(6, INPUT);  
  Serial.println("setting pin 9");
  pinMode(9, INPUT);  
}
//tracks accel data to note changes w/ a 3 degree buffer.
//if no changes have happened within 20 seconds, flash LEDs.
void loop() {
  mma.getEvent(&event);

  if (event.acceleration.x > 0.1) {
    degreesX= 100 * event.acceleration.x;
    // degreesX = map(x, 0, 97, 0, 90);
    if (degreesX >= savedX - 3 && degreesX <= savedX + 3){
      count++;
    }
    else{
      count = 0;
      digitalWrite(6,LOW);
      digitalWrite(9,LOW);
    }
    Serial.print("Tilting up ");
    Serial.print(degreesX);
    Serial.println("  degrees");
    savedX = degreesX;
  }

  if (event.acceleration.x < -0.1){
    degreesX = 100 * event.acceleration.x;
    // degreesX = map(x, 0, -100, 0, 90);
    if (degreesX >= savedX - 3 && degreesX <= savedX + 3){
      count = count + 1;
    }
    else{
      Serial.println("LEDs OFF");
      count = 0;
      digitalWrite(6,LOW);
      digitalWrite(9,LOW);
    }
    Serial.print("Tilting up ");
    Serial.print(degreesX);
    Serial.println("  degrees");
    savedX = degreesX;
  }

  if (event.acceleration.y > 0.1) {
    degreesY = 100 * event.acceleration.y;
    // degreesY = map(y, 0, 97, 0, 90);
    if (degreesY >= savedY - 3 && degreesY <= savedY + 3){
      count++;
    }
    else{
      countY = 0;
      Serial.println("LEDs OFF");
      digitalWrite(6,LOW);
      digitalWrite(9,LOW);
    }
    savedY = degreesY;
    Serial.print("Tilting left ");
    Serial.print(degreesY);
    Serial.println("  degrees");
  }

  if (event.acceleration.y < -0.1) {
    degreesY = 100 * event.acceleration.y;
    // degreesY = map(y, 0, -100, 0, 90);
    if (degreesY >= savedY - 3 && degreesY <= savedY + 5){
      countY++;
    }
    else{
      countY = 0;
      Serial.println("LEDs OFF");
      digitalWrite(6,LOW);
      digitalWrite(9,LOW);
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
    digitalWrite(6,HIGH);
    digitalWrite(9,HIGH);
  }

  Serial.print("Count X:");
  Serial.print(count);
  Serial.print(" Count Y:");
  Serial.println(countY);
  delay(1000);
}
