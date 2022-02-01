#pragma once

// MUst be updated to the new Project HW !!!
#define PROG_LED_PIN 10
#define PROG_LED_PIN_ACTIVE_ON HIGH
#define PROG_BUTTON_PIN 8
#define PROG_BUTTON_PIN_INTERRUPT_ON FALLING
//#define SAVE_INTERRUPT_PIN A2 
#define LED_YELLOW_PIN 0


/* Define Arduino Board */
/* ******************** */
//#define ArduinoZero
//#define ItsyBitsyM0
#define Xiao

//Debug Outputs
#define KDEBUG_min
#define KDEBUG
#define KDEBUG_ID
#define KDEBUG_Rocker           // comment this line to disable DEBUG Rocker info
#define KDEBUG_handled          // comment this line to disable DEBUG info Data handled
//#define KDEBUG_Received         // comment this line to disable DEBUG mode for Recvived Data infomration
//#define KDEBUG_Repeater         // comment this line to disable DEBUG mode for Repeater
//#define KDEBUG_BaseID           // comment this line to disable DEBUG mode for Base-ID

// Defines for Testing
//#define EnOceanTEST

// disable define to disable KNX Funktion (for Testing ENocean ONLY)
#define KNXenable 

#ifdef KDEBUG_min
// Get correct serial port for debugging
#ifdef ArduinoZero
#define SERIAL_PORT SerialUSB   // Serial port for Arduino Zero
#endif 
#ifdef ItsyBitsyM0
#define SERIAL_PORT Serial      // Serial port for ItsyBitsy
#endif
#ifdef Xiao
#define SERIAL_PORT SerialUSB      // Serial port for ItsyBitsy
#endif
#endif





