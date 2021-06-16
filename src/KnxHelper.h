#pragma once

#include <knx.h>

// That is not a final list. In case you need more dpt types, you can/must add this dpt to this list of defines
// enum supported dpt
#define VAL_DPT_1 0
#define VAL_DPT_2 1
#define VAL_DPT_5 2
#define VAL_DPT_5001 3
#define VAL_DPT_6 4
#define VAL_DPT_7 5
#define VAL_DPT_8 6
#define VAL_DPT_9 7
#define VAL_DPT_16 8
#define VAL_DPT_17 9
#define VAL_DPT_232 10
#define VAL_DPT_10 11   // Time
#define VAL_DPT_11 12   // Date
#define VAL_DPT_3_7 13  // Dimming 
#define VAL_DPT_3_8 14  // Blinds
#define VAL_DPT_9_4 15  // LUX
#define VAL_DPT_1_19 16  // Door Window
#define VAL_DPT_9_20 17  // Bat Spannung
#define VAL_DPT_1_18 18  // Bat Spannung
#define VAL_DPT_9_8  19  // CO2 / VOC
#define VAL_DPT_9_7  20  // Hum
#define VAL_DPT_9_1  21  // Temp


Dpt &getDPT(uint8_t iDptIndex);
bool delayCheck(uint32_t iOldTimer, uint32_t iDuration);

uint16_t getPar(uint16_t PAR, uint8_t CH);
uint16_t getCom(uint16_t COM, uint8_t CH);