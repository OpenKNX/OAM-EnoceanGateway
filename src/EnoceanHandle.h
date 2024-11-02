
#ifndef EnOceanDevice_H_
#define EnOceanDevice_H_

#include "EnOcean.h"
#include "EnoceanSendFunctions.h"
#include "RPS_Telegram.h"
#include "4BS_Telegram.h"
#include "VLD_Telegram.h"
#include "1BS_Telegram.h"
#include "hardware.h"
// #include "Helper.h"
#include "KnxHelper.h"

// ################################################
// ### DEBUG CONFIGURATION
// ################################################

class EnOceanDevice : public IEnOceanDevice
{
private:
  uint8_t lui8_SendeID_p[4] = {0x0A, 0x0B, 0x0C, 0x0D};
  uint8_t deviceId_Arr[4] = {0, 0, 0, 0};
  uint32_t deviceId = 0;
  uint8_t profil = 0;
  uint8_t profil2nd = 0;

  uint8_t buttonStateSimulation1 = SIMULATE_NOTHING;
  uint8_t buttonStateSimulation2 = SIMULATE_NOTHING;
  bool buttonMessage1 = false;
  bool buttonMessage2 = false;

  RockerStates state = idle;

  // uint32_t rocker_longpress_delay2;

  union EnoceanHandle
  {
    uint8_t msg_sent_after_receive;           // Für MVA-005  A5-20-06
    uint8_t rockerState_pressed = RockerIdle; // ROCKER
  } unionMSG;

  union TaskHandle1
  {
    uint8_t val_A5_20_06[4];         // Für MVA-005  A5-20-06
    uint8_t val_A5_20_04[4];         // Für MVA-005  A5-20-04
    uint8_t val_A5_20_01[4];         // Für MVA-005  A5-20-01
    uint32_t rocker_longpress_delay; // ROCKER
    uint32_t buttonLastPushTime1;    // ROCKER Senden
    uint8_t val_D2_01_0E_Energy[5];  // Für D2-01-0E parameter Energy
  } union1;

  union TaskHandle2
  {
    byte val_A5_20_01_TeachIn[4];             // Für MVA-005  A5-20-01
    byte val_A5_20_04_TeachIn[4];             // Für MVA-005  A5-20-04
    byte val_A5_20_06_TeachIn[4];             // Für MVA-005  A5-20-06
    uint32_t buttonLastPushTime2;             // ROCKER Senden
    uint8_t rockerState_Release = RockerIdle; // ROCKER
    uint8_t val_D2_01_0E_Power[5];            // Für D2-01-0E parameter Power
  } union2;

  union TaskHandle3
  {
    uint8_t rockerNr; // ROCKER
    uint8_t A52001_CMD;
    uint8_t A52004;
    uint8_t D2_01_0E_SetActor; // D2-01-0E setup Actor settings for measurments
  } union3;

  // Task variable Read Request
  uint8_t sCalled = 1;
  uint32_t gStartupDelay = 0;

public:
  EnOceanDevice()
  {
    // numberOfComObjects = 4;
    // numberOfParameters = 2;
  }

  //*************************************************************************************************************************************************************
  //*************************************************************************************************************************************************************
  //
  //   initBaseId()        // things that need to be done regularly
  //
  //*************************************************************************************************************************************************************
  //*************************************************************************************************************************************************************

  void initBaseID(uint8_t channel, uint8_t BaseID1, uint8_t BaseID2, uint8_t BaseID3, uint8_t BaseID4)
  {
    lui8_SendeID_p[0] = BaseID1;
    lui8_SendeID_p[1] = BaseID2;
    lui8_SendeID_p[2] = BaseID3;
    lui8_SendeID_p[3] = BaseID4 + (4 * channel);
    // ACHTUNG: es stehen nur 128 frei wählbare IDs zur verfügung.
    // Die 128 Stk können NICHT frei bestimmt werden -> Es gehen nur die 128 folgenden ID !!!
    // D.h. BaseID4 liegt default immer zwischen 0x00 und 0x80
    // Auf BaseID4 können dann maximal die 127 weiteren IDs gelegt werden.
    // -> 4 * Channels (20CH) = 80 --> OK

#ifdef KDEBUG_Sender_ID
    SERIAL_PORT.print("CH");
    SERIAL_PORT.print(channel);
    SERIAL_PORT.print(" SENDER_ID: ");
    for (int i = 0; i < BASEID_BYTES; i++)
    {
      SERIAL_PORT.print(lui8_SendeID_p[i], HEX);
      SERIAL_PORT.print("");
    }
    SERIAL_PORT.println("");
#endif
  }

  //*************************************************************************************************************************************************************
  //*************************************************************************************************************************************************************
  //
  //   init()        // things that need to be done regularly
  //
  //*************************************************************************************************************************************************************
  //*************************************************************************************************************************************************************
  void init(uint16_t startAtComObj, uint16_t startAtParameter, uint8_t channel)
  {
    // init(lastComObj, lastParam, channel);
    // lastComObj = ENO_KoOffset + (channel * ENO_KoBlockSize);
    // lastParam = ENO_ParamBlockOffset + (channel * ENO_ParamBlockSize);
    firstComObj = startAtComObj;
    firstParameter = startAtParameter;
    index = channel;
    // we only store the first comObj as this information is sufficient to identify all the com-objects

    // regarding configuration:
    deviceId = 4292662400; // nur zum Testen <<<<----------------------------------------------------
    deviceId = 2958397;    // TASTER
    deviceId_Arr[0] = deviceId >> 24;
    deviceId_Arr[1] = deviceId >> 16;
    deviceId_Arr[2] = deviceId >> 8;
    deviceId_Arr[3] = deviceId; // nur zum Testen <<<<----------------------------------------------------

    deviceId_Arr[0] = knx.paramByte(ENO_CHId0 + firstParameter);
    deviceId_Arr[1] = knx.paramByte(ENO_CHId2 + firstParameter);
    deviceId_Arr[2] = knx.paramByte(ENO_CHId4 + firstParameter);
    deviceId_Arr[3] = knx.paramByte(ENO_CHId6 + firstParameter);

    // init parameter
    unionMSG.msg_sent_after_receive = 0;

    // default Werte
    // ************** A5-20-01 **************************************************************************************
    if (knx.paramWord(ENO_CHProfil4BS20 + firstParameter) == A5_20_01)
    {
      union1.val_A5_20_01[0] = 0;                                                 // Value Pos = 0% or Temp-Selection = 0°C
      union1.val_A5_20_01[1] = 100;                                               // TMP Raum Temp = 40°C (value/2.5)
      union1.val_A5_20_01[2] = (knx.paramByte(ENO_CHA52001SPS + firstParameter)); // Ein Parameter reicht, da alles im UNION gespeichert wird.
      union1.val_A5_20_01[2] &= ~(1 << 0);                                        // SET RCU to "0"
      union1.val_A5_20_01[2] &= ~(1 << 1);                                        // SET SPN to "0"
      union1.val_A5_20_01[2] &= ~(1 << 3);                                        // SET SB  to "0"
      union1.val_A5_20_01[2] &= ~(1 << 4);                                        // SET CMD to "0"
      union1.val_A5_20_01[2] &= ~(1 << 5);                                        // SET CMD to "0"
      union1.val_A5_20_01[2] &= ~(1 << 6);                                        // SET CMD to "0"
      union1.val_A5_20_01[3] |= 1 << 3;                                           // Set LNRB
#ifdef KDEBUG
      SERIAL_PORT.print(F("SPS: "));
      SERIAL_PORT.println((union1.val_A5_20_01[2] & ENO_CHA52001SPSMask) >> ENO_CHA52001SPSShift);
      SERIAL_PORT.print(F("DB1: "));
      SERIAL_PORT.println(union1.val_A5_20_01[2], BIN);
#endif
    }
    // ************** A5-20-04 **************************************************************************************
    else if (knx.paramWord(ENO_CHProfil4BS20 + firstParameter) == A5_20_04)
    {
      union1.val_A5_20_04[0] = 0;   // Value Pos = 0%
      union1.val_A5_20_04[1] = 153; // TMP Raum Temp = 22°C (value-10)/0.078)
      union1.val_A5_20_04[2] = 0;
      union1.val_A5_20_04[2] = (knx.paramByte(ENO_CHA52004WCU + firstParameter)) << 1; // Ein Parameter reicht, da alles im UNION gespeichert wird.
      if ((knx.paramByte(ENO_CHA52004MC + firstParameter) >> ENO_CHA52004MCShift) & 1) // MC = 1 -> DISABLE
        union1.val_A5_20_04[2] |= 1 << 6;
      else // MC = 0 -> ENABLE
        union1.val_A5_20_04[2] &= ~(1 << 6);
      union1.val_A5_20_04[3] = (knx.paramByte(ENO_CHA52004DSO + firstParameter)); // Ein Parameter reicht, da alles im UNION gespeichert wird.
      union1.val_A5_20_04[3] |= 1 << 3;                                           // Set LNRB
      union1.val_A5_20_04[3] &= ~(1 << 2);                                        // Set BLC to "0"
      union1.val_A5_20_04[3] &= ~(1 << 1);                                        // Set SER to "0"
      union1.val_A5_20_04[3] &= ~(1 << 0);                                        // Set SER to "0"
#ifdef KDEBUG
      SERIAL_PORT.print(F("MC: "));
      SERIAL_PORT.println((union1.val_A5_20_04[2] & ENO_CHA52004MCMask) >> ENO_CHA52004MCShift);
      SERIAL_PORT.print(F("DSO: "));
      SERIAL_PORT.println((union1.val_A5_20_04[3] & ENO_CHA52004DSOMask) >> ENO_CHA52004DSOShift);
      SERIAL_PORT.print(F("WUC: "));
      SERIAL_PORT.println((union1.val_A5_20_04[2] & 0x7E) >> 1);
      SERIAL_PORT.println(union1.val_A5_20_04[2], BIN);
#endif
    }
    // ************** A5-20-06 **************************************************************************************
    else if (knx.paramWord(ENO_CHProfil4BS20 + firstParameter) == A5_20_06)
    {
      union1.val_A5_20_06[0] = 0;                                                 // Value Pos = 0% or Temp-Selection = 0°C
      union1.val_A5_20_06[1] = 160;                                               // TMP Raum Temp = 40°C (value/4.0)
      union1.val_A5_20_06[2] = (knx.paramByte(ENO_CHA52006RFC + firstParameter)); // Ein Parameter reicht, da alles im UNION gespeichert wird.
      union1.val_A5_20_06[3] |= 1 << 3;                                           // Set LNRB
#ifdef KDEBUG
      SERIAL_PORT.print(F("RFC: "));
      SERIAL_PORT.println((union1.val_A5_20_06[2] & ENO_CHA52006RFCMask) >> ENO_CHA52006RFCShift);
      SERIAL_PORT.print(F("SPS: "));
      SERIAL_PORT.println((union1.val_A5_20_06[2] & ENO_CHA52006SPSMask) >> ENO_CHA52006SPSShift);
      SERIAL_PORT.print(F("TSL: "));
      SERIAL_PORT.println((union1.val_A5_20_06[2] & ENO_CHA52006TSLMask) >> ENO_CHA52006TSLShift);
#endif
    }
    else if (knx.paramWord(ENO_CHProfilVLD01 + firstParameter) == D2_01_0E)
    {
      union3.D2_01_0E_SetActor = 2; // set for Task() function. Each run unionMSG.D2_01_0E_SetActor -1, after two runs unionMSG.D2_01_0E_SetActor = 0

      bool RM = (knx.paramByte(ENO_CHD2010ERM + firstParameter) >> ENO_CHD2010ERMShift) & 1;
      bool UNE = (knx.paramByte(ENO_CHD2010EUNE + firstParameter) >> ENO_CHD2010EUNEShift) & 1;
      bool UNP = (knx.paramByte(ENO_CHD2010EUNP + firstParameter) >> ENO_CHD2010EUNPShift) & 1;

      union1.val_D2_01_0E_Energy[0] = 0x1E;
      union2.val_D2_01_0E_Power[0] = 0x1E;
      union1.val_D2_01_0E_Energy[0] ^= (-RM ^ union1.val_D2_01_0E_Energy[0]) & (1 << 7);
      union2.val_D2_01_0E_Power[0] ^= (-RM ^ union2.val_D2_01_0E_Power[0]) & (1 << 7);

      // Energy
      union1.val_D2_01_0E_Energy[0] &= ~(1 << 5);
      // Power
      union2.val_D2_01_0E_Power[0] |= (1 << 5);

      // Power
      union2.val_D2_01_0E_Power[1] = (knx.paramWord(ENO_CHD2010EdeltaPw + firstParameter) >> 8); // maskiere LSB part of the 16bit value und schiebe ihn in ein 1Byte speicher
      union2.val_D2_01_0E_Power[1] = (union2.val_D2_01_0E_Power[1] << 4);                        // verschiebt den LSB-Wert auf die richtige Position
      if (UNP == 1)
        union2.val_D2_01_0E_Power[1] = union2.val_D2_01_0E_Power[1] + 4; // Eränzt noch die passende Unit (KW)
      else
        union2.val_D2_01_0E_Power[1] = union2.val_D2_01_0E_Power[1] + 3;                           // Eränzt noch die passende Unit (W)
      union2.val_D2_01_0E_Power[2] = (uint8_t)knx.paramWord(ENO_CHD2010EdeltaPw + firstParameter); // speichert den MSB Wert

      // Energy
      union1.val_D2_01_0E_Energy[1] = (knx.paramWord(ENO_CHD2010EdeltaEkwh + firstParameter) >> 8); // maskiere LSB part of the 16bit value und schiebe ihn in ein 1Byte speicher
      union1.val_D2_01_0E_Energy[1] = (union1.val_D2_01_0E_Energy[1] << 4);                         // verschiebt den LSB-Wert auf die richtige Position
      if (UNE == 1)
        union1.val_D2_01_0E_Energy[1] = union1.val_D2_01_0E_Energy[1] + 2; // Eränzt noch die passende Unit (KWH)
      else
        union1.val_D2_01_0E_Energy[1] = union1.val_D2_01_0E_Energy[1] + 1;                            // Eränzt noch die passende Unit (WH)
      union1.val_D2_01_0E_Energy[2] = (uint8_t)knx.paramWord(ENO_CHD2010EdeltaEkwh + firstParameter); // speichert den MSB Wert

      if ((knx.paramByte(ENO_CHD2010EMAT + firstParameter)) * 10 >= knx.paramByte(ENO_CHD2010EMIT + firstParameter)) // Check if Max >= MIN
      {
        union1.val_D2_01_0E_Energy[3] = knx.paramByte(ENO_CHD2010EMAT + firstParameter);
        union1.val_D2_01_0E_Energy[4] = knx.paramByte(ENO_CHD2010EMIT + firstParameter);
        union2.val_D2_01_0E_Power[3] = knx.paramByte(ENO_CHD2010EMAT + firstParameter);
        union2.val_D2_01_0E_Power[4] = knx.paramByte(ENO_CHD2010EMIT + firstParameter);
      }
      else
      {
        union1.val_D2_01_0E_Energy[4] = union1.val_D2_01_0E_Energy[3]; // When Max not >= Min -> set Min to max
        union2.val_D2_01_0E_Power[4] = union2.val_D2_01_0E_Power[3];   // When Max not >= Min -> set Min to max
      }

#ifdef KDEBUG
      SERIAL_PORT.println(knx.paramWord(ENO_CHD2010EdeltaEwh + firstParameter));
      SERIAL_PORT.println(knx.paramWord(ENO_CHD2010EdeltaPw + firstParameter));
      SERIAL_PORT.println(F("D2-01-0E"));
      SERIAL_PORT.print(F("RM: "));
      SERIAL_PORT.println(RM);
      SERIAL_PORT.print(F("UN_e: "));
      SERIAL_PORT.println((union1.val_D2_01_0E_Energy[1]) & 0x07);
      SERIAL_PORT.print(F("UN_p: "));
      SERIAL_PORT.println((union2.val_D2_01_0E_Power[1]) & 0x07);
#endif
    }
    // default Startup delay Startpunkt
    gStartupDelay = millis(); // Read Request Delay startup

    // profil = 45; // nur zum Testen <<<<----------------------------------------------------

#ifdef KDEBUG
    if (knx.configured())
    {
      SERIAL_PORT.print(F("I am: "));
      for (int i = 0; i < BASEID_BYTES; i++)
      {
        SERIAL_PORT.print(deviceId_Arr[i], HEX);
      }
      SERIAL_PORT.println(F(""));
      // SERIAL_PORT.println(ENO_CHProfilSelection);
      // SERIAL_PORT.println(firstParameter);
      // SERIAL_PORT.println(knx.paramByte(ENO_CHProfilSelection + firstParameter));

      switch (knx.paramByte(ENO_CHProfilSelection + firstParameter))
      // switch (u8RORG_Rocker)                                 // nur zum Testen <<<<----------------------------------------------------
      {
      case u8RORG_1BS:
        SERIAL_PORT.print(F("Profil: 1BS "));
        SERIAL_PORT.println(knx.paramWord(ENO_CHProfilSelection1BS + firstParameter));
        break;
      case u8RORG_4BS:
        SERIAL_PORT.print(F("Profil: 4BS "));
        SERIAL_PORT.println(knx.paramWord(ENO_CHProfilSelection4BS + firstParameter));
        break;
      case u8RORG_RPS:
        SERIAL_PORT.print(F("Profil: RPS "));
        SERIAL_PORT.println(knx.paramWord(ENO_CHProfilSelectionRPS + firstParameter));
        break;
      case u8RORG_VLD:
        SERIAL_PORT.print(F("Profil: VLD "));
        SERIAL_PORT.println(knx.paramWord(ENO_CHProfilSelectionVLD + firstParameter));
        break;
      case u8RORG_Rocker:
        SERIAL_PORT.print(F("Profil: Rocker Switch "));
        SERIAL_PORT.println(knx.paramByte(ENO_CHProfilSelection + firstParameter));
        break;
      default:
        SERIAL_PORT.println(F("Profil: ERROR"));
        break;
      }
    }
#endif
  }

  //*************************************************************************************************************************************************************
  //*************************************************************************************************************************************************************
  //
  //   Task ()        // things that need to be done regularly
  //
  //*************************************************************************************************************************************************************
  //*************************************************************************************************************************************************************
  void task()
  {
    // bool longPressDim = false;

    switch (knx.paramByte(ENO_CHProfilSelection + firstParameter))
    {
    case u8RORG_1BS:
      /* code */
      break;

    case u8RORG_4BS:
      // *************** Sent after receive ***********************************************************************
      // A5-20-01
      if (unionMSG.msg_sent_after_receive == msg_A5_20_01)
      {
        // Set LNRB
        union1.val_A5_20_01[3] |= 1 << 3; // Set LNRB
        send_4BS_Msg(lui8_SendeID_p, index, union1.val_A5_20_01, 0);
        // löscht Ref Run Bit
        union1.val_A5_20_01[2] &= ~(1 << 7); // clear Bit Ref RUN
        union1.val_A5_20_01[2] &= ~(1 << 6); // clear Bit Service CMD
        union1.val_A5_20_01[2] &= ~(1 << 5); // clear Bit Service CMD
        union1.val_A5_20_01[2] &= ~(1 << 4); // clear Bit Service CMD
        union1.val_A5_20_01[2] &= ~(1 << 3); // clear Bit SB
        union1.val_A5_20_01[2] &= ~(1 << 1); // clear Bit SPN
        unionMSG.msg_sent_after_receive = 0;
      }
      // *************** Teachin Funktion  ***********************************************************************
      else if (unionMSG.msg_sent_after_receive == TEACHIN_A52001)
      {
        union2.val_A5_20_01_TeachIn[0] = 0x80;
        union2.val_A5_20_01_TeachIn[1] = 0x08;
        union2.val_A5_20_01_TeachIn[2] = 0x49;
        union2.val_A5_20_01_TeachIn[3] = 0xF0;
        send_4BS_Msg(lui8_SendeID_p, index, union2.val_A5_20_01_TeachIn, 0);
#ifdef KDEBUG
        SERIAL_PORT.println(F("TeachIn Response Sent"));
#endif
        unionMSG.msg_sent_after_receive = 0;
      }
      //-----------------------------------------------------------------------------------------------------------
      // A5-20-04
      else if (unionMSG.msg_sent_after_receive == msg_A5_20_04)
      {
        // Set LNRB
        union1.val_A5_20_04[3] |= 1 << 3; // Set LNRB
        send_4BS_Msg(lui8_SendeID_p, index, union1.val_A5_20_04, 0);
        union1.val_A5_20_01[3] &= ~(1 << 0); // clear Bit Service CMD
        union1.val_A5_20_01[3] &= ~(1 << 1); // clear Bit Service CMD
        unionMSG.msg_sent_after_receive = 0;
      }
      // *************** Teachin Funktion  ***********************************************************************
      else if (unionMSG.msg_sent_after_receive == TEACHIN_A52004)
      {
        union2.val_A5_20_04_TeachIn[0] = 0x80;
        union2.val_A5_20_04_TeachIn[1] = 0x20;
        union2.val_A5_20_04_TeachIn[2] = 0x49;
        union2.val_A5_20_04_TeachIn[3] = 0xF0;
        send_4BS_Msg(lui8_SendeID_p, index, union2.val_A5_20_04_TeachIn, 0);
#ifdef KDEBUG
        SERIAL_PORT.println(F("TeachIn Response Sent"));
#endif
        unionMSG.msg_sent_after_receive = 0;
      }
      //-----------------------------------------------------------------------------------------------------------
      // A5-20-06
      else if (unionMSG.msg_sent_after_receive == msg_A5_20_06)
      {
        // Set LNRB
        union1.val_A5_20_06[3] |= 1 << 3; // Set LNRB
        send_4BS_Msg(lui8_SendeID_p, index, union1.val_A5_20_06, 0);
        // löscht Ref Run Bit
        union1.val_A5_20_06[2] &= ~(1 << 7); // clear Bit
        unionMSG.msg_sent_after_receive = 0;
      }
      // *************** Teachin Funktion  ***********************************************************************
      // A5-20-06
      else if (unionMSG.msg_sent_after_receive == TEACHIN_A52006)
      {
        union2.val_A5_20_06_TeachIn[0] = 0x80;
        union2.val_A5_20_06_TeachIn[1] = 0x30;
        union2.val_A5_20_06_TeachIn[2] = 0x49;
        union2.val_A5_20_06_TeachIn[3] = 0xF0;
        send_4BS_Msg(lui8_SendeID_p, index, union2.val_A5_20_06_TeachIn, 0);
#ifdef KDEBUG
        SERIAL_PORT.println(F("TeachIn Response Sent"));
#endif
        unionMSG.msg_sent_after_receive = 0;
      }
      // *************** Read Request  ***********************************************************************
      // A5-20-06
      if (knx.paramWord(ENO_CHProfil4BS20 + firstParameter) == A5_20_06 && sCalled < 255)
      {
        if (sCalled == 1 && delayCheck(gStartupDelay, sCalled * 1000))
        {
          sCalled += 1;
          knx.getGroupObject(firstComObj).requestObjectRead(); // Set Temp / Pos
        }
        if (sCalled == 2 && delayCheck(gStartupDelay, sCalled * 1000))
        {
          sCalled += 1;
          knx.getGroupObject(firstComObj + 3).requestObjectRead(); // Raum Temp
        }
        if (sCalled == 3)
        {
          sCalled = 255;
        }
      }
      break; // ENDE 4BS

    case u8RORG_VLD:
      //------------  Channel 1 ---------------------------------------
      if (buttonStateSimulation1 == SIMULATE_PUSH)
      {
#ifdef KDEBUG
        SERIAL_PORT.print(F("Triggered "));
        SERIAL_PORT.print(firstComObj);
        SERIAL_PORT.println(F(" Push"));
#endif
        send_RPS_Taster(lui8_SendeID_p, buttonMessage1, true, 0); // BaseID_CH = 0
        union1.buttonLastPushTime1 = millis();
        buttonStateSimulation1 = SIMULATE_RELEASE;
      }
      else if (buttonStateSimulation1 == SIMULATE_RELEASE)
      {
        if (millis() - union1.buttonLastPushTime1 >= SIMULATE_PAUSE_BEFORE_RELEASE)
        {
#ifdef KDEBUG
          SERIAL_PORT.print(F("Triggered "));
          SERIAL_PORT.print(firstComObj);
          SERIAL_PORT.println(F(" Release"));
#endif
          send_RPS_Taster(lui8_SendeID_p, buttonMessage1, false, 0); // BaseID_CH = 0
          buttonStateSimulation1 = SIMULATE_NOTHING;
        }
      }
      //------------  Channel 2 ---------------------------------------
      if (buttonStateSimulation2 == SIMULATE_PUSH)
      {
#ifdef KDEBUG
        SERIAL_PORT.print(F("Triggered "));
        SERIAL_PORT.print(firstComObj + 1);
        SERIAL_PORT.println(F(" Push"));
#endif
        send_RPS_Taster(lui8_SendeID_p, buttonMessage2, true, 1); // BaseID_CH = 1  zum Anpassen der BaseID damit jeder CH seine eigene ID hat
        union2.buttonLastPushTime2 = millis();
        buttonStateSimulation2 = SIMULATE_RELEASE;
      }
      else if (buttonStateSimulation2 == SIMULATE_RELEASE)
      {
        if (millis() - union2.buttonLastPushTime2 >= SIMULATE_PAUSE_BEFORE_RELEASE)
        {
#ifdef KDEBUG
          SERIAL_PORT.print(F("Triggered "));
          SERIAL_PORT.print(firstComObj + 1);
          SERIAL_PORT.println(F(" Release"));
#endif
          send_RPS_Taster(lui8_SendeID_p, buttonMessage2, false, 1); // BaseID_CH = 1  zum Anpassen der BaseID damit jeder CH seine eigene ID hat
          buttonStateSimulation2 = SIMULATE_NOTHING;
        }
      }
      else if (union3.D2_01_0E_SetActor == 2) // init value union3.D2_01_0E_SetActor = 2
      {
        setActorsMeasurment(lui8_SendeID_p, index, union1.val_D2_01_0E_Energy);
        union3.D2_01_0E_SetActor = union3.D2_01_0E_SetActor - 1; //  union3.D2_01_0E_SetActor -1 = 1
      }
      else if (union3.D2_01_0E_SetActor == 1)
      {
        setActorsMeasurment(lui8_SendeID_p, index, union2.val_D2_01_0E_Power);
        union3.D2_01_0E_SetActor = union3.D2_01_0E_SetActor - 1; // after that union3.D2_01_0E_SetActor = 0 and no additional MSG will send
      }
      break; // ENDE VLD

    case u8RORG_RPS:
      /* code */
      break; // ENDE RPS

    case u8RORG_Rocker:
      //------------ Rocker Switch -------------------

      switch (state)
      {
      case idle:
        if (unionMSG.rockerState_pressed != RockerIdle)
        {
          union3.rockerNr = unionMSG.rockerState_pressed;
          union1.rocker_longpress_delay = millis();
          unionMSG.rockerState_pressed = RockerIdle;
          state = checkShortLong;
#ifdef KDEBUG_Rocker
          SERIAL_PORT.print(F("start"));
#endif
        }
        break;

      case checkShortLong:
        if (delayCheck(union1.rocker_longpress_delay, knx.paramByte(ENO_CHRockerLongPressWaitTime + firstParameter) * 20))
        {
#ifdef KDEBUG_Rocker
          SERIAL_PORT.print(F("long "));
#endif
          longPress(union3.rockerNr, firstParameter, firstComObj + 1);
          state = waitLongRelease;
        }
        else
        {
          if (union2.rockerState_Release != RockerIdle)
          {

#ifdef KDEBUG_Rocker
            SERIAL_PORT.print(F("short "));
#endif
            shortPress(union3.rockerNr, firstParameter, firstComObj + 1);
            state = waitShortRelease;
          }
        }
        break;

      case waitShortRelease:
        if (union2.rockerState_Release != RockerIdle)
        {
          union2.rockerState_Release = RockerIdle;
          unionMSG.rockerState_pressed = RockerIdle;
          state = idle;
        }
        break;

      case waitLongRelease:
        if (union2.rockerState_Release != RockerIdle)
        {
#ifdef KDEBUG_Rocker
          SERIAL_PORT.print(F("wait release "));
#endif
          longStop(union3.rockerNr, firstParameter, firstComObj + 1);
          union2.rockerState_Release = RockerIdle;
          unionMSG.rockerState_pressed = RockerIdle;
          state = idle;
        }
        break;

      default:
        break;
      }
      break; // ENDE ROCKER

    default:
      break; // ENDE 4BS, VLD, 1BS, RPS, ROCKER
    }
  }

// something happened on the bus, let's react
#pragma GCC diagnostic push // If you don't want to be warned because you are not using index, include that pragma stuff
#pragma GCC diagnostic ignored "-Wunused-parameter"

  //*************************************************************************************************************************************************************
  //*************************************************************************************************************************************************************
  //
  //   KNX handle
  //
  //*************************************************************************************************************************************************************
  //*************************************************************************************************************************************************************
  void handleKnxEvents(byte koIndex, byte koNr, GroupObject &iKo)
  {
    uint8_t teachinCH = 0;

    if (koIndex != index) // prüft ob KO zum CHannel passt
    {
      return;
    }

    // SERIAL_PORT.print(F("Profil: "));
    // SERIAL_PORT.println(knx.paramByte(ENO_CHProfilSelection + firstParameter));

    switch (knx.paramByte(ENO_CHProfilSelection + firstParameter))
    {
    case u8RORG_4BS:
#ifdef KDEBUG
      SERIAL_PORT.print(F("4BS "));
      SERIAL_PORT.println(knx.paramWord(ENO_CHProfil4BS20 + firstParameter));
#endif
      switch (knx.paramWord(ENO_CHProfil4BS20 + firstParameter))
      {
      // *************** A5-20-01 ***********************************************************
      case A5_20_01:
        switch (koNr)
        {
        case KO_0: // SET Temp / SET Pos
          if ((knx.paramByte(ENO_CHA52001SPS + firstParameter) >> ENO_CHA52001SPSShift) & 1)
          {
            union1.val_A5_20_01[0] = (float)iKo.value(getDPT(VAL_DPT_9)) * 2.5; // Set Point Temp#
#ifdef KDEBUG
            SERIAL_PORT.print(F("SET Temp to: "));
            SERIAL_PORT.print(union1.val_A5_20_01[0] / 2.5);
            SERIAL_PORT.println(F("°C"));
#endif
          }
          else
          {
            union1.val_A5_20_01[0] = (uint8_t)iKo.value(getDPT(VAL_DPT_5)); // Set Point Pos
            union1.val_A5_20_01[0] = round(union1.val_A5_20_01[0] / 2.55);
#ifdef KDEBUG
            SERIAL_PORT.print(F("SET Pos to: "));
            SERIAL_PORT.print(union1.val_A5_20_01[0]);
            SERIAL_PORT.println(F("%"));
#endif
          }
          break;
        case KO_1: // Summer Bit
          if (iKo.value(getDPT(VAL_DPT_1)))
          {
            union1.val_A5_20_01[2] = (uint8_t)union1.val_A5_20_01[2] | (1 << 3); // Set Bit
#ifdef KDEBUG
            SERIAL_PORT.println(F("Sommer Umschaltung: aktiv"));
#endif
          }
          else
          {
            union1.val_A5_20_01[2] = (uint8_t)union1.val_A5_20_01[2] & ~(1 << 3); // clear Bit
#ifdef KDEBUG
            SERIAL_PORT.println(F("Sommer Umschaltung: inaktiv"));
#endif
          }
          break;
        case KO_2: // Run Init
          if (iKo.value(getDPT(VAL_DPT_1)))
          {
            union1.val_A5_20_01[2] = (uint8_t)union1.val_A5_20_01[2] | (1 << 7); // Set Bit
#ifdef KDEBUG
            SERIAL_PORT.println(F("Run Init"));
#endif
          }
          else
          {
            union1.val_A5_20_01[2] = (uint8_t)union1.val_A5_20_01[2] & ~(1 << 7); // clear Bit
          }

          break;
        case KO_3: // Raum Temp
          union1.val_A5_20_01[1] = (uint8_t)iKo.value(getDPT(VAL_DPT_9)) * 6.375;
#ifdef KDEBUG
          SERIAL_PORT.print(F("Raum-Temp: "));
          SERIAL_PORT.print(union1.val_A5_20_01[1] / 6.375);
          SERIAL_PORT.println(F("°C"));
#endif
          break;
        case KO_4: // Service CMD

          union3.A52001_CMD = iKo.value(getDPT(VAL_DPT_5));
          if (union3.A52001_CMD < 8)
          {
            if (union3.A52001_CMD == 0)
            {
              union1.val_A5_20_01[2] &= ~(1 << 4); // clear Bit4
              union1.val_A5_20_01[2] &= ~(1 << 5); // clear Bit5
              union1.val_A5_20_01[2] &= ~(1 << 6); // clear Bit6
            }
            else if (union3.A52001_CMD == 1)
            {
              union1.val_A5_20_01[2] |= (1 << 4);  // Set   Bit4
              union1.val_A5_20_01[2] &= ~(1 << 5); // clear Bit5
              union1.val_A5_20_01[2] &= ~(1 << 6); // clear Bit6
            }
            else if (union3.A52001_CMD == 2)
            {
              union1.val_A5_20_01[2] &= ~(1 << 4); // clear Bit4
              union1.val_A5_20_01[2] |= (1 << 5);  // set   Bit5
              union1.val_A5_20_01[2] &= ~(1 << 6); // clear Bit6
            }
            else if (union3.A52001_CMD == 3)
            {
              union1.val_A5_20_01[2] &= ~(1 << 4); // clear Bit4
              union1.val_A5_20_01[2] &= ~(1 << 5); // clear Bit5
              union1.val_A5_20_01[2] |= (1 << 6);  // set   Bit6
            }
#ifdef KDEBUG
            SERIAL_PORT.print(F("Service CMD: "));
            SERIAL_PORT.println(union3.A52001_CMD);
#endif
          }
          else
          {
            union1.val_A5_20_01[2] &= ~(1 << 4); // clear Bit4
            union1.val_A5_20_01[2] &= ~(1 << 5); // clear Bit5
            union1.val_A5_20_01[2] &= ~(1 << 6); // clear Bit6
#ifdef KDEBUG
            SERIAL_PORT.print(F("Service CMD: Wrong Value"));
#endif
          }
          break;
        }
        break;
      // *************** A5-20-04 ***********************************************************
      case A5_20_04:
        switch (koNr)
        {
        case KO_0: //  SET Pos
          union1.val_A5_20_04[0] = (uint8_t)iKo.value(getDPT(VAL_DPT_5));
          union1.val_A5_20_04[0] = round(union1.val_A5_20_04[0] / 2.55);
#ifdef KDEBUG
          SERIAL_PORT.print(F("SET Pos to: "));
          SERIAL_PORT.print(union1.val_A5_20_04[0]);
          SERIAL_PORT.println(F("%"));
#endif
          break;
        case KO_1: //  SET Temp
          union1.val_A5_20_04[1] = ((float)iKo.value(getDPT(VAL_DPT_9)) - 10.0) / 0.078;
#ifdef KDEBUG
          SERIAL_PORT.print(F("SET Temp to: "));
          SERIAL_PORT.print(union1.val_A5_20_04[1] * 0.078 + 10.0);
          SERIAL_PORT.println(F("°C"));
#endif
          break;
        case KO_2: // Wake UP cycle
          union3.A52004 = iKo.value(getDPT(VAL_DPT_5));
          if (union3.A52004 <= 63)
          {
            union1.val_A5_20_04[2] = ((uint8_t)union3.A52004); // Set WCU
#ifdef KDEBUG
            SERIAL_PORT.print(F("WCU: "));
            SERIAL_PORT.println(union3.A52004);
            SERIAL_PORT.print(F("DB1: "));
            SERIAL_PORT.println(union1.val_A5_20_04[2]);
#endif
          }
          else
          {
#ifdef KDEBUG
            SERIAL_PORT.print(F("WCU: wrong Value"));
#endif
          }
#ifdef KDEBUG
          SERIAL_PORT.print(F("MC: "));
          SERIAL_PORT.println((knx.paramByte(ENO_CHA52004MC + firstParameter) >> ENO_CHA52004MCShift) & 1);
#endif
          break;
        case KO_3: // Button Lock
          if (iKo.value(getDPT(VAL_DPT_1)))
          {
            union1.val_A5_20_04[3] |= (1 << 2); // set   Bit2
          }
          else
          {
            union1.val_A5_20_04[3] &= ~(1 << 2); // clear Bit2
          }
#ifdef KDEBUG
          SERIAL_PORT.print(F("BLC: "));
          SERIAL_PORT.println((bool)iKo.value(getDPT(VAL_DPT_1)));
#endif
          break;
        case KO_4: // Service CMD
          union3.A52004 = iKo.value(getDPT(VAL_DPT_5));

          switch (union3.A52004)
          {
          case 0:                                // No Change
            union1.val_A5_20_04[3] &= ~(1 << 0); // clear Bit0
            union1.val_A5_20_04[3] &= ~(1 << 1); // clear Bit1
            break;
          case 1:                                // open Value
            union1.val_A5_20_04[3] |= (1 << 0);  // set   Bit0
            union1.val_A5_20_04[3] &= ~(1 << 1); // clear Bit1
            break;
          case 2:                                // RUN initilistation
            union1.val_A5_20_04[3] &= ~(1 << 0); // clear Bit0
            union1.val_A5_20_04[3] |= (1 << 1);  // set   Bit1
            break;
          case 3:                               // RUN initilistation
            union1.val_A5_20_04[3] |= (1 << 0); // set   Bit0
            union1.val_A5_20_04[3] |= (1 << 1); // set   Bit1
            break;
          default:
            union1.val_A5_20_04[3] &= ~(1 << 0); // clear Bit0
            union1.val_A5_20_04[3] &= ~(1 << 1); // clear Bit1
            break;
          }
#ifdef KDEBUG
          SERIAL_PORT.print(F("Service : "));
          SERIAL_PORT.println(union3.A52004);
#endif
          break;
        }
        break;
      // *************** A5-20-06 ***********************************************************
      case A5_20_06:

        switch (koNr)
        {
        case KO_Teachin: // Teach-in MSG
          teachinCH = iKo.value(getDPT(VAL_DPT_5));
#ifdef KDEBUG_min
          SERIAL_PORT.println(teachinCH);
          SERIAL_PORT.println(index + 1);
#endif
          if (teachinCH != index + 1)
          {
            return;
          }
#ifdef KDEBUG_min
          SERIAL_PORT.println(F("ready to send"));
#endif
          union1.val_A5_20_06[0] = 0x80;
          union1.val_A5_20_06[1] = 0x30;
          union1.val_A5_20_06[2] = 0x49;
          union1.val_A5_20_06[3] = 0x80;
          send_4BS_Msg(lui8_SendeID_p, index, union1.val_A5_20_06, 0);
          break;

        case KO_0: // SET Temp oder SET Pos
          if ((knx.paramByte(ENO_CHA52006SPS + firstParameter) >> ENO_CHA52006SPSShift) & 1)
          {
            union1.val_A5_20_06[0] = (float)iKo.value(getDPT(VAL_DPT_9)) * 2.0; // Set Point Temp#
#ifdef KDEBUG
            SERIAL_PORT.print(F("SET Temp to: "));
            SERIAL_PORT.print(union1.val_A5_20_06[0] / 2.0);
            SERIAL_PORT.println(F("°C"));
#endif
          }
          else
          {
            union1.val_A5_20_06[0] = (uint8_t)iKo.value(getDPT(VAL_DPT_5)); // Set Point Pos
            union1.val_A5_20_06[0] = round(union1.val_A5_20_06[0] / 2.55);
#ifdef KDEBUG
            SERIAL_PORT.print(F("SET Pos to: "));
            SERIAL_PORT.print(union1.val_A5_20_06[0]);
            SERIAL_PORT.println(F("%"));
#endif
          }
          break;

        case KO_1: // Sommer Umschaltung
          if (iKo.value(getDPT(VAL_DPT_1)))
          {
            union1.val_A5_20_06[2] = (uint8_t)union1.val_A5_20_06[2] | (1 << 3); // Set Bit
#ifdef KDEBUG
            SERIAL_PORT.println(F("Sommer Umschaltung: aktiv"));
#endif
          }
          else
          {
            union1.val_A5_20_06[2] = (uint8_t)union1.val_A5_20_06[2] & ~(1 << 3); // clear Bit
#ifdef KDEBUG
            SERIAL_PORT.println(F("Sommer Umschaltung: inaktiv"));
#endif
          }
          break;

        case KO_2: // Set Communications Interval
#ifdef KDEBUG
          SERIAL_PORT.print(F("Com Interval: "));
          SERIAL_PORT.println((uint8_t)iKo.value(getDPT(VAL_DPT_5)));
#endif
          union1.val_A5_20_06[2] &= ~(1 << ENO_CHA52006RFCShift);                                    // clear Bit4
          union1.val_A5_20_06[2] &= ~(1 << (ENO_CHA52006RFCShift + 1));                              // clear Bit5
          union1.val_A5_20_06[2] &= ~(1 << (ENO_CHA52006RFCShift + 2));                              // clear Bit6
          union1.val_A5_20_06[2] |= ((uint8_t)iKo.value(getDPT(VAL_DPT_5)) << ENO_CHA52006RFCShift); // Set Bit
          break;

        case KO_3:                                                              // Raum Temperatur
          union1.val_A5_20_06[1] = (uint8_t)iKo.value(getDPT(VAL_DPT_9)) * 4.0; // Raumtemperatur
#ifdef KDEBUG
          SERIAL_PORT.print(F("Raum-Temp: "));
          SERIAL_PORT.print(union1.val_A5_20_06[1]);
          SERIAL_PORT.println(F("°C"));
#endif
          break;

        case KO_4: // Standby
          if (iKo.value(getDPT(VAL_DPT_1)))
          {
            union1.val_A5_20_06[2] = (uint8_t)union1.val_A5_20_06[2] | (1 << 0); // Set Bit
#ifdef KDEBUG
            SERIAL_PORT.println(F("Standby"));
#endif
          }
          else
          {
            union1.val_A5_20_06[2] = (uint8_t)union1.val_A5_20_06[2] & ~(1 << 0); // clear Bit
#ifdef KDEBUG
            SERIAL_PORT.println(F("normal Mode"));
#endif
          }
          break;

        default:
          break;
        }
        // gesendet wird in der TASK() da das Device nur nachrichten Empfangen kann, 1sek nachdem es eine Nachrticht geschickt hat
        // send_4BS_Msg(lui8_SendeID_p, index, union1.val_A5_20_06);
        break;
      } // ENDE 2BS
      break;
    case u8RORG_VLD:
      switch (knx.paramWord(ENO_CHProfilSelectionVLD + firstParameter))
      {
      case D2_01:
        switch (knx.paramWord(ENO_CHProfilVLD01 + firstParameter))
        {
        case D2_01_0E:
          switch (koNr)
          {
          case KO_0: // schalten Aktor
            if (iKo.value(getDPT(VAL_DPT_1)))
            {
              buttonStateSimulation1 = SIMULATE_PUSH;
              buttonMessage1 = true;
            }
            else
            {
              buttonStateSimulation1 = SIMULATE_PUSH;
              buttonMessage1 = false;
            }
#ifdef KDEBUG
            SERIAL_PORT.println(F("KNX KO_0 handled"));
#endif
            break;
          case KO_1: // Anfrage Messwerte
#ifdef KDEBUG
            SERIAL_PORT.println(F("KNX KO_1 handled"));
#endif
            getActorsMeasurmentValue(lui8_SendeID_p, index, union1.val_D2_01_0E_Energy, iKo.value(getDPT(VAL_DPT_1))); // Energy = 0  / Power = 1
            break;
          case KO_2: // Anfrage Statuswerte SWITCH
#ifdef KDEBUG
            SERIAL_PORT.println(F("KNX KO_2 handled"));
#endif
            getStatusActors(lui8_SendeID_p, index); // Request Aktor Status
            break;
          }
        default:
          break; // ENDE VLD D2_01_0E
        case D2_01_12:
          switch (koNr)
          {
          case KO_0: // schalten Aktor CH1
            setStatusActors(lui8_SendeID_p, index, iKo.value(getDPT(VAL_DPT_1)));

            if (iKo.value(getDPT(VAL_DPT_1)))
            {

              /*buttonStateSimulation1 = SIMULATE_PUSH;
              buttonMessage1 = true;*/
#ifdef KDEBUG
              SERIAL_PORT.println(F("KO value = HIGH"));
#endif
            }
            else
            {
              /*buttonStateSimulation1 = SIMULATE_PUSH;
              buttonMessage1 = false;*/
#ifdef KDEBUG
              SERIAL_PORT.println(F("KO value = LOW"));
#endif
            }
#ifdef KDEBUG
            SERIAL_PORT.println(F("KNX KO_0 handled"));
#endif
            break;
          case KO_1: //  Schalten Aktor CH2
            // union4.koIndex2 = index + 2;
            if (iKo.value(getDPT(VAL_DPT_1)))
            {
              buttonStateSimulation2 = SIMULATE_PUSH;
              buttonMessage2 = true;
            }
            else
            {
              buttonStateSimulation2 = SIMULATE_PUSH;
              buttonMessage2 = false;
            }
#ifdef KDEBUG
            SERIAL_PORT.println(F("KNX KO_1 handled"));
#endif
            break;
          case KO_4:
#ifdef KDEBUG
            SERIAL_PORT.println(F("KNX KO_4 handled"));
#endif
            getStatusActors(lui8_SendeID_p, index); // Request Aktor Status
            break;
          default:
            break;
          }
          break; // ENDE VLD D2_01_12
        }
        break; // ENDE VLD D2_01
      }
      break; // ENDE VLD

    case u8RORG_Rocker:
      switch (knx.paramByte(ENO_CHDirectionKnxEnocean + firstParameter))
      {
      case 1:
#ifdef KDEBUG
        SERIAL_PORT.println(F("send Rocker MSG"));
#endif
        if (iKo.value(getDPT(VAL_DPT_1)))
        {
          send_RPS_Taster(lui8_SendeID_p, false, true, 0); // BaseID_CH = 0
        }
        else
        {
          send_RPS_Taster(lui8_SendeID_p, true, true, 0); // BaseID_CH = 0
        }
        break;
      }
      break;

    default:
      return;
      break; // ENDE ROCKER
    }
  }
#pragma GCC diagnostic pop // I don't want a warning, just because we don't do anything here

// decode EnOcean message. Fail fast!
#pragma GCC diagnostic push // If you don't want to be warned because you are not using index, include that pragma stuff
#pragma GCC diagnostic ignored "-Wunused-parameter"

  //*************************************************************************************************************************************************************
  //*************************************************************************************************************************************************************
  //
  //   EnOcean handle
  //
  //*************************************************************************************************************************************************************
  //*************************************************************************************************************************************************************
  bool handleEnOceanPacket(PACKET_SERIAL_TYPE *f_Pkt_st)
  {
    // uint8_t DevicId_Arr_TEST[4] = {0xFF, 0xDC, 0xD4, 0x80};
    // uint8_t MSGDef;

    // Get rid of messages we can't handle
    if (f_Pkt_st->u8DataBuffer[0] != knx.paramByte(ENO_CHProfilSelection + firstParameter) && knx.paramByte(ENO_CHProfilSelection + firstParameter) != u8RORG_Rocker)
      return false;

    switch (knx.paramByte(ENO_CHProfilSelection + firstParameter))
    {
    case u8RORG_1BS:
#ifdef KDEBUG_ID
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[2], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[3], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[4], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.println(f_Pkt_st->u8DataBuffer[5], HEX);
#endif
      // Get rid of messages not intended for us
      if (f_Pkt_st->u8DataBuffer[2] != deviceId_Arr[0])
        return false;
      if (f_Pkt_st->u8DataBuffer[3] != deviceId_Arr[1])
        return false;
      if (f_Pkt_st->u8DataBuffer[4] != deviceId_Arr[2])
        return false;
      if (f_Pkt_st->u8DataBuffer[5] != deviceId_Arr[3])
        return false;

      handle_1BS(f_Pkt_st, profil, firstComObj, firstParameter);
      break;

    case u8RORG_RPS:
      // Get rid of messages not intended for us
      if (f_Pkt_st->u8DataBuffer[2] != deviceId_Arr[0])
        return false;
      if (f_Pkt_st->u8DataBuffer[3] != deviceId_Arr[1])
        return false;
      if (f_Pkt_st->u8DataBuffer[4] != deviceId_Arr[2])
        return false;
      if (f_Pkt_st->u8DataBuffer[5] != deviceId_Arr[3])
        return false;

#ifdef KDEBUG_ID
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[2], HEX);
      SERIAL_PORT.print("-");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[3], HEX);
      SERIAL_PORT.print("-");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[4], HEX);
      SERIAL_PORT.print("-");
      SERIAL_PORT.println(f_Pkt_st->u8DataBuffer[5], HEX);
#endif

      handle_RPS(f_Pkt_st, profil, firstComObj, firstParameter);
      break;

    case u8RORG_4BS:

      // Get rid of messages not intended for us
      if (f_Pkt_st->u8DataBuffer[5] != deviceId_Arr[0])
        return false;
      if (f_Pkt_st->u8DataBuffer[6] != deviceId_Arr[1])
        return false;
      if (f_Pkt_st->u8DataBuffer[7] != deviceId_Arr[2])
        return false;
      if (f_Pkt_st->u8DataBuffer[8] != deviceId_Arr[3])
        return false;

#ifdef KDEBUG_ID
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[5], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[6], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[7], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.println(f_Pkt_st->u8DataBuffer[8], HEX);
#endif
      unionMSG.msg_sent_after_receive = handle_4BS(f_Pkt_st, profil, profil2nd, firstComObj, firstParameter);
      break;

    case u8RORG_VLD:

      // Get rid of messages not intended for us
      if (f_Pkt_st->u8DataBuffer[f_Pkt_st->u16DataLength - 5] != deviceId_Arr[0])
        return false;
      if (f_Pkt_st->u8DataBuffer[f_Pkt_st->u16DataLength - 4] != deviceId_Arr[1])
        return false;
      if (f_Pkt_st->u8DataBuffer[f_Pkt_st->u16DataLength - 3] != deviceId_Arr[2])
        return false;
      if (f_Pkt_st->u8DataBuffer[f_Pkt_st->u16DataLength - 2] != deviceId_Arr[3])
        return false;

#ifdef KDEBUG_ID
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[f_Pkt_st->u16DataLength - 5], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[f_Pkt_st->u16DataLength - 4], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[f_Pkt_st->u16DataLength - 3], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.println(f_Pkt_st->u8DataBuffer[f_Pkt_st->u16DataLength - 2], HEX);
#endif

      handle_VLD(f_Pkt_st, profil, firstComObj, firstParameter);
      break;

    case u8RORG_Rocker:
      // Get rid of messages not intended for us
      if (f_Pkt_st->u8DataBuffer[2] != deviceId_Arr[0])
        return false;
      if (f_Pkt_st->u8DataBuffer[3] != deviceId_Arr[1])
        return false;
      if (f_Pkt_st->u8DataBuffer[4] != deviceId_Arr[2])
        return false;
      if (f_Pkt_st->u8DataBuffer[5] != deviceId_Arr[3])
        return false;

#ifdef KDEBUG_Rocker
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[2], HEX);
      SERIAL_PORT.print(".");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[3], HEX);
      SERIAL_PORT.print(".");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[4], HEX);
      SERIAL_PORT.print(".");
      SERIAL_PORT.println(f_Pkt_st->u8DataBuffer[5], HEX);
#endif
      // uint8_t stateRocker = handle_RPS_Rocker(f_Pkt_st, profil, firstComObj, firstParameter, index);
      uint8_t stateRocker = f_Pkt_st->u8DataBuffer[1];
#ifdef KDEBUG
      // SERIAL_PORT.println(stateRocker, HEX);
#endif
      switch (stateRocker)
      {
      case AI_pressed:
        unionMSG.rockerState_pressed = stateRocker;
        break;
      case AI_release:
        union2.rockerState_Release = stateRocker;
        break;
      case AO_pressed:
        unionMSG.rockerState_pressed = stateRocker;
        break;
      case AO_release:
        union2.rockerState_Release = stateRocker;
        break;
      case BI_pressed:
        unionMSG.rockerState_pressed = stateRocker;
        break;
      case BI_release:
        union2.rockerState_Release = stateRocker;
        break;
      case BO_pressed:
        unionMSG.rockerState_pressed = stateRocker;
        break;
      case BO_release:
        union2.rockerState_Release = stateRocker;
        break;
      case CI_pressed:
        unionMSG.rockerState_pressed = stateRocker;
        break;
      case CI_release:
        union2.rockerState_Release = stateRocker;
        break;
      case CO_pressed:
        unionMSG.rockerState_pressed = stateRocker;
        break;
      case CO_release:
        union2.rockerState_Release = stateRocker;
        break;
      case Contact_pressed:
        if (knx.paramByte(firstParameter + ENO_CHRockerProfil) == Wippen1)
          handle_RPS_Rocker(f_Pkt_st, profil, firstComObj, firstParameter, index);
        break;
      case Contact_release:
        if (knx.paramByte(firstParameter + ENO_CHRockerProfil) == Wippen1)
          handle_RPS_Rocker(f_Pkt_st, profil, firstComObj, firstParameter, index);
        break;
      default:
        union2.rockerState_Release = stateRocker; // Für Wert = 0
        break;
      }
      break;
    }
    return true;
  }
#pragma GCC diagnostic pop // I don't want a warning, just because we don't do anything here
};

#endif /* EnOceanDevice_EMPTY_H_ */
