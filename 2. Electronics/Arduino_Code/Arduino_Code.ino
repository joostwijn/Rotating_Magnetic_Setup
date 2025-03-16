#include "stepper.h"
#include "screen.h"

void setup() {
  initStepper();
  initScreen();
}

void loop() {
  runStepper();
  runScreen();
}
