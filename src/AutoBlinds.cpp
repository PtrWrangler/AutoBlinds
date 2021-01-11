/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "/Users/Mat/Desktop/AutoBlinds/AutoBlinds/src/AutoBlinds.ino"
/*
 * Project AutoBlinds
 * Description: Make my blinds controllable via bluetooth and mobile app! Eventually add sensors to make automatic?
 * Parts:
 *   Particle Argon
 *   6-24 Volt DC Gearhead Motor
 *   3 Relays allowing for an H-Bridge wiring setup to control the direction and power to the motor
 * Author: Mathieu Rauch
 * Date: Dec 5, 2020
 */

#include "Particle.h"

void RollBlindsUp();
void RollBlindsDown();
void BlindsSetPower(bool ActivatePower);
static void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);
void setup();
void loop();
#line 14 "/Users/Mat/Desktop/AutoBlinds/AutoBlinds/src/AutoBlinds.ino"
SerialLogHandler logHandler(115200, LOG_LEVEL_ERROR, {
    { "app", LOG_LEVEL_TRACE }, // enable all app messages
});

// Variables for keeping Blinds state
typedef struct {
  uint8_t Power;
  uint8_t GoUp;
  uint8_t GoDown;
  uint8_t GoTop;
  uint8_t GoBottom;
  uint8_t SensorTop;
  uint8_t SensorBottom;
} BlindsState_t;
static BlindsState_t sBlindsState;

//          Define IO Pins  and Gearmotor H-Bridge relay control functions  
/*________________________________________________________________________________________*/

// Set H-Bridge relay control pins (Swap Hot/Cold relays must always be swapped together) 
int Pin_IrSensorTop          = D2;
int Pin_IrSensorBottom       = D3;
int Pin_BlindsSwapColdRelay  = D5;
int Pin_BlindsSwapHotRelay   = D6;
int Pin_BlindsStartStopRelay = D7;

// Define H-Bridge relay control functions (Both Relays must be switched together)
void RollBlindsUp()   { digitalWrite(Pin_BlindsSwapColdRelay,  LOW); digitalWrite(Pin_BlindsSwapHotRelay,  LOW);  }
void RollBlindsDown() { digitalWrite(Pin_BlindsSwapColdRelay, HIGH); digitalWrite(Pin_BlindsSwapHotRelay, HIGH); }

void BlindsSetPower(bool ActivatePower) 
{
  if (ActivatePower)
    digitalWrite(Pin_BlindsStartStopRelay, LOW);
  else
    digitalWrite(Pin_BlindsStartStopRelay,  HIGH);
}

//          Define Bluetooth services and OnDataReceived callback function
/*________________________________________________________________________________________*/

// Bluetooth characteristics
const char* serviceUuid  = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
const char* BlindsPower  = "6E400004-B5A3-F393-E0A9-E50E24DCCA9E";
const char* BlindsUp     = "6E400005-B5A3-F393-E0A9-E50E24DCCA9E";
const char* BlindsDown   = "6E400006-B5A3-F393-E0A9-E50E24DCCA9E";
const char* BlindsTop    = "6E400007-B5A3-F393-E0A9-E50E24DCCA9E";
const char* BlindsBottom = "6E400008-B5A3-F393-E0A9-E50E24DCCA9E";

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
    Log.info("Blinds Up characteristic received %d", data[0]);
  }
  else if (context == BlindsDown)
  {
    sBlindsState.GoDown = data[0];
    Log.info("Blinds Down characteristic received %d", data[0]);
  }
  else if (context == BlindsTop)
  {
    sBlindsState.GoTop = data[0];
    Log.info("Blinds Top characteristic received %d", data[0]);
  }
  else if (context == BlindsBottom)
  {
    sBlindsState.GoBottom = data[0];
    Log.info("Blinds Bottom characteristic received %d", data[0]);
  }
  else if (context == BlindsPower)
  {
    sBlindsState.Power = data[0];
    Log.info("Blinds Power characteristic received %d", data[0]);
  }
}

//                                  setup() and loop() functions
/*________________________________________________________________________________________*/

void setup() {
  // Setup relay control pins (Swap Hot/Cold relays must always be swapped together)
  pinMode(Pin_BlindsSwapColdRelay,  OUTPUT);
  pinMode(Pin_BlindsSwapHotRelay,   OUTPUT);
  pinMode(Pin_BlindsStartStopRelay, OUTPUT);
  pinMode(Pin_IrSensorTop,          INPUT_PULLDOWN);
  pinMode(Pin_IrSensorBottom,       INPUT_PULLDOWN);

  // Initialize Blinds State Structure
  sBlindsState.Power        = 0;
  sBlindsState.GoUp         = 0;
  sBlindsState.GoDown       = 0;
  sBlindsState.GoTop        = 0;
  sBlindsState.GoBottom     = 0;
  sBlindsState.SensorTop    = 0; 
  sBlindsState.SensorBottom = 0;

  // Set the Blinds BLE service
  BleUuid BlindsService(serviceUuid);

  // Initialize/Add the BLE characteristics
  BleCharacteristic BlindsPowerCharacteristic ("BlindsPower", BleCharacteristicProperty::WRITE_WO_RSP, BlindsPower, serviceUuid, onDataReceived, (void*)BlindsPower);
  BleCharacteristic BlindsUpCharacteristic    ("BlindsUp", BleCharacteristicProperty::WRITE_WO_RSP, BlindsUp, serviceUuid, onDataReceived, (void*)BlindsUp);
  BleCharacteristic BlindsDownCharacteristic  ("BlindsDown", BleCharacteristicProperty::WRITE_WO_RSP, BlindsDown, serviceUuid, onDataReceived, (void*)BlindsDown);
  BleCharacteristic BlindsTopCharacteristic   ("BlindsTop", BleCharacteristicProperty::WRITE_WO_RSP, BlindsTop, serviceUuid, onDataReceived, (void*)BlindsTop);
  BleCharacteristic BlindsBottomCharacteristic("BlindsBottom", BleCharacteristicProperty::WRITE_WO_RSP, BlindsBottom, serviceUuid, onDataReceived, (void*)BlindsBottom);
  BLE.addCharacteristic(BlindsPowerCharacteristic);
  BLE.addCharacteristic(BlindsUpCharacteristic);
  BLE.addCharacteristic(BlindsDownCharacteristic);
  BLE.addCharacteristic(BlindsTopCharacteristic);
  BLE.addCharacteristic(BlindsBottomCharacteristic);

  // BLE Advertising data
  BleAdvertisingData advData;
  advData.appendServiceUUID(BlindsService);   // Add the Blinds Level service
  BLE.advertise(&advData);                    // Start advertising!
}
 

void loop() {
  sBlindsState.SensorTop    = digitalRead(Pin_IrSensorTop);
  sBlindsState.SensorBottom = digitalRead(Pin_IrSensorBottom);

  bool lCalcActivatePower = false;

  if (sBlindsState.GoUp)
  {
    RollBlindsUp();
    if (sBlindsState.Power && !sBlindsState.SensorTop)
      lCalcActivatePower = true;
  }
  else if (sBlindsState.GoDown)
  {
    RollBlindsDown();
    if (sBlindsState.Power && sBlindsState.SensorBottom)
    lCalcActivatePower = true;
  }
  
  BlindsSetPower(lCalcActivatePower);

  // RollBlindsOn();
  // delay(3000);
  // RollBlindsUp();
  // delay(3000);
  // RollBlindsDown();
  // delay(3000);
  // RollBlindsOff();
  // int val = analogRead(A2);
  // Log.info("pot value: %d", val);
  // delay(3000);
}

