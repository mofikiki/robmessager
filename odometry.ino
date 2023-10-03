#include <Wire.h>
#include <MPU6050.h>

const int motorADirPin = D3;  // Motor A, direction pin
const int motorAPWMPin = D5;  // Motor A, PWM pin
const int motorBDirPin = D6;  // Motor B, direction pin
const int motorBPWMPin = D7;  // Motor B, PWM pin

int motorASpeed = 170;  // Speed for Motor A (0-255)
int motorBSpeed = 170;  // Speed for Motor B (0-255)

MPU6050 mpu;
float gyroXOffset = 0.00;
float gyroYOffset = 0.00;
float gyroZOffset = 0.01;
const int numReadings = 10;
float gyroXReadings[numReadings];
float gyroYReadings[numReadings];
float gyroZReadings[numReadings];
float alpha = 0.98;
float pitch = 0;

// Differential drive parameters
float wheelbase = 25.0;    // Distance between the two wheels (in units like centimeters)
float wheelRadius = 3.25;  // Radius of the wheels (in units like centimeters)

// Encoder counts per wheel revolution for each encoder
float encoderCountsPerRevolution1 = 30.0;  // Customize encoder resolution for encoder 1
float encoderCountsPerRevolution2 = 88.0;  // Customize encoder resolution for encoder 2

float counter2 = 0.0;
float counter1 = 0.0;

// Robot position and orientation
float x = 0.0;      // X position (in units like centimeters)
float y = 0.0;      // Y position (in units like centimeters)
float theta = 0.0;  // Orientation (in degrees)

// PID control parameters
float kp = 0.1;     // Proportional gain
float ki = 0.01;    // Integral gain
float kd = 0.1;     // Derivative gain
float error = 0.0;  // Error
float preerror = 0.0;  // Previous error
float errorsum = 0.0;  // Sum of errors
float controlpwm = 0.0;  // PWM control signal

// Variables for receiving desired position and movement commands
float desiredX = 0.0;
float desiredY = 0.0;
float desiredTheta = 0.0;
bool moveCommandReceived = false;

void setup() {
  pinMode(motorADirPin, OUTPUT);
  pinMode(motorAPWMPin, OUTPUT);
  pinMode(motorBDirPin, OUTPUT);
  pinMode(motorBPWMPin, OUTPUT);
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
  // Read incoming data from Serial
  while (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    parseSerialData(data);
  }




  

  // Calculate odometry based on encoder counts and differential drive kinematics
  float deltaLeft = (counter1 / encoderCountsPerRevolution1) * 2 * PI * wheelRadius;
  float deltaRight = (counter2 / encoderCountsPerRevolution2) * 2 * PI * wheelRadius;
  float deltaTheta = (deltaRight - deltaLeft) / wheelbase;
  float deltaDistance = (deltaLeft + deltaRight) / 2;

  x += deltaDistance * cos(theta);
  y += deltaDistance * sin(theta);
  theta += deltaTheta * 180.0 / PI;  // Convert to degrees

  float desired = getDesiredPositionFromServer();

  // Calculate PID control signal
float actual = getactualposition();  // Calculate the distance with orientation
  // Replace with your actual distance measurement logic
 
  preerror = error;
  error = desired - actual;
  errorsum = errorsum + error;
  controlpwm = (kp * error) + (ki * errorsum) - (kd * (error - preerror));

  // Use the movePID function to control motors
  movePID(controlpwm);

  // Send filtered pitch angle and robot position to the computer via Serial
  Serial.print("error: ");
  Serial.println(error);


  // Reset encoder counts
  counter1 = 0.0;
  counter2 = 0.0;

  delay(100);  // Adjust the delay as needed
}

void parseSerialData(String data) {
  // Parse incoming data from Python
  if (data.startsWith("DESIRED:")) {
    // Example format: "DESIRED: 10.0 20.0 30.0"
    sscanf(data.c_str(), "DESIRED: %f %f f", &desiredX, &desiredY, &desiredTheta);
    moveCommandReceived = true;
  } else {
    // Handle other types of data as needed
  }
}

// Function to get the desired position from Python
float getDesiredPositionFromServer() {
  // Return the desired position received from Python
  return desiredX,desiredY, desiredTheta;
}
float getactualposition(){
  return x,y,theta;
}
void movePID(float speed) {
  int signpwm;
  if (speed >= 0) {
    signpwm = 1;
    speed = speed * 4;
  } else {
    signpwm = -1;
  }
  int magpwm = constrain(abs(speed), 0, 250);
  analogWrite(motorAPWMPin, magpwm);
  analogWrite(motorBPWMPin, magpwm);
  if (signpwm == -1) {
    digitalWrite(motorADirPin, LOW);  // Reverse direction for Motor A
    digitalWrite(motorBDirPin, LOW);  // Reverse direction for Motor B
  } else {
    digitalWrite(motorADirPin, HIGH);  // Forward direction for Motor A
    digitalWrite(motorBDirPin, HIGH);  // Forward direction for Motor B
  }
}
