#include <Arduino.h>
#include "stepper.h"

// --- Setting up the stepper motor and driver ---
// Create an AccelStepper object in DRIVER mode so that the Accel library knows we are using a driver (DRV8825).
// STEP_PIN: A pulse signal is sent to this pin. Each pulse moves the motor by one step (or microstep, depending on the driver settings).
// DIR_PIN: This pin controls the rotation direction. HIGH typically means clockwise (CW), and LOW means counterclockwise (CCW).
// STEP_PIN and DIR_PIN are defined in stepper.h.
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// --- Constants for step calculation ---
// Number of full steps per revolution (Nema 17 = 200 steps per rev)
const int FULL_STEPS_PER_REV = 200;   
// Microstepping multiplier: For 1/16 microstepping, a multiplier of 16 is needed in order to calculate the number of steps per rotation. Change accordingly.
const int MICROSTEP_MULTIPLIER = 16;    
// Total steps per revolution in microstepping mode
const int STEPS_PER_REV = FULL_STEPS_PER_REV * MICROSTEP_MULTIPLIER;  

// Flag indicating if the motor should run continuously
bool Continuous_Mode = false;

// ---------------------------------------------------------------------
// Initializes the stepper motor and sets up serial communication.
void initStepper() {
  Serial.begin(9600);
  stepper.setMaxSpeed(16000); // Set the maximum speed in steps/second. 
  // Set the current limit higher if the stepper cannot achieve the max speed https://www.makerguides.com/drv8825-stepper-motor-driver-arduino-tutorial/.
}

// ---------------------------------------------------------------------
// Updates the motor parameters.
// Parameters:
//   newSpeed: Desired speed in rotations per second (RPS).
//   newTurns: Number of full revolutions to move (0 for continuous motion).
//   newDir: 'R' for clockwise, 'L' for counterclockwise. Any other value stops the motor.
void updateStepper(float newSpeed, int newTurns, char newDir) {
  // Determine the direction factor: 1 for clockwise, -1 for counterclockwise.
  int directionFactor = (newDir == 'R') ? 1 : (newDir == 'L') ? -1 : 0;

  // Stop the motor if an invalid direction is provided, otherwise calculate the speed.
  if (directionFactor == 0) {
    Continuous_Mode = false;
    stepper.stop(); // Stop immediately.
    stepper.moveTo(stepper.currentPosition());  // Force the target to be the current position to eliminate any residual motion.
  } else {
    int speedStepsPerSec = newSpeed * STEPS_PER_REV;  // Convert the speed from rotations per second (RPS) to steps per second.

    // Do continuous turning if the number of turns is set to zero, otherwise move a specified number of revolutions.
    if (newTurns == 0) {
      // Continuous motion mode: run indefinitely at the specified speed.
      Continuous_Mode = true;
      stepper.setSpeed(directionFactor * speedStepsPerSec);
    } else {
      // Fixed-turn mode: move a specified number of revolutions.
      Continuous_Mode = false;
      long targetPosition = stepper.currentPosition() + (long)directionFactor * newTurns * STEPS_PER_REV;  // Calculate the target position relative to the current position.
      stepper.moveTo(targetPosition); // Set the target position.
      stepper.setSpeed(directionFactor * speedStepsPerSec); // Set the motor speed AFTER setting the target position.
    }
  }
}

// ---------------------------------------------------------------------
// Runs the motor based on the current mode.
// This function is called in the .ino file.
void runStepper() {
  if (Continuous_Mode) {
    stepper.runSpeed();         // Run continuously at a constant speed.
  } else {
    stepper.runSpeedToPosition(); // Move toward the target position at constant speed.
  }
}
