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
void RollBlindsOn();
void RollBlindsOff();
static void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);
void setup();
void loop();
#line 14 "/Users/Mat/Desktop/AutoBlinds/AutoBlinds/src/AutoBlinds.ino"
SerialLogHandler logHandler(115200, LOG_LEVEL_ERROR, {
    { "app", LOG_LEVEL_TRACE }, // enable all app messages
});

// Variables for keeping Blinds state
typedef struct {
  uint8_t GoUp;
  uint8_t GoDown;
  uint8_t SensorTop;
  uint8_t SensorBottom;
} BlindsState_t;
static BlindsState_t sBlindsState;

//          Blinds H-Bridge relay control setup and control functions  
/*________________________________________________________________________________________*/

// Set H-Bridge relay control pins (Swap Hot/Cold relays must always be swapped together) 
int Pin_BlindsSwapColdRelay  = D5;
int Pin_BlindsSwapHotRelay   = D6;
int Pin_BlindsStartStopRelay = D7;

// Define H-Bridge relay control functions
void RollBlindsUp()   { digitalWrite(Pin_BlindsSwapColdRelay, HIGH); digitalWrite(Pin_BlindsSwapHotRelay, HIGH); }
void RollBlindsDown() { digitalWrite(Pin_BlindsSwapColdRelay,  LOW); digitalWrite(Pin_BlindsSwapHotRelay,  LOW);  }

void RollBlindsOn()   { digitalWrite(Pin_BlindsStartStopRelay, HIGH); }
void RollBlindsOff()  { digitalWrite(Pin_BlindsStartStopRelay,  LOW); }

//          Define Bluetooth services and OnDataReceived callback function
/*________________________________________________________________________________________*/

// Bluetooth characteristics
const char* serviceUuid  = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
const char* BlindsUp     = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
const char* BlindsDown   = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";

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

//          
/*________________________________________________________________________________________*/

int i, j;
bool sRelayState;



//                                  setup() and loop() functions
/*________________________________________________________________________________________*/

void setup() {
  // Setup relay control pins (Swap Hot/Cold relays must always be swapped together)
  pinMode(Pin_BlindsSwapColdRelay,  OUTPUT);
  pinMode(Pin_BlindsSwapHotRelay,   OUTPUT);
  pinMode(Pin_BlindsStartStopRelay, OUTPUT);

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

  RollBlindsOn();
  delay(3000);
  RollBlindsUp();
  delay(3000);
  RollBlindsDown();
  delay(3000);
  RollBlindsOff();
  delay(3000);
}

