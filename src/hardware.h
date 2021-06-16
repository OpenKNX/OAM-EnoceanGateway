#pragma once

// MUst be updated to the new Project HW !!!
#define PROG_LED_PIN 10
#define PROG_LED_PIN_ACTIVE_ON HIGH
#define PROG_BUTTON_PIN 8
#define PROG_BUTTON_PIN_INTERRUPT_ON FALLING
//#define SAVE_INTERRUPT_PIN A2 
//#define LED_YELLOW_PIN 0


/* Define Arduino Board */
/* ******************** */
//#define ArduinoZero
//#define ItsyBitsyM0
#define Xiao

//Debug Outputs
#define KDEBUG_min
#define KDEBUG

#ifdef KDEBUG
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





