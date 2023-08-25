#include <AccelStepper.h>
#include <ESP8266WiFi.h> // Include the ESP8266 WiFi library if needed

const int stepsPerRevolution = 200 * 4;  // 200 steps per revolution, 16 microsteps per step

// ULN2003 Motor Driver Pins
#define IN1 5
#define IN2 4
#define IN3 14
#define IN4 12

// Initialize the stepper library
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

void setup() {
  // Initialize the serial port
  Serial.begin(115200);
  
  // Set the speed and acceleration
  stepper.setMaxSpeed(100);
  stepper.setAcceleration(50);
  // Set target position
  stepper.moveTo(stepsPerRevolution);
}

void loop() {
  // Check current stepper motor position to invert direction
  if (stepper.distanceToGo() == 0){
    stepper.moveTo(-stepper.currentPosition());
    Serial.println("Changing direction");
  }
  // Move the stepper motor (one step at a time)
  stepper.run();
}
