#include <Servo.h>

const int e18_sensor = 7;
volatile int state = HIGH;

Servo servoMotor;
int angle = 0;

void setup() {
  Serial.begin(115200);
  servoMotor.attach(3);
  servoMotor.write(angle);

  pinMode(e18_sensor, INPUT);
  attachInterrupt(digitalPinToInterrupt(e18_sensor), sensorInterrupt, FALLING);
}

void loop() {
  for (angle = 0; angle <= 180; angle += 1) {
    servoMotor.write(angle);
    printStateAndAngle();
    // delay(10);
  }

  for (angle = 180; angle >= 0; angle -= 1) {
    servoMotor.write(angle);
    printStateAndAngle();
    // delay(10);
  }
}

void sensorInterrupt() {
  state = LOW;
}

void printStateAndAngle() {
  state = digitalRead(e18_sensor);
  delay(10);
  if (state == LOW) {
    Serial.print("All clear - ");
  } else {
    Serial.print("Object detected- ");
  }
  Serial.print("Angle: ");
  Serial.println(angle);
}
