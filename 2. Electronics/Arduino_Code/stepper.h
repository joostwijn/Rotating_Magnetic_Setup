#ifndef STEPPER_H
#define STEPPER_H

#include <AccelStepper.h>

#define DIR_PIN 2     // Direction pin
#define STEP_PIN 3    // Step pin

extern AccelStepper stepper;

// Initializes the stepper motor.
void initStepper();

// Updates the stepper parameters using the screen values.
// newSpeed is in rotations per second (RPS),
// newTurns is the number of turns (0 for continuous),
// newDir is 'R' for right, 'L' for left, or 'S' for stop.
void updateStepper(float newSpeed, int newTurns, char newDir);

// Runs the stepper motor (either constant speed or with acceleration).
void runStepper();

#endif // STEPPER_H
