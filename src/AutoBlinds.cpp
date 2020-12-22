/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "/Users/Mat/Desktop/AutoBlinds/AutoBlinds/src/AutoBlinds.ino"
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

static void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);
void setup();
void loop();
#line 16 "/Users/Mat/Desktop/AutoBlinds/AutoBlinds/src/AutoBlinds.ino"
SerialLogHandler logHandler(115200, LOG_LEVEL_ERROR, {
    { "app", LOG_LEVEL_TRACE }, // enable all app messages
});

// Bluetooth characteristics
const char* serviceUuid  = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
const char* BlindsUp     = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
const char* BlindsDown   = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";

// Stepper motor setup
const int stepsPerRevolution = 2038;
const int sStepUpDelay         = 15;
const int sStepDownDelay       = 2;

//Stepper stepper(stepsPerRevolution, 4, 5, 6, 7);

// Variables for keeping Blinds state
typedef struct {
  uint8_t GoUp;
  uint8_t GoDown;
  uint8_t SensorTop;
  uint8_t SensorBottom;
} BlindsState_t;

int i, j;
bool sRelayState;

static BlindsState_t sBlindsState;

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
    sBlindsState.GoUp = data[0];
    Log.info("Blinds Going Up.");
  }
  else if (context == BlindsDown)
  {
    sBlindsState.GoDown = data[0];
    Log.info("Blinds Going Down.");
  }
}

void setup() {
  // Setup Stepper relay control pin
  pinMode(D2, OUTPUT);

  // Initialize Blinds State Structure
  sBlindsState.GoUp         = 0;
  sBlindsState.GoDown       = 0;
  sBlindsState.SensorTop    = 0; 
  sBlindsState.SensorBottom = 0;

  // Set the Blinds BLE service
  BleUuid BlindsService(serviceUuid);

  // Initialize/Add the BLE characteristics
  BleCharacteristic redCharacteristic("BlindsUp", BleCharacteristicProperty::WRITE_WO_RSP, BlindsUp, serviceUuid, onDataReceived, (void*)BlindsUp);
  BleCharacteristic greenCharacteristic("BlindsDown", BleCharacteristicProperty::WRITE_WO_RSP, BlindsDown, serviceUuid, onDataReceived, (void*)BlindsDown);
  BLE.addCharacteristic(redCharacteristic);
  BLE.addCharacteristic(greenCharacteristic);

  // BLE Advertising data
  BleAdvertisingData advData;
  advData.appendServiceUUID(BlindsService);   // Add the Blinds Level service
  BLE.advertise(&advData);                    // Start advertising!


  i = 0;
  j = 0;
  sRelayState = false;
}
 

void loop() {


  if (i == 3000)
  {
    
    i = 0;
    j += 1;
    sRelayState = !sRelayState;
    Log.info("changing state: %d", sRelayState);
  }
  i += 1;

  // if (sRelayState)
  // {
  //   digitalWrite(D2, HIGH);
  // }
  // else
  // {
  //   digitalWrite(D2, LOW);
  // }
  digitalWrite(D2, HIGH);
  delay(2);

  // if (sBlindsState.GoUp)
  // {
  //   RotateStepperMotorClockwiseUp();
  // }
  // else if (sBlindsState.GoDown)
  // {
  //   RotateStepperMotorCounterclockwiseDown();
  // }
}

// void RotateStepperMotorClockwiseUp()
// {
//   digitalWrite(D7, HIGH); digitalWrite(D6, LOW); digitalWrite(D5, LOW); digitalWrite(D4, LOW);
//   delay(sStepUpDelay);

//   digitalWrite(D7, LOW); digitalWrite(D6, HIGH); digitalWrite(D5, LOW); digitalWrite(D4, LOW);
//   delay(sStepUpDelay);

//   digitalWrite(D7, LOW); digitalWrite(D6, LOW); digitalWrite(D5, HIGH); digitalWrite(D4, LOW);
//   delay(sStepUpDelay);

//   digitalWrite(D7, LOW); digitalWrite(D6, LOW); digitalWrite(D5, LOW); digitalWrite(D4, HIGH);
//   delay(sStepUpDelay);
// }

// void RotateStepperMotorCounterclockwiseDown()
// {
//   digitalWrite(D7, LOW); digitalWrite(D6, LOW); digitalWrite(D5, LOW); digitalWrite(D4, HIGH);
//   delay(sStepDownDelay);

//   digitalWrite(D7, LOW); digitalWrite(D6, LOW); digitalWrite(D5, HIGH); digitalWrite(D4, LOW);
//   delay(sStepDownDelay);

//   digitalWrite(D7, LOW); digitalWrite(D6, HIGH); digitalWrite(D5, LOW); digitalWrite(D4, LOW);
//   delay(sStepDownDelay);

//   digitalWrite(D7, HIGH); digitalWrite(D6, LOW); digitalWrite(D5, LOW); digitalWrite(D4, LOW);
//   delay(sStepDownDelay);
// }

