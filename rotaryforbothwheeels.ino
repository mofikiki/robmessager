#include <Arduino.h>
#include <Wire.h>

// Encoder pins (change these to the GPIO pins you want to use)
const int aPin1 = D2;  // Change to the GPIO pin you want to use
const int bPin1 = D3;  // Change to the GPIO pin you want to use
const int aPin2 = D5;  // Change to the GPIO pin you want to use
const int bPin2 = D6;  // Change to the GPIO pin you want to use

int counter1 = 0;
int aStateCLK1;
int bStateCLK1;
int lastStateCLK1;

int counter2 = 0;
int aStateCLK2;
int bStateCLK2;
int lastStateCLK2;

unsigned long prevTime = 0;
float speed1 = 0.0;
float distance1 = 0.0;
float speed2 = 0.0;
float distance2 = 0.0;

float encoderResolution1 = 30;  // Customize encoder resolution for encoder 1
float encoderResolution2 = 88;  // Customize encoder resolution for encoder 2
float gearRatio1 = 0.00163;     // Customize gear ratio for encoder 1
float gearRatio2 = 0.0179;      // Customize gear ratio for encoder 2
const float wheelCircumference = 20.43;

// Function to calculate speed from counter, resolution, and gear ratio
float calculateSpeed(int counter, float encoderResolution, float gearRatio) {
  unsigned long currentTime = millis();
  float elapsedMillis = float(currentTime - prevTime);
  prevTime = currentTime;

  return (counter / encoderResolution) / (elapsedMillis / 1000.0);
}

// Function to calculate distance from counter, resolution, and gear ratio
float calculateDistance(int counter, float encoderResolution, float gearRatio) {
  return (counter / encoderResolution) * (gearRatio * wheelCircumference);
}

// Define the updateEncoder1 function
ICACHE_RAM_ATTR void updateEncoder1() {
    // ISR code here

  aStateCLK1 = digitalRead(aPin1);
  bStateCLK1 = digitalRead(bPin1);

  if (aStateCLK1 != lastStateCLK1) {
    if (bStateCLK1 != lastStateCLK1) {
      counter1++;
    } else {
      counter1--;
    }
    speed1 = calculateSpeed(counter1, encoderResolution1, gearRatio1);
    distance1 = calculateDistance(counter1, encoderResolution1, gearRatio1);

    lastStateCLK1 = aStateCLK1;
  }
}

// Define the updateEncoder2 function
ICACHE_RAM_ATTR void updateEncoder2() {
  aStateCLK2 = digitalRead(aPin2);
  bStateCLK2 = digitalRead(bPin2);

  if (aStateCLK2 != lastStateCLK2) {
    if (bStateCLK2 != lastStateCLK2) {
      counter2++;
    } else {
      counter2--;
    }
    speed2 = calculateSpeed(counter2, encoderResolution2, gearRatio2);
    distance2 = calculateDistance(counter2, encoderResolution2, gearRatio2);

    lastStateCLK2 = aStateCLK2;
  }
}

void setup() {
  pinMode(aPin1, INPUT_PULLUP);  // Set as INPUT_PULLUP to enable internal pull-up resistors
  pinMode(bPin1, INPUT_PULLUP);
  pinMode(aPin2, INPUT_PULLUP);
  pinMode(bPin2, INPUT_PULLUP);

  Serial.begin(9600);

  // Attach interrupts to encoder pins
  attachInterrupt(digitalPinToInterrupt(aPin1), updateEncoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(bPin1), updateEncoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(aPin2), updateEncoder2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(bPin2), updateEncoder2, CHANGE);
}

void loop() {
  // Check if there is data available on the serial port
  while (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    // You can add the code to parse the serial data here if needed
  }

  // Send encoder data to the computer via Serial
  Serial.print("RIGHT  - Counter: ");
  Serial.print(counter1);
  Serial.print("\tSpeed (rotations/s): ");
  Serial.print(speed1);
  Serial.print("\tDistance (m): ");
  Serial.print(distance1, 4);

  Serial.print("\t LEFT - Counter: ");
  Serial.print(counter2);
  Serial.print("\tSpeed (rotations/s): ");
  Serial.print(speed2);
  Serial.print("\tDistance (m): ");
  Serial.println(distance2, 4);

  // Create a string to send both encoder data separated by a semicolon
  String encoderData = String(counter1) + ";" + String(speed1) + ";" + String(distance1, 4) + ";"
                       + String(counter2) + ";" + String(speed2) + ";" + String(distance2, 4) + "\n";

  // Send the combined encoder data to serial_port_arduino2
  Serial1.begin(9600); // Initialize the second serial port
  Serial1.print(encoderData);

  delay(100);  // Adjust the delay as needed
}