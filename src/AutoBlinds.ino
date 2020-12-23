/*
 * Project AutoBlinds
 * Description: Make my blinds controllable via bluetooth and mobile app! Eventually add sensors to make automatic?
 *   Parts:
 *   Particle Argon
 *   Stepper Motor Driver (ULN2003) + Stepper Motor: (28byj-48)
 *     32*63.68395 steps per revolution = 2037.8864 ~ 2038
 * Author: Mathieu Rauch
 * Date: Dec 5, 2020
 */

#include "Particle.h"
//#include "/Users/Mat/Particle/community/libraries/Stepper@1.1.3/src/Stepper.h"
//#include <Stepper.h>

SerialLogHandler logHandler(115200, LOG_LEVEL_ERROR, {
    { "app", LOG_LEVEL_TRACE }, // enable all app messages
});


int cLaserPin = D6;
int cIrSensorPin = D5;

int gPrevIRState;

void setup() {
  // Setup Stepper relay control pin
  pinMode(cLaserPin, OUTPUT);
  pinMode(cIrSensorPin, INPUT_PULLDOWN);

  gPrevIRState = 0;

}
 

void loop() {

  int lSensorStatus = digitalRead(cIrSensorPin);

  if (lSensorStatus != gPrevIRState)
  {
    Log.info("IR Sensor status:  %d", lSensorStatus);
    gPrevIRState = lSensorStatus;

    if (lSensorStatus == 1)
    {
      Log.info("Turning Laser on.");
      digitalWrite(cLaserPin, HIGH);
    }
    else
    {
      Log.info("Turning Laser off.");
      digitalWrite(cLaserPin, LOW);
    }
  }


  delay(2);

}
