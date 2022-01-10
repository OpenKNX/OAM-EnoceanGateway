
#ifndef EnOceanDevice_H_
#define EnOceanDevice_H_

#include "EnOcean.h"
#include "EnoceanGateway.h"
#include "EnoceanSendFunctions.h"
#include "RPS_Telegram.h"
#include "4BS_Telegram.h"
#include "VLD_Telegram.h"
#include "hardware.h"
#include "KnxHelper.h"

// ################################################
// ### DEBUG CONFIGURATION
// ################################################
// DEBUG-MODE
#define KDEBUG // comment this line to disable DEBUG mode

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

  union EnoceanHandle
  {
    uint8_t msg_sent_after_receive;
    uint8_t rockerState = RockerIdle;
  } unionMSG;

  union TaskHandle1
  {
    uint8_t val_A5_20_06[4];
    uint32_t rocker_longpress_delay;
    uint32_t buttonLastPushTime1;
  } union1;

  union TaskHandle2
  {
    byte val_A5_20_06_TeachIn[4];
    uint32_t rocker_longpress_delay;
    uint32_t buttonLastPushTime2;
  } union2;

  union TaskHandle3
  {
    uint8_t koIndex1;
    uint8_t rockerNr;
  } union3;

  union TaskHandle4
  {
    uint8_t koIndex2;
  } union4;

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

#ifdef KDEBUG
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
  void init(uint8_t startAtComObj, uint8_t startAtParameter, uint8_t channel)
  {
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

    //default Werte
    if (knx.paramWord(ENO_CHProfil4BS20 + firstParameter) == A5_20_06)
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
    // default Startup delay Startpunkt
    gStartupDelay = millis(); // Read Request Delay startup

    //profil = 45; // nur zum Testen <<<<----------------------------------------------------

#ifdef KDEBUG
    if (knx.configured())
    {
      SERIAL_PORT.print(F("I am: "));
      for (int i = 0; i < BASEID_BYTES; i++)
      {
        SERIAL_PORT.print(deviceId_Arr[i], HEX);
      }
      SERIAL_PORT.println(F(""));
      SERIAL_PORT.println(knx.paramByte(ENO_CHProfilSelection + firstParameter));

      switch (knx.paramByte(ENO_CHProfilSelection + firstParameter))
      //switch (u8RORG_Rocker)                                 // nur zum Testen <<<<----------------------------------------------------
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

    switch (knx.paramByte(ENO_CHProfilSelection + firstParameter))
    {
    case u8RORG_1BS:
      /* code */
      break;

    case u8RORG_4BS:
      // *************** Sent after receive ***********************************************************************
      //-----------------------------------------------------------------------------------------------------------
      //A5-20-06
      if (unionMSG.msg_sent_after_receive == msg_A5_20_06)
      {
        // Set LNRB
        union1.val_A5_20_06[3] |= 1 << 3; // Set LNRB
        send_4BS_Msg(enOcean.getBaseId(), index, union1.val_A5_20_06, 0);
        // löscht Ref Run Bit
        union1.val_A5_20_06[2] & ~(1 << 7); // clear Bit
        unionMSG.msg_sent_after_receive = 0;
      }
      // *************** Teachin Funktion  ***********************************************************************
      //-----------------------------------------------------------------------------------------------------------
      //A5-20-06
      else if (unionMSG.msg_sent_after_receive == TEACHIN_A52006)
      {
        union2.val_A5_20_06_TeachIn[0] = 0x80;
        union2.val_A5_20_06_TeachIn[1] = 0x30;
        union2.val_A5_20_06_TeachIn[2] = 0x49;
        union2.val_A5_20_06_TeachIn[3] = 0xF0;
        send_4BS_Msg(enOcean.getBaseId(), index, union2.val_A5_20_06_TeachIn, 0);
#ifdef KDEBUG
        SERIAL_PORT.println(F("TeachIn Response Sent"));
#endif
        unionMSG.msg_sent_after_receive = 0;
      }
      // *************** Read Request  ***********************************************************************
      //-----------------------------------------------------------------------------------------------------------
      //A5-20-06
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
        send_RPS_Taster(lui8_SendeID_p, buttonMessage1, true, 0); //BaseID_CH = 0
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
          send_RPS_Taster(lui8_SendeID_p, buttonMessage1, false, 0); //BaseID_CH = 0
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
      break; // ENDE VLD

    case u8RORG_RPS:
      /* code */
      break; // ENDE RPS

    case u8RORG_Rocker:
      //------------ Rocker Switch -------------------
      if (unionMSG.rockerState != RockerIdle)
      {
#ifdef KDEBUG
        SERIAL_PORT.println(unionMSG.rockerState, HEX);
#endif
        switch (unionMSG.rockerState)
        {
          //-------- Rocker pressed ---------------------
        case AI_pressed:
          union1.rocker_longpress_delay = millis();
          unionMSG.rockerState = RockerIdle;
          union3.rockerNr = ROCKER_AI;
          break;
        case AO_pressed:
          union1.rocker_longpress_delay = millis();
          unionMSG.rockerState = RockerIdle;
          union3.rockerNr = ROCKER_AO;
          break;
        case BI_pressed:
          union1.rocker_longpress_delay = millis();
          unionMSG.rockerState = RockerIdle;
          union3.rockerNr = ROCKER_BI;
          break;
        case BO_pressed:
          union1.rocker_longpress_delay = millis();
          unionMSG.rockerState = RockerIdle;
          union3.rockerNr = ROCKER_BO;
          break;
        case CI_pressed:
          union1.rocker_longpress_delay = millis();
          unionMSG.rockerState = RockerIdle;
          union3.rockerNr = ROCKER_CI;
          break;
        case CO_pressed:
          union1.rocker_longpress_delay = millis();
          unionMSG.rockerState = RockerIdle;
          union3.rockerNr = ROCKER_CO;
          break;

        //-------- Rocker short presse release ---------------------
        case AI_release:
#ifdef KDEBUG
          SERIAL_PORT.print(F("AI short: "));
#endif
          release_Button(ButtonStateI, firstParameter, firstComObj, ENO_CHRockerFunktionA, ENO_CHRockerASzeneB, false);
          union3.rockerNr = ROCKER_INACTIVE;
          break;

        case AO_release:
#ifdef KDEBUG
          SERIAL_PORT.print(F("AO short: "));
#endif
          release_Button(ButtonStateO, firstParameter, firstComObj, ENO_CHRockerFunktionA, ENO_CHRockerASzeneA, false);
          union3.rockerNr = ROCKER_INACTIVE;
          break;
        case BI_release:
#ifdef KDEBUG
          SERIAL_PORT.print(F("BI short: "));
#endif
          release_Button(ButtonStateI, firstParameter, firstComObj, ENO_CHRockerFunktionB, ENO_CHRockerBSzeneB, false);
          union3.rockerNr = ROCKER_INACTIVE;
          break;
        case BO_release:
#ifdef KDEBUG
          SERIAL_PORT.print(F("BO short: "));
#endif
          release_Button(ButtonStateO, firstParameter, firstComObj, ENO_CHRockerFunktionB, ENO_CHRockerBSzeneA, false);
          union3.rockerNr = ROCKER_INACTIVE;
          break;

        case CI_release:
#ifdef KDEBUG
          SERIAL_PORT.print(F("CI short: "));
#endif
          release_Button(ButtonStateI, firstParameter, firstComObj, ENO_CHRockerFunktionC, ENO_CHRockerCSzeneB, false);
          union3.rockerNr = ROCKER_INACTIVE;
          break;

        case CO_release:
#ifdef KDEBUG
          SERIAL_PORT.print(F("CO short: "));
#endif
          release_Button(ButtonStateO, firstParameter, firstComObj, ENO_CHRockerFunktionC, ENO_CHRockerCSzeneA, false);
          union3.rockerNr = ROCKER_INACTIVE;
          break;
        default:
          break;
        } // ENDE Rocker Taste
      }   // ENDE IF Idle

      if (union3.rockerNr != 0 && delayCheck(union1.rocker_longpress_delay, knx.paramByte(ENO_CHRockerLongPressWaitTime + firstParameter) * 10))
      {
#ifdef KDEBUG
        SERIAL_PORT.print(F("Rocker Nr. "));
        SERIAL_PORT.println(union3.rockerNr);
        SERIAL_PORT.print(F("Par: "));
        SERIAL_PORT.println(knx.paramByte(ENO_CHRockerLongPressWaitTime + firstParameter) * 10);  
#endif
        switch (union3.rockerNr)
        {
        case ROCKER_AI:
#ifdef KDEBUG
          SERIAL_PORT.print(F("AI long: "));
#endif
          release_Button(ButtonStateI, firstParameter, firstComObj, ENO_CHRockerFunktionA, ENO_CHRockerASzeneB, true);
          union3.rockerNr = ROCKER_INACTIVE;
          break;
        case ROCKER_AO:
#ifdef KDEBUG
          SERIAL_PORT.print(F("AO long: "));
#endif
          release_Button(ButtonStateO, firstParameter, firstComObj, ENO_CHRockerFunktionA, ENO_CHRockerASzeneA, true);
          union3.rockerNr = ROCKER_INACTIVE;
          break;
        case ROCKER_BI:
#ifdef KDEBUG
          SERIAL_PORT.print(F("BI long: "));
#endif
          release_Button(ButtonStateI, firstParameter, firstComObj, ENO_CHRockerFunktionB, ENO_CHRockerBSzeneB, true);
          union3.rockerNr = ROCKER_INACTIVE;
          break;
        case ROCKER_BO:
#ifdef KDEBUG
          SERIAL_PORT.print(F("BO long: "));
#endif
          release_Button(ButtonStateO, firstParameter, firstComObj, ENO_CHRockerFunktionB, ENO_CHRockerBSzeneA, true);
          union3.rockerNr = ROCKER_INACTIVE;
          break;
        case ROCKER_CI:
#ifdef KDEBUG
          SERIAL_PORT.print(F("CI long: "));
#endif
          release_Button(ButtonStateI, firstParameter, firstComObj, ENO_CHRockerFunktionC, ENO_CHRockerCSzeneB, true);
          union3.rockerNr = ROCKER_INACTIVE;
          break;
        case ROCKER_CO:
#ifdef KDEBUG
          SERIAL_PORT.print(F("CO long: "));
#endif
          release_Button(ButtonStateO, firstParameter, firstComObj, ENO_CHRockerFunktionC, ENO_CHRockerBSzeneA, true);
          union3.rockerNr = ROCKER_INACTIVE;
          break;

        default:
          break;
        }
      }
      break; // ENDE ROCKER

    default:
      break;
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

    //SERIAL_PORT.print(F("Profil: "));
    //SERIAL_PORT.println(knx.paramByte(ENO_CHProfilSelection + firstParameter));

    switch (knx.paramByte(ENO_CHProfilSelection + firstParameter))
    {
    case u8RORG_4BS:

      SERIAL_PORT.print(F("4BS "));
      SERIAL_PORT.println(knx.paramWord(ENO_CHProfil4BS20 + firstParameter));

      switch (knx.paramWord(ENO_CHProfil4BS20 + firstParameter))
      {
      case A5_20_06:

        switch (koNr)
        {
        case KO_Teachin: // Teach-in MSG
          teachinCH = iKo.value(getDPT(VAL_DPT_5));
          SERIAL_PORT.println(teachinCH);
          SERIAL_PORT.println(index + 1);
          if (teachinCH != index + 1)
          {
            return;
          }
          SERIAL_PORT.println(F("ready to send"));
          union1.val_A5_20_06[0] = 0x80;
          union1.val_A5_20_06[1] = 0x30;
          union1.val_A5_20_06[2] = 0x49;
          union1.val_A5_20_06[3] = 0x80;
          send_4BS_Msg(enOcean.getBaseId(), index, union1.val_A5_20_06, 0);
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
#ifdef KDEBUG #
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
          union1.val_A5_20_06[2] &= ~(1 << ENO_CHA52006RFCShift + 1);                                // clear Bit5
          union1.val_A5_20_06[2] &= ~(1 << ENO_CHA52006RFCShift + 2);                                // clear Bit6
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
        //send_4BS_Msg(enOcean.getBaseId(), index, union1.val_A5_20_06);
        break;
      } // ENDE 2BS
      break;
    case u8RORG_VLD:
      switch (knx.paramWord(ENO_CHProfilSelectionVLD + firstParameter))
      {
      case D2_01:
        switch (knx.paramWord(ENO_CHProfilVLD01 + firstParameter))
        {
        case D2_01_12:
          switch (koNr)
          {
          case KO_0: // chalten Aktor CH1
            union3.koIndex1 = index + 1;
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
          case KO_1: //  Schalten Aktor CH2
            union4.koIndex2 = index + 2;
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
            getStatusActors(enOcean.getBaseId(), index); // Request Aktor Status
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
        // funktion zum senden Sknx.paramByte(ENO_CHProfilSelection + firstParameter
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
    uint8_t DevicId_Arr_TEST[4] = {0xFF, 0xDC, 0xD4, 0x80};
    uint8_t MSGDef;

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

      handle_1BS(f_Pkt_st);
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

#ifdef KDEBUG
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[2], HEX);
      SERIAL_PORT.print(".");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[3], HEX);
      SERIAL_PORT.print(".");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[4], HEX);
      SERIAL_PORT.print(".");
      SERIAL_PORT.println(f_Pkt_st->u8DataBuffer[5], HEX);
#endif

      unionMSG.rockerState = handle_RPS_Rocker(f_Pkt_st, profil, firstComObj, firstParameter, index);
      break;
    }
    return true;
  }
#pragma GCC diagnostic pop // I don't want a warning, just because we don't do anything here

  /*******************************************************************************************
 *  1BS
 *******************************************************************************************/

  void handle_1BS(PACKET_SERIAL_TYPE *f_Pkt_st)
  {
    bool bvalue;

    ONEBS_TELEGRAM_TYPE *l1bsTlg_p = (ONEBS_TELEGRAM_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);

    // D5-00-01 Contact: 0 = open / 1 = close
    // ETS Parameter to define state for open / Close
    if (l1bsTlg_p->u81bsTelData.State == 1) // CLOSE
    {
      if (((knx.paramByte(firstParameter + ENO_CHWindowcloseValue)) >> ENO_CHWindowcloseValueShift) & 1)
        bvalue = true;
      else
        bvalue = false;
    }
    else
    { // OPEN
      if (((knx.paramByte(firstParameter + ENO_CHWindowcloseValue)) >> ENO_CHWindowcloseValueShift) & 1)
        bvalue = false;
      else
        bvalue = true;
    }
    knx.getGroupObject(firstComObj).value(bvalue, getDPT(VAL_DPT_1));

#ifdef KDEBUG
    SERIAL_PORT.print(F("detected: 1BS State: "));
    SERIAL_PORT.println(l1bsTlg_p->u81bsTelData.State);
#endif
  }
};

#endif /* EnOceanDevice_EMPTY_H_ */
