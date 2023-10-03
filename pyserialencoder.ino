#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
float gyroXOffset = 00.00;
float gyroYOffset = 0.00;
float gyroZOffset = 0.01;
const int numReadings = 10;
float gyroXReadings[numReadings];
float gyroYReadings[numReadings];
float gyroZReadings[numReadings];
float alpha = 0.98;
float pitch = 0;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(10);

  // Initialize MPU6050 sensor
  mpu.initialize();

  for (int i = 0; i < numReadings; i++) {
    gyroXReadings[i] = 0.0;
    gyroYReadings[i] = 0.0;
    gyroZReadings[i] = 0.0;
  }
}

void loop() {
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);

  float gyroX = (gx / 131.0) + gyroXOffset;
  float gyroY = (gy / 131.0) + gyroYOffset;
  float gyroZ = (gz / 131.0) + gyroZOffset;

  for (int i = numReadings - 1; i > 0; i--) {
    gyroXReadings[i] = gyroXReadings[i - 1];
    gyroYReadings[i] = gyroYReadings[i - 1];
    gyroZReadings[i] = gyroZReadings[i - 1];
  }
  gyroXReadings[0] = gyroX;
  gyroYReadings[0] = gyroY;
  gyroZReadings[0] = gyroZ;

  // pitch = alpha * (pitch + gyroY * 0.01) + (1 - alpha) * (atan2(-gyroX, gyroZ) * 180.0 / PI);

  // Send filtered pitch angle to the computer via Serial
  Serial.print("X: " );
  Serial.print(gyroX);
  Serial.print("   Y: ");
  Serial.println(gyroY);
 


  delay(100);  // Adjust the delay as needed
}
