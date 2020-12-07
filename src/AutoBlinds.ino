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
#include "/Users/Mat/Particle/community/libraries/Stepper@1.1.3/src/Stepper.h"
//#include <Stepper.h>

SerialLogHandler logHandler(115200, LOG_LEVEL_ERROR, {
    { "app", LOG_LEVEL_TRACE }, // enable all app messages
});

// Bluetooth characteristics
const char* serviceUuid  = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
const char* BlindsUp     = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
const char* BlindsDown   = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";

// Stepper motor setup
const int stepsPerRevolution = 2038;

// Variables for keeping Blinds state
typedef struct {
  uint8_t GoingUp;
  uint8_t GoingDown;
} BlindsState_t;

static BlindsState_t sBlindsState;

int i;
int j;

// Static function for handling Bluetooth Low Energy callbacks
static void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
  Log.info("Receiving BLE Data...");

  // We're only looking for one byte
  if( len != 1 ) {
    Log.info("Error: onDataReceived() Received more than one byte");
    return;
 	}

  if (context == BlindsUp)
  {
    sBlindsState.GoingUp = data[0];
    Log.info("Blinds Going Up.");
  }
  else if (context == BlindsDown)
  {
    sBlindsState.GoingDown = data[0];
    Log.info("Blinds Going Down.");
  }
}

void setup() {
  // Setup Stepper Motor control pins
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);


  // Set the Blinds BLE service
  BleUuid BlindsService(serviceUuid);

  // Initialize/Add the characteristics
  BleCharacteristic redCharacteristic("BlindsUp", BleCharacteristicProperty::WRITE_WO_RSP, BlindsUp, serviceUuid, onDataReceived, (void*)BlindsUp);
  BleCharacteristic greenCharacteristic("BlindsDown", BleCharacteristicProperty::WRITE_WO_RSP, BlindsDown, serviceUuid, onDataReceived, (void*)BlindsDown);
  BLE.addCharacteristic(redCharacteristic);
  BLE.addCharacteristic(greenCharacteristic);

  // Advertising data
  BleAdvertisingData advData;
  advData.appendServiceUUID(BlindsService);   // Add the Blinds Level service
  BLE.advertise(&advData);                    // Start advertising!


  i = 0;
  j =0;
}
 

void loop() {

  int loops = 1000;

  if (i == 100)
  {
    //Log.info("looping...: %d", j);
    i = 0;
    j += 1;
  }
  i += 1;


  
}






/*

for(int i = 0; i<loops; i++)
  {
    digitalWrite(D7, HIGH); digitalWrite(D6, LOW); digitalWrite(D5, LOW); digitalWrite(D4, LOW);
    delay(2);

    digitalWrite(D7, LOW); digitalWrite(D6, HIGH); digitalWrite(D5, LOW); digitalWrite(D4, LOW);
    delay(2);

    digitalWrite(D7, LOW); digitalWrite(D6, LOW); digitalWrite(D5, HIGH); digitalWrite(D4, LOW);
    delay(2);

    digitalWrite(D7, LOW); digitalWrite(D6, LOW); digitalWrite(D5, LOW); digitalWrite(D4, HIGH);
    delay(2);
  }

    // Rotation in opposite direction 
  for(int j = 0; j<loops; j++)
  {
    digitalWrite(D7, LOW); digitalWrite(D6, LOW); digitalWrite(D5, LOW); digitalWrite(D4, HIGH);
    delay(2);

    digitalWrite(D7, LOW); digitalWrite(D6, LOW); digitalWrite(D5, HIGH); digitalWrite(D4, LOW);
    delay(2);

    digitalWrite(D7, LOW); digitalWrite(D6, HIGH); digitalWrite(D5, LOW); digitalWrite(D4, LOW);
    delay(2);

    digitalWrite(D7, HIGH); digitalWrite(D6, LOW); digitalWrite(D5, LOW); digitalWrite(D4, LOW);
    delay(2);
  }
  */