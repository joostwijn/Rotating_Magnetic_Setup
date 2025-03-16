#include <Arduino.h>
#include <SoftwareSerial.h>
#include "screen.h"
#include "stepper.h"  // To call updateStepper()

// The screen operates as a separate UI with its own microprocessor, sending only relevant data to the Arduino via the serial port.  
// This approach decouples the menu logic from the Arduino, reducing processing load and making the system much easier to operate.  
// Refer to https://nextion.tech/instruction-set/ and https://nextion.tech/editor_guide/ for relevant Nextion documentation.

// Define SoftwareSerial on pins 10 (RX) and 11 (TX). A software serial is created so we can send code to the arduino whilst receiving data from the screen.
SoftwareSerial displaySerial(10, 11);

void initScreen() {
  displaySerial.begin(9600);
}

void runScreen() {
  // Wait until at least 3 bytes are available.
  if (displaySerial.available() >= 3) {
    // Read the first byte and convert it to a float (speed in RPS).
    int rawSpeed = displaySerial.read();
    float newSpeed = rawSpeed / 10.0;  // e.g., raw value 36 becomes 3.6 RPS.
    
    // Read the second byte as the number of turns in integer.
    int newTurns = displaySerial.read();  // 0 means continuous.
    
    // Read the third byte as the direction character.
    char newDir = (char)displaySerial.read();  // 'R', 'L', or 'S'.
    
    // Debug output.
    Serial.print("Screen - Speed = ");
    Serial.print(newSpeed);
    Serial.print(" RPS, Turns = ");
    Serial.print(newTurns);
    Serial.print(", Dir = ");
    Serial.println(newDir);
    
    // Update the stepper motor using the screen data.
    updateStepper(newSpeed, newTurns, newDir);
    
    Serial.println("---------------------");
  }
}
