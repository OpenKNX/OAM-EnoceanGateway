/*
   EnOcean.h

*/

#ifndef ENOCEAN_H_
#define ENOCEAN_H_

#include "Arduino.h"
//#include "DebugUtil.h"
#include "OpenKNX.h"
// #include "hardware.h"
// #include <knx.h>

#ifndef EnOceanTEST
// important to change this to the number of devices defined.
#define MAX_NUMBER_OF_DEVICES 30
#else
#define MAX_NUMBER_OF_DEVICES 1
#endif

#define SIMULATE_NOTHING 0
#define SIMULATE_PUSH 1
#define SIMULATE_RELEASE 2
#define SIMULATE_PAUSE_BEFORE_RELEASE 100

#define ENOCEAN_OK 0
#define ENOCEAN_OUT_OF_RANGE 21
#define ENOCEAN_NOT_VALID_CHKSUM 7
#define ENOCEAN_NO_RX_TEL 6
#define ENOCEAN_NEW_RX_BYTE 3

/*
  	Packet type
*/

#define u8RADIO_ERP1 0x01
#define u8RESPONSE 0x02

/*
  	EEP type
*/

// Receive
#define u8RORG_1BS 0xD5    // 213
#define u8RORG_RPS 0xF6    // 246
#define u8RORG_VLD 0xD2    // 210
#define u8RORG_4BS 0xA5    // 165
#define u8RORG_Rocker 0xFA // 250

// Send
#define u8RORG_COMMON_COMMAND 0x05
#define u8KNX_RORG_Rocker 1
#define u8KNX_RORG_VLD 2
#define u8KNX_RORG_4BS 3

// COMANDS

#define u8CO_WR_IDBASE 0x07
#define u8CO_RD_IDBASE 0x08
#define u8CO_WR_REPEATER 0x09
#define u8CO_RD_REPEATER 0x0A // 10

#define RPS_BUTTON_CHANNEL_AI 0
#define RPS_BUTTON_CHANNEL_AO 1
#define RPS_BUTTON_CHANNEL_BI 2
#define RPS_BUTTON_CHANNEL_BO 3

#define RPS_BUTTON_2NDACT_NO 0
#define RPS_BUTTON_2NDACT_VALID 1

#define VLD_CMD_ID_01 0x01
#define VLD_CMD_ID_02 0x02
#define VLD_CMD_ID_03 0x03
#define VLD_CMD_ID_04 0x04
#define VLD_CMD_ID_05 0x05
#define VLD_CMD_ID_06 0x06

#define RPS_Func_10 0xA

#define BASEID_BYTES 4
#define DATBUF_SZ 100

#define SER_SYNCH_CODE 0x55
#define SER_HEADER_NR_BYTES 4

//Rocker States
#define ButtonStateO 0
#define ButtonStateI 1

#define RockerIdle 0x01
#define AI_pressed 0x10
#define AO_pressed 0x30
#define BI_pressed 0x50
#define BO_pressed 0x70
#define CI_pressed 0x15
#define CO_pressed 0x37
#define Contact_pressed 0xF0

#define AI_release 0x00
#define AO_release 0x20
#define BI_release 0x40
#define BO_release 0x60
#define CI_release 0x05
#define CO_release 0x07
#define Contact_release 0xE0



#define ROCKER_INACTIVE 0x00
#define ROCKER_AI 0x01
#define ROCKER_AO 0x02
#define ROCKER_BI 0x03
#define ROCKER_BO 0x04
#define ROCKER_CI 0x05
#define ROCKER_CO 0x06

typedef enum
{
  idle,
  checkShortLong,
  waitShortRelease,
  waitLongRelease,
} RockerStates;

/*
struct VLD_D2_04_00_TELEGRAM_CMD_SENRESP_TYPE
{
  uint8_t DB_Bit7 
  ...   
  uint8_t DB_Bit0 
};
*/

struct ONEBS_TEL_DATA_TYPE
{
  uint8_t State : 1; // (DB_BIT 0)   open 1: closed
  uint8_t NA : 7;    // (DB_BIT 7-1) not used
};

struct ONEBS_TEL_DATA_TYPE_V2
{
  uint8_t NA  : 4;  // (DB_BIT 3-0) NA
  uint8_t LRN : 1;  // (DB_BIT 4)   LRN Bit
  uint8_t NA1 : 3;  // (DB_BIT 7-5) NA
};

struct RPS_TEL_DATA_TYPE
{
  uint8_t SA : 1; // (DB_BIT 0)   No 2nd action; 1: Valid second action
  uint8_t R2 : 3; // (DB_BIT 3-2) Rocker second action.
  uint8_t EB : 1; // (DB_BIT 4)   Released; 1: Pressed
  uint8_t R1 : 3; // (DB_BIT 7-5) Rocker first action. See
};

struct RPS_F6_10_00_DATA_TYPE
{
  uint8_t NA : 4;    // (DB_BIT 3-0) Not use XXXX
  uint8_t STATE : 4; // (DB_BIT 7-4) Status
};

struct RPS_F6_10_01_DATA_TYPE
{
  uint8_t NA2 : 1;    // (DB_BIT 0)   Not used
  uint8_t HANDLE : 1; // (DB_BIT 1)   Handle
  uint8_t NA1 : 2;    // (DB_BIT 3-2) Not used
  uint8_t STATE : 2;  // (DB_BIT 5-4) Status
  uint8_t NA : 2;     // (DB_BIT 7-6) only 0x11
};

struct FOURBS_A5_06_01_DATA_TYPE // good?
{
  uint8_t NA2 : 4; // (DB_BIT 3-0) NA
  uint8_t LRN : 1; // (DB_BIT 4)   LRN Bit
  uint8_t NA : 2;  // (DB_BIT 6-5) NA
  uint8_t RS : 1;  // (DB_BIT 7)   Range Select
};

struct FOURBS_A5_06_03_DATA_TYPE // Good
{
  uint8_t Na : 6;  // (DB_BIT 5-0) NA
  uint8_t LUX : 2; // (DB_BIT 7-6) LUX
};

struct FOURBS_A5_07_01_DATA_TYPE
{
  uint8_t AVAILABLE_BAT : 1; // (DB_BIT 0)   Available Bat voltage Status
  uint8_t NA : 2;            // (DB_BIT 2-1) NA
  uint8_t LRN : 1;           // (DB_BIT 3)   LRN Bit
  uint8_t NA2 : 4;           // (DB_BIT 7-4) NA
};

struct FOURBS_A5_07_02_DATA_TYPE
{
  uint8_t Na : 7;  // (DB_BIT 6-0) Not used
  uint8_t PIR : 1; // (DB_BIT 7)   PIR
};

struct FOURBS_A5_07_03_DATA_TYPE
{
  uint8_t Na : 6;  // (DB_BIT 5-0) NA
  uint8_t LUX : 2; // (DB_BIT 7-6) LUX
};

struct FOURBS_A5_07_03_DATA_TYPE2
{
  uint8_t Na : 7;  // (DB_BIT 6-0) Not used
  uint8_t PIR : 1; // (DB_BIT 7)   PIR
};

struct FOURBS_A5_08_DATA_TYPE
{
  uint8_t OCC : 1;  // (DB_BIT 0) Occupancy Button
  uint8_t PIR : 1;  // (DB_BIT 1)   PIR
  uint8_t NA1 : 1;  // (DB_BIT 2)   Not used
  uint8_t LRNB : 1; // (DB_BIT 3)   Teach-In Bit
  uint8_t NA2 : 4;  // (DB_BIT 4-7) Not used
};

struct FOURBS_A5_12_01_DATA_TYPE
{
  uint8_t DIV : 2;  // (DB_BIT 0-1) Divisor
  uint8_t DT : 1;   // (DB_BIT 2)   Unit
  uint8_t LRNB : 1; // (DB_BIT 3)   Teach-In Bit
  uint8_t TI : 4;  // (DB_BIT 4-7)  Tarif
};

struct FOURBS_A5_07_14_01_06_DATA_TYPE
{
  uint8_t CT : 1;  // (DB_BIT 0)    Contact
  uint8_t VIB : 1; // (DB_BIT 1)    Vibration Detection
  uint8_t Na : 1;  // (DB_BIT 2)    Not used
  uint8_t LR : 1;  // (DB_BIT 3)    LR
  uint8_t NA : 4;  // (DB_BIT 4-7)  not used
};

struct FOURBS_A5_07_14_07_08_DATA_TYPE
{
  uint8_t VIB : 1; // (DB_BIT 0)    Vibration Detection
  uint8_t LCT : 1; // (DB_BIT 1)    Lock contact
  uint8_t DCT : 1; // (DB_BIT 2)    Door contact
  uint8_t LR : 1;  // (DB_BIT 3)    LR
  uint8_t NA : 4;  // (DB_BIT 4-7)  not used
};

struct FOURBS_A5_07_14_09_0A_DATA_TYPE
{
  uint8_t VIB : 1; // (DB_BIT 0)    Vibration Detection
  uint8_t CT : 2;  // (DB_BIT 1 -2) contact
  uint8_t LR : 1;  // (DB_BIT 3)    LR
  uint8_t NA : 4;  // (DB_BIT 4-7)  not used
};

struct FOURBS_A5_20_06_DATA_TYPE
{
  uint8_t Offset : 7; // (DB_BIT 0-6)  Offset
  uint8_t LOM : 1;    // (DB_BIT 7)    LOM (local offset mode)
};

struct FOURBS_A5_20_01_DATA_TYPE
{
  uint8_t NA : 3;        // (DB_BIT 0-2)   NA
  uint8_t LRNstatus : 1; // (DB_BIT 3)     Teachin
  uint8_t NA1 : 3;       // (DB_BIT 4-6)   NA
  uint8_t LRNtype : 1;   // (DB_BIT 7)     LRNTYpe
};

struct FOURBS_A5_20_04_DATA_TYPE
{
  uint8_t FL : 1;           // (DB_BIT 0)     FL
  uint8_t TS : 1;           // (DB_BIT 1)     TTS
  uint8_t BLS : 1;          // (DB_BIT 2)     BLS
  uint8_t LRNstatus : 1;    // (DB_BIT 3)     LRN Status
  uint8_t NA : 2;           // (DB_BIT 4-5)   NA
  uint8_t STR : 1;          // (DB_BIT 6)     STR
  uint8_t MSTLRNtype : 1;   // (DB_BIT 7)     MST & Teachin
};


/*
struct 
{
  uint8_t DB_Bit0 | Arduino Bit7  
  ...   
  uint8_t DB_Bit7 | Arduino Bit0   
};
*/

struct VLD_TEL_MEASUREMENT_TYPE
{
  uint8_t type : 3;  // NOT USED
  uint8_t dummy : 5; //
};

struct VLD_D2_01_TELEGRAM_CMD_03_ACTS_TYPE
{
  uint8_t outputCH : 5; // (DB_BIT 4-0) Channel
  uint8_t notused : 3;  // (DB_BIT 7-5) NOT USED
};

struct VLD_D2_01_TELEGRAM_CMD_04_ACTRESP_TYPE
{
  uint8_t outputValue : 7; // (DB_BIT 6-0) Output Value
  uint8_t LC : 1;          // (DB_BIT 7)   Load control
};

struct VLD_D2_01_TELEGRAM2_CMD_04_ACTRESP_TYPE
{
  uint8_t IOChannel : 5; // (DB_BIT 4-0) I/O Channel
  uint8_t EL : 2;        // (DB_BIT 6-5) Error Level
  uint8_t OC : 1;        // (DB_BIT 7)   Overcurrent
};

struct VLD_D2_01_TELEGRAM_DB_4_CMD_07_ACTRESP_TYPE
{
  uint8_t IO_Channel : 5;    // (DB_BIT 4-0) IO-Channel
  uint8_t UNIT : 3;          // (DB_BIT 5-7) UNIT
};

struct VLD_D2_04_00_TELEGRAM_CMD_SENRESP_TYPE
{
  uint8_t nouse : 4;    // (DB_BIT 0-3) not use
  uint8_t batt : 3;     // (DB_BIT 4-6) Battery autonomy
  uint8_t daynight : 1; // (DB_BIT 7)   Day / Night
};

struct VLD_D2_05_TELEGRAM_CMD_04_ACTRESP_TYPE
{
  uint16_t CMD : 4;       // (DB_BIT 3-0) CMD = 0x4
  uint16_t IOChannel : 4; // (DB_BIT 7-4) Channel
  uint16_t LM : 3;        // (DB_BIT 2-0) locking MOde
  uint16_t NA : 5;        // (DB_BIT 7-3) NOT USED
};

struct VLD_D2_06_Date_TYPE_Alarm
{
  uint8_t PPAL  : 4;      // (DB_BIT 3-0) Protection Plus Alarm
  uint8_t BAL   : 4;      // (DB_BIT 7-4) Burglary Alarm
};

struct VLD_D2_06_Date_TYPE_Window
{
  uint8_t WS  : 4;      // (DB_BIT 3-0) Window State
  uint8_t HP  : 4;      // (DB_BIT 7-4) Position Handle
};

struct VLD_D2_06_Date_TYPE_Buttons
{
  uint8_t BL  : 4;      // (DB_BIT 3-0) Button Left
  uint8_t BR  : 4;      // (DB_BIT 7-4) Button Right
};

struct VLD_D2_06_Date_TYPE_Motion_Mode
{
  uint8_t V  : 4;      // (DB_BIT 3-0) Vacation-Mode
  uint8_t M  : 4;      // (DB_BIT 7-4) Motion
};

struct VLD_D2_06_Date_TYPE_Battery
{
  uint8_t NA : 3;      // (DB_BIT 2-0) not used
  uint8_t BS : 5;      // (DB_BIT 7-3) Battery State
};


struct VLD_D2_06_50_TYPE_Window
{
  uint8_t WDS : 7;      // (DB_BIT 0-6) WDS
  uint8_t AM  : 1;      // (DB_BIT 7)   AM
};

struct VLD_D2_06_50_TYPE_Battery
{
  uint8_t BS  : 7;      // (DB_BIT 0-6)  BS
  uint8_t CB  : 1;      // (DB_BIT 7)   CB
};

struct VLD_D2_06_50_TYPE_Alarm
{
  uint8_t BA  : 7;      // (DB_BIT 0-3)  BA
  uint8_t NA  : 1;      // (DB_BIT 7-4)  Not used
};

struct VLD_D2_14_30_Data_TYPE
{
  uint8_t VIB : 4;     // (DB_BIT 0-3)  not used
  uint8_t IAQTH : 3;   // (DB_BIT 4-6)  IAQTH
  uint8_t HCI_LSB : 1; // (DB_BIT 7)    HCI
};

struct VLD_D2_14_30_Data_TYPE1
{
  uint8_t HCI_MSB : 1; // (DB_BIT 0)    HCI
  uint8_t HUM_LSB : 7; // (DB_BIT 1-7)  HUM
};

struct VLD_D2_14_30_Data_TYPE2
{
  uint8_t HUM_MSB : 1;  // (DB_BIT 0)    HUM
  uint8_t Temp_LSB : 7; // (DB_BIT 1-7) Temp
};

struct VLD_D2_14_30_Data_TYPE3
{
  uint8_t Temp_MSB : 1; // (DB_BIT 0)    Temp
  uint8_t RPLT_LSB : 7; // (DB_BIT 1-7)  RPLT
};

struct VLD_D2_14_30_Data_TYPE4
{
  uint8_t RPLT_MSB : 1;      // (DB_BIT 0)    EPLT
  uint8_t ES : 2;            // (DB_BIT 1-2)  ES
  uint8_t LastEvent_LSB : 5; // (DB_BIT 3-7)  LastEvent Bits
};

struct VLD_D2_14_30_Data_TYPE5
{
  uint8_t LastEvent_MSB : 3; // (DB_BIT 0-2)  LastEvent Bits
  uint8_t statusbits : 4;    // (DB_BIT 3-6)  Status Bits
  uint8_t smokeAlarm : 1;    // (DB_BIT 7)    Smoke Alarm
};


struct VLD_D2_14_41_Data_TYPE8
{
  uint8_t Temp_MSB : 8;  // (DB_BIT 0-7)    Temp_MSB
};

struct VLD_D2_14_41_Data_TYPE7
{
  uint8_t Hum_MSB : 6;  // (DB_BIT 0-5)   HUM MSB
  uint8_t Temp_LSB : 2;  // (DB_BIT 6-7)  Temp_LSB
};

struct VLD_D2_14_41_Data_TYPE6
{
  uint8_t LUX_MSB : 6;  // (DB_BIT 0-5)   LUX MSB
  uint8_t Hum_LSB : 2;  // (DB_BIT 6-7)   Hum_LSB
};

struct VLD_D2_14_41_Data_TYPE5
{
  uint8_t Lux : 8;  // (DB_BIT 0-7)    Lux
};

struct VLD_D2_14_41_Data_TYPE4
{
  uint8_t ACCX_MSB : 3;      // (DB_BIT 0-1)  not ACCY_LSB
  uint8_t SensorStatus : 2;  // (DB_BIT 2-3)  SensorStatus
  uint8_t LUX_LSB : 3;       // (DB_BIT 4-7)  LUX_LSB
};

struct VLD_D2_14_41_Data_TYPE3
{
  uint8_t ACCY_MSB : 1;  // (DB_BIT 0)    ACC_X MSB
  uint8_t ACCX_LSB : 7;  // (DB_BIT 1-7)    ACC_X LSB
};

struct VLD_D2_14_41_Data_TYPE2
{
  uint8_t ACCY_MID : 8;  // (DB_BIT 0-7)    ACC_Y MID
};

struct VLD_D2_14_41_Data_TYPE1
{
  uint8_t ACCZ_MSB : 7;  // (DB_BIT 0-6)    ACC_Z MSB
  uint8_t ACCY_LSB : 1;   // (DB_BIT 7)   ACCY_LSB
};

struct VLD_D2_14_41_Data_TYPE
{
  uint8_t NA : 4;       // (DB_BIT 0-3)  not used
  uint8_t contact : 1;  // (DB_BIT 4)    Contcat
  uint8_t ACCZ_LSB : 3; // (DB_BIT 5-7)  ACC_Z LSB
};

struct VLD_D2_01_TELEGRAM_CMD_03_TYPE
{
  VLD_D2_01_TELEGRAM_CMD_03_ACTS_TYPE u8VldTelActSta;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
  uint8_t u8SubTelegramm;
  uint8_t u8DestinationId_p[4];
};

struct VLD_D2_01_TELEGRAM_CMD_04_TYPE
{
  VLD_D2_01_TELEGRAM_CMD_04_ACTRESP_TYPE u8VldTelActResp;
  VLD_D2_01_TELEGRAM2_CMD_04_ACTRESP_TYPE u8VldTelActResp2;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct VLD_D2_01_TELEGRAM_CMD_07_TYPE
{
  VLD_D2_01_TELEGRAM_DB_4_CMD_07_ACTRESP_TYPE u8VldTelActResp;
  uint32_t u32MeasurementValue;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct VLD_D2_04_00_TELEGRAM
{
  uint8_t voc;
  uint8_t hum;
  uint8_t temp;
  VLD_D2_04_00_TELEGRAM_CMD_SENRESP_TYPE u8VldTelSenSta;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct VLD_D2_03_0A_TELEGRAM
{
  uint8_t bat;
  uint8_t value;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct VLD_D2_05_00_TELEGRAM_CMD_04_TYPE
{
  uint8_t pos;
  uint8_t angle;
  VLD_D2_05_TELEGRAM_CMD_04_ACTRESP_TYPE u8VldTelSenSta;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct VLD_D2_06_01_TELEGRAM
{
  uint8_t u8MT;             // Message Type
  VLD_D2_06_Date_TYPE_Alarm u8VldTelAlarm;
  VLD_D2_06_Date_TYPE_Window u8VldTelWindow;            
  VLD_D2_06_Date_TYPE_Buttons u8VldTelButton;
  VLD_D2_06_Date_TYPE_Motion_Mode u8VldTelMotionMode;
  uint8_t u8Temp;    
  uint8_t u8Hum;  
  uint8_t u8LuxMSB;    
  uint8_t u8LuxLSB;     
  VLD_D2_06_Date_TYPE_Battery u8VldTelBat;   
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct VLD_D2_06_50_Window_Status_0x01
{
  uint8_t u8MT;             // Message Type
  VLD_D2_06_50_TYPE_Window u8VldTelWindow;  
  uint8_t u8CT_4;
  uint8_t u8CT_3;
  uint8_t u8CT_2;
  uint8_t u8CT_1;
  VLD_D2_06_50_TYPE_Battery u8VldTelBattery;  
  uint8_t u8StatusBits; 
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct VLD_D2_06_50_Alarm_Status_0x02
{
  uint8_t u8MT;             // Message Type
  VLD_D2_06_50_TYPE_Alarm u8VldTelAlarm;  
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct VLD_D2_14_00_TELEGRAM
{
  uint8_t u8B1;
  uint8_t u8B2;
  uint8_t u8B3;
  uint8_t u8B4;
  uint8_t u8B5;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct VLD_D2_14_30_TELEGRAM
{
  VLD_D2_14_30_Data_TYPE5 u8VldTelSenSta5;
  VLD_D2_14_30_Data_TYPE4 u8VldTelSenSta4;
  VLD_D2_14_30_Data_TYPE3 u8VldTelSenSta3;
  VLD_D2_14_30_Data_TYPE2 u8VldTelSenSta2;
  VLD_D2_14_30_Data_TYPE1 u8VldTelSenSta1;
  VLD_D2_14_30_Data_TYPE u8VldTelSenSta;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct VLD_D2_14_41_TELEGRAM
{
  uint8_t Temp_MSB;
  VLD_D2_14_41_Data_TYPE7 u8VldTelSenSta7;
  VLD_D2_14_41_Data_TYPE6 u8VldTelSenSta6;
  uint8_t LUX;
  VLD_D2_14_41_Data_TYPE4 u8VldTelSenSta4;
  VLD_D2_14_41_Data_TYPE3 u8VldTelSenSta3;
  VLD_D2_14_41_Data_TYPE2 u8VldTelSenSta2;
  VLD_D2_14_41_Data_TYPE1 u8VldTelSenSta1;
  VLD_D2_14_41_Data_TYPE u8VldTelSenSta;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct RPS_TELEGRAM_TYPE
{
  RPS_TEL_DATA_TYPE u8RpsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct RPS_F6_10_00_TYPE
{
  RPS_F6_10_00_DATA_TYPE u8RpsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct RPS_F6_10_01_TYPE
{
  RPS_F6_10_01_DATA_TYPE u8RpsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_02_TYPE
{
  uint8_t NA;
  uint8_t NA1;
  uint8_t Temp;
  uint8_t NA2;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_02_2030TYPE
{
  uint8_t NA;
  uint8_t TempMSB;
  uint8_t TempLSB;
  uint8_t NA2;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_04_TYPE
{
  uint8_t NA;
  uint8_t Hum;
  uint8_t Temp;
  uint8_t NA2;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_04_03_TYPE
{
  uint8_t Hum;
  uint8_t TempMSB;
  uint8_t TempLSB;
  uint8_t NA2;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_06_01_TYPE
{
  uint8_t u8SupplyVoltage;
  uint8_t u8Illumination1;
  uint8_t u8Illumination2;
  FOURBS_A5_06_01_DATA_TYPE u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_06_01_V2_TYPE
{
  uint8_t u8Illumbyte3;
  uint8_t u8Illumbyte2;
  uint8_t u8byte1;
  uint8_t u8byte0;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_06_03_TYPE
{
  uint8_t u8SupplyVoltage;
  uint8_t u8LuxMSB;
  FOURBS_A5_06_03_DATA_TYPE u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_07_01_TYPE
{
  uint8_t u8SupplyVoltage;
  uint8_t Free;
  uint8_t PIR;
  FOURBS_A5_07_01_DATA_TYPE u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_07_02_TYPE
{
  uint8_t u8SupplyVoltage;
  uint8_t u8Illumination1;
  uint8_t u8Illumination2;
  FOURBS_A5_07_02_DATA_TYPE u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_07_03_TYPE
{
  uint8_t u8SupplyVoltage;
  uint8_t u8LuxMSB;
  FOURBS_A5_07_03_DATA_TYPE u84BsTelData2;
  FOURBS_A5_07_03_DATA_TYPE2 u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_08_TYPE
{
  uint8_t u8SupplyVoltage;
  uint8_t Ill;
  uint8_t TMP;
  FOURBS_A5_08_DATA_TYPE u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_12_01_TYPE
{
  uint8_t Meterreading_MSB;
  uint8_t Meterreading_MID;
  uint8_t Meterreading_LSB;
  FOURBS_A5_12_01_DATA_TYPE u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_14_01_06_TYPE
{
  uint8_t u8SupplyVoltage;
  uint8_t u8Lux;
  uint8_t u8Free;
  FOURBS_A5_07_14_01_06_DATA_TYPE u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_14_07_08_TYPE
{
  uint8_t u8SupplyVoltage;
  uint8_t u8Free;
  uint8_t u8Free2;
  FOURBS_A5_07_14_07_08_DATA_TYPE u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_14_09_0A_TYPE
{
  uint8_t u8SupplyVoltage;
  uint8_t u8Free;
  uint8_t u8Free2;
  FOURBS_A5_07_14_09_0A_DATA_TYPE u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_20_01_TYPE
{
  uint8_t u8CurrentPos;
  uint8_t u8StatusBits;
  uint8_t u8Temp;
  FOURBS_A5_20_01_DATA_TYPE u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_20_04_TYPE
{
  uint8_t u8CurrentPos;
  uint8_t u8Temp;
  uint8_t u8TempError;
  FOURBS_A5_20_04_DATA_TYPE u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct FOURBS_A5_20_06_TYPE
{
  uint8_t u8CurrentPos;
  FOURBS_A5_20_06_DATA_TYPE u84BsTelData;
  uint8_t u8Temp;
  uint8_t u8StatusBits;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

struct ONEBS_TELEGRAM_TYPE
{
  ONEBS_TEL_DATA_TYPE u81bsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};


struct ONEBS_TELEGRAM_TYPE_V2
{
  uint8_t u8EnergyVoltage;
  uint8_t u8BatteryVoltage;
  uint8_t free;
  ONEBS_TEL_DATA_TYPE_V2 u84BsTelData;
  uint8_t u8SenderId_p[4];
  uint8_t u8Status;
};

//! uart_getPacket state machine states.
enum STATES_GET_PACKET
{
  //! Waiting for the synchronisation byte 0x55
  GET_SYNC_STATE = 0,
  //! Copying the 4 after sync byte: raw data length (2 bytes), optional data length (1), type (1).
  GET_HEADER_STATE,
  //! Checking the header CRC8 checksum. Resynchronisation test is also done here
  CHECK_CRC8H_STATE,
  //! Copying the data and optional data bytes to the paquet buffer
  GET_DATA_STATE,
  //! Checking the info CRC8 checksum.
  CHECK_CRC8D_STATE,
};

//! Packet structure (ESP3)
struct PACKET_SERIAL_TYPE
{
  // Amount of raw data bytes to be received. The most significant byte is sent/received first
  uint16_t u16DataLength;
  // Amount of optional data bytes to be received
  uint8_t u8OptionLength;
  // Packetype code
  uint8_t u8Type;
  // Data buffer: raw data + optional bytes
  uint8_t *u8DataBuffer;
};

class IEnOceanDevice
{
protected:
  uint16_t firstComObj = 0;
  uint16_t firstParameter = 0;
  uint8_t index = 0;

public:
  uint8_t numberOfComObjects;
  uint8_t numberOfParameters;

  
  virtual ~IEnOceanDevice()
  {
  }
  virtual void init(uint16_t startAtComObj, uint16_t startAtParameter, uint8_t channel) = 0;
  virtual void initBaseID(uint8_t channel, uint8_t BaseID1, uint8_t BaseID2, uint8_t BaseID3, uint8_t BaseID4) = 0;
  virtual void task() = 0;
  virtual bool handleEnOceanPacket(PACKET_SERIAL_TYPE *f_Pkt_st) = 0;
  virtual void handleKnxEvents(byte koIndex, byte koNr, GroupObject &iKo) = 0;
};



class EnOcean: public OpenKNX::Module
{
  public:

  EnOcean();
  virtual ~EnOcean();


  //static EnOcean Eno;

  void initSerial(Stream &serial);
  void init2();
  void task();
  static void taskCallback(void *iThis);

  bool sendPacket(PACKET_SERIAL_TYPE *pPacket);
  void obtainSenderId(uint8_t *senderId, uint8_t channel);
  void handleKnxEvents(byte koIndex, byte koNr, GroupObject &iKo);
  void configureDeviceBaseID(IEnOceanDevice &device, uint8_t channel);
  void configureDevice(IEnOceanDevice &device, uint8_t channel);
  uint16_t getNumberDevices();
  uint8_t *getBaseId();
  uint8_t getBaseId_Byte4();

  // knx module handling
  void processInputKo(GroupObject& iKo);
  void setup();
  void loop();
  const std::string version() override;
  const std::string name() override;

private:
  Stream *_serial;
  bool isInited;

  uint8_t u8CRC;
  uint8_t u8RxByte;
  uint8_t u8RetVal;

  uint16_t lastParam;
  uint16_t lastComObj;
  uint8_t lastDevice;
  uint8_t lastSenderIdOffset = 0;

  uint8_t u8datBuf[DATBUF_SZ];

  IEnOceanDevice *deviceRegistry[MAX_NUMBER_OF_DEVICES]{NULL};

  STATES_GET_PACKET u8State;
  PACKET_SERIAL_TYPE m_Pkt_st;

  static uint8_t u8CRC8Table[256];

  uint8_t lui8_BaseID_p[BASEID_BYTES];

  void readBaseId(uint8_t *fui8_BaseID_p);
  void setBaseId(uint8_t *fui8_BaseID_p);
  void setRepeaterFunc();
  void readRepeaterFunc();
  void getEnOceanMSG(uint8_t u8RetVal, PACKET_SERIAL_TYPE *f_Pkt_st);
  bool checkBaseID();

  uint8_t uart_getPacket(PACKET_SERIAL_TYPE *pPacket, uint16_t u16BufferLength);
  uint8_t uart_sendPacket(PACKET_SERIAL_TYPE *pPacket);
};
extern EnOcean enOcean;

#endif /* ENOCEAN_H_ */