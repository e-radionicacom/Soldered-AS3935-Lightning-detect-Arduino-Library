/**
 **************************************************

   @file        Example1_BasicLightning_I2C.ino
   @brief       This example demonstrates how to detect lightning! It has a few basic
                settings to help with rejecting noise or "disturbers" (false lightning events).
                It uses the onboard interrupt hardware pin, so in addition to attaching to
                it data lines you'll need to connnect to the interrupt pin labled "INT".


 	product: www.solde.red/333097

   @authors     Elias Santistevan, SparkFun Electronics

    Modified by Soldered.com
 ***************************************************/

#include <Wire.h>
#include "AS3935-Lightning-sensor-SOLDERED.h"

// 0x03 is default, but the address can also be 0x02, 0x01.
// Adjust the address jumpers on the underside of the product.
#define AS3935_ADDR 0x03
#define INDOOR 0x12
#define OUTDOOR 0xE
#define LIGHTNING_INT 0x08
#define DISTURBER_INT 0x04
#define NOISE_INT 0x01

AS3935 lightning(AS3935_ADDR);

// Interrupt pin for lightning detection
const int intPin = 4;

// This variable holds the number representing the lightning or non-lightning
// event issued by the lightning detector.
int intVal = 0;
int noise = 2; // Value between 1-7
int disturber = 2; // Value between 1-10

void setup()
{
  // When lightning is detected the interrupt pin goes HIGH.
  pinMode(intPin, INPUT);

  Serial.begin(115200);
  Serial.println("AS3935 Franklin Lightning Detector");

  Wire.begin(); // Begin Wire before lightning sensor.

  if ( !lightning.begin() ) { // Initialize the sensor.
    Serial.println ("Lightning Detector did not start up, freezing!");
    while (1);
  }
  else
    Serial.println("Schmow-ZoW, Lightning Detector Ready!");

  // The lightning detector defaults to an indoor setting at
  // the cost of less sensitivity, if you plan on using this outdoors
  // uncomment the following line:
  lightning.setIndoorOutdoor(OUTDOOR);

    // This line should be called before using sensor. This function
  // calibrates lightning sensor.
  lightning.tuneAntenna();
}

void loop()
{
  if (digitalRead(intPin) == HIGH) {
    // Hardware has alerted us to an event, now we read the interrupt register
    // to see exactly what it is.
    intVal = lightning.readInterruptReg();
    if (intVal == NOISE_INT) {
      Serial.println("Noise.");
      // Too much noise? Uncomment the code below, a higher number means better
      // noise rejection.
      //lightning.setNoiseLevel(setNoiseLevel);
    }
    else if (intVal == DISTURBER_INT) {
      Serial.println("Disturber.");
      // Too many disturbers? Uncomment the code below, a higher number means better
      // disturber rejection.
      //lightning.watchdogThreshold(threshVal);
    }
    else if (intVal == LIGHTNING_INT) {
      Serial.println("Lightning Strike Detected!");
      // Lightning! Now how far away is it? Distance estimation takes into
      // account any previously seen events in the last 15 seconds.
      byte distance = lightning.distanceToStorm();
      Serial.print("Approximately: ");
      Serial.print(distance);
      Serial.println("km away!");
    }
  }
  delay(1000); // Slow it down.
}
