
#ifndef EnOceanDevice_H_
#define EnOceanDevice_H_

#include "EnOcean.h"
#include "EnoceanGateway.h"
#include "EnoceanSendFunctions.h"
#include "RPS_Telegram.h"
#include "4BS_Telegram.h"
#include "VLD_Telegram.h"

// ################################################
// ### DEBUG CONFIGURATION
// ################################################
// DEBUG-MODE
#define KDEBUG // comment this line to disable DEBUG mode

class EnOceanDevice : public IEnOceanDevice
{
private:
  uint8_t deviceId_Arr[4] = {0, 0, 0, 0};
  uint32_t deviceId = 0;
  uint8_t profil = 0;
  uint8_t profil2nd = 0;

  uint8_t buttonStateSimulation1 = SIMULATE_NOTHING;
  uint8_t buttonStateSimulation2 = SIMULATE_NOTHING;
  bool buttonMessage1 = false;
  bool buttonMessage2 = false;
  uint32_t buttonLastPushTime1 = 0;
  uint32_t buttonLastPushTime2 = 0;
  uint8_t koIndex1;
  uint8_t koIndex2;

  union
  {
    byte val_A5_20_06[4];
  } union1;

public:
  EnOceanDevice()
  {
    // numberOfComObjects = 4;
    // numberOfParameters = 2;
  }

  // pre-configure device, set sender-id etc.
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

    //profil = 45; // nur zum Testen <<<<----------------------------------------------------

#ifdef KDEBUG
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
#endif
  }

  // things that need to be done regularly
  void task()
  {
    //------------  Channel 1 ---------------------------------------
    if (buttonStateSimulation1 == SIMULATE_PUSH)
    {
      send_RPS_Taster(enOcean.getBaseId(), koIndex1, buttonMessage1, true); // +koIndex zum Anpassen der BaseID damit jeder CH seine eigene ID hat
      buttonLastPushTime1 = millis();
      buttonStateSimulation1 = SIMULATE_RELEASE;
#ifdef KDEBUG
      SERIAL_PORT.print(F("Triggered "));
      SERIAL_PORT.print(firstComObj);
      SERIAL_PORT.println(F(" Push"));
#endif
    }
    else if (buttonStateSimulation1 == SIMULATE_RELEASE)
    {
      if (millis() - buttonLastPushTime1 >= SIMULATE_PAUSE_BEFORE_RELEASE)
      {
        send_RPS_Taster(enOcean.getBaseId(), koIndex1, buttonMessage1, false);
        buttonStateSimulation1 = SIMULATE_NOTHING;
#ifdef KDEBUG
        SERIAL_PORT.print(F("Triggered "));
        SERIAL_PORT.print(firstComObj);
        SERIAL_PORT.println(F(" Release"));
#endif
      }
    }
    //------------  Channel 2 ---------------------------------------
    if (buttonStateSimulation2 == SIMULATE_PUSH)
    {
      send_RPS_Taster(enOcean.getBaseId(), koIndex2, buttonMessage2, true); // +koIndex zum Anpassen der BaseID damit jeder CH seine eigene ID hat
      buttonLastPushTime2 = millis();
      buttonStateSimulation2 = SIMULATE_RELEASE;
#ifdef KDEBUG
      SERIAL_PORT.print(F("Triggered "));
      SERIAL_PORT.print(firstComObj + 1);
      SERIAL_PORT.println(F(" Push"));
#endif
    }
    else if (buttonStateSimulation2 == SIMULATE_RELEASE)
    {
      if (millis() - buttonLastPushTime2 >= SIMULATE_PAUSE_BEFORE_RELEASE)
      {
        send_RPS_Taster(enOcean.getBaseId(), koIndex2, buttonMessage2, false);
        buttonStateSimulation2 = SIMULATE_NOTHING;
#ifdef KDEBUG
        SERIAL_PORT.print(F("Triggered "));
        SERIAL_PORT.print(firstComObj + 1);
        SERIAL_PORT.println(F(" Release"));
#endif
      }
    }
  }

  // something happened on the bus, let's react
#pragma GCC diagnostic push // If you don't want to be warned because you are not using index, include that pragma stuff
#pragma GCC diagnostic ignored "-Wunused-parameter"
  void handleKnxEvents(byte koIndex, byte koNr, GroupObject &iKo)
  {
    static bool init1 = true;
    static bool init2 = true;

    SERIAL_PORT.print(F("Profil: "));
    SERIAL_PORT.println(knx.paramByte(ENO_CHProfilSelection + firstParameter));

    switch (knx.paramByte(ENO_CHProfilSelection + firstParameter))
    {
    case u8RORG_4BS:

      SERIAL_PORT.print(F("4BS "));
      SERIAL_PORT.println(knx.paramWord(ENO_CHProfil4BS20 + firstParameter));

      switch (knx.paramWord(ENO_CHProfil4BS20 + firstParameter))
      {
      case A5_20_06:

        //default Werte
        if (init1)
          union1.val_A5_20_06[0] = 0;
        if (init2)
          union1.val_A5_20_06[1] = 160;
        // set Settings from ETS
        union1.val_A5_20_06[2] = knx.paramByte(ENO_CHA52006RFC + firstParameter); // Ein Parameter reicht, da alles im UNION gespeichert wird.

        switch (koNr)
        {
        case KO_0: // SET Temp oder SET Pos
          init1 = false;

          if (knx.paramByte(ENO_CHA52006SPS + firstParameter) & ENO_CHA52006SPSMask > 0)
          {
            union1.val_A5_20_06[0] = (uint8_t)iKo.value(getDPT(VAL_DPT_9)) * 2.0; // Set Point Temp
#ifdef KDEBUG
            SERIAL_PORT.print(F("SET Temp to: "));
            SERIAL_PORT.print((uint8_t)iKo.value(getDPT(VAL_DPT_9)));
            SERIAL_PORT.println(F("°C"));
#endif
          }

          else
            union1.val_A5_20_06[0] = (uint8_t)iKo.value(getDPT(VAL_DPT_5)); // Set Point Pos
#ifdef KDEBUG
          SERIAL_PORT.print(F("SET Pos to: "));
          SERIAL_PORT.print((uint8_t)iKo.value(getDPT(VAL_DPT_5)));
          SERIAL_PORT.println(F("%"));
#endif
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

        case KO_2: // Run Reference
          if (iKo.value(getDPT(VAL_DPT_1)))
          {
            union1.val_A5_20_06[2] = (uint8_t)union1.val_A5_20_06[2] | (1 << 7); // Set Bit
#ifdef KDEBUG
            SERIAL_PORT.println(F("RUN Reference"));
#endif
          }
          else
            union1.val_A5_20_06[2] = (uint8_t)union1.val_A5_20_06[2] & ~(1 << 7); // clear Bit
          break;

        case KO_3: // Raum Temperatur
          init2 = false;
          union1.val_A5_20_06[1] = (uint8_t)iKo.value(getDPT(VAL_DPT_9)) * 4.0; // Raumtemperatur
#ifdef KDEBUG
          SERIAL_PORT.print(F("Raum-Temp: "));
          SERIAL_PORT.print((uint8_t)iKo.value(getDPT(VAL_DPT_9)));
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
        union1.val_A5_20_06[3] = 0; // only TeachIn

        send_4BS_Msg(enOcean.getBaseId(), index, union1.val_A5_20_06);
        break;
      }
      break;
    case u8RORG_VLD:
      switch (knx.paramWord(ENO_CHProfilSelectionVLD + firstParameter))
      {
      case D2_01_12:
        switch (koNr)
        {
        case KO_0: // chalten Aktor CH1
          koIndex1 = index + 1;
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
          break;
        case KO_1: //  Schalten Aktor CH2
          koIndex2 = index + 2;
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
          break;
        case KO_4:                                         // Request Aktor Status
          getStatusActors(enOcean.getBaseId(), index + 1); // index + 1 -> da es reicht einen Kanal anzufragen, die Antwort kommt immer für beide Kanäle
          break;
        default:
          break;
        }
        break;
      }
      break;

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
      break;
    }
  }
#pragma GCC diagnostic pop // I don't want a warning, just because we don't do anything here

  // decode EnOcean message. Fail fast!
#pragma GCC diagnostic push // If you don't want to be warned because you are not using index, include that pragma stuff
#pragma GCC diagnostic ignored "-Wunused-parameter"
  bool handleEnOceanPacket(PACKET_SERIAL_TYPE *f_Pkt_st)
  {
    uint8_t DevicId_Arr_TEST[4] = {0xFF, 0xDC, 0xD4, 0x80};
    uint8_t MSGDef;

#ifndef EnOceanTEST
    // Get rid of messages we can't handle
    if (f_Pkt_st->u8DataBuffer[0] != knx.paramByte(ENO_CHProfilSelection + firstParameter) && knx.paramByte(ENO_CHProfilSelection + firstParameter) != u8RORG_Rocker)
      return false;
#endif

// ONLY FOR TESTING
#ifdef EnOceanTEST
    SERIAL_PORT.println("ENOCEAN TEST");

    profil = A5_20;
    profil2nd = A5_20_06;
    MSGDef = u8RORG_4BS;

    switch (MSGDef)
    {
    case u8RORG_1BS:
#ifdef KDEBUG
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[2], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[3], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[4], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.println(f_Pkt_st->u8DataBuffer[5], HEX);
#endif
      // Get rid of messages not intended for us
      if (f_Pkt_st->u8DataBuffer[2] != DevicId_Arr_TEST[0])
        return false;
      if (f_Pkt_st->u8DataBuffer[3] != DevicId_Arr_TEST[1])
        return false;
      if (f_Pkt_st->u8DataBuffer[4] != DevicId_Arr_TEST[2])
        return false;
      if (f_Pkt_st->u8DataBuffer[5] != DevicId_Arr_TEST[3])
        return false;

      handle_1BS(f_Pkt_st);
      break;

    case u8RORG_RPS:
      // Get rid of messages not intended for us
      if (f_Pkt_st->u8DataBuffer[2] != DevicId_Arr_TEST[0])
        return false;
      if (f_Pkt_st->u8DataBuffer[3] != DevicId_Arr_TEST[1])
        return false;
      if (f_Pkt_st->u8DataBuffer[4] != DevicId_Arr_TEST[2])
        return false;
      if (f_Pkt_st->u8DataBuffer[5] != DevicId_Arr_TEST[3])
        return false;

#ifdef KDEBUG
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
      if (f_Pkt_st->u8DataBuffer[5] != DevicId_Arr_TEST[0])
        return false;
      if (f_Pkt_st->u8DataBuffer[6] != DevicId_Arr_TEST[1])
        return false;
      if (f_Pkt_st->u8DataBuffer[7] != DevicId_Arr_TEST[2])
        return false;
      if (f_Pkt_st->u8DataBuffer[8] != DevicId_Arr_TEST[3])
        return false;

#ifdef KDEBUG
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[5], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[6], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[7], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.println(f_Pkt_st->u8DataBuffer[8], HEX);
#endif

      handle_4BS(f_Pkt_st, profil, profil2nd, firstComObj, firstParameter);
      break;

    case u8RORG_VLD:

      // Get rid of messages not intended for us
      if (f_Pkt_st->u8DataBuffer[f_Pkt_st->u16DataLength - 5] != DevicId_Arr_TEST[0])
        return false;
      if (f_Pkt_st->u8DataBuffer[f_Pkt_st->u16DataLength - 4] != DevicId_Arr_TEST[1])
        return false;
      if (f_Pkt_st->u8DataBuffer[f_Pkt_st->u16DataLength - 3] != DevicId_Arr_TEST[2])
        return false;
      if (f_Pkt_st->u8DataBuffer[f_Pkt_st->u16DataLength - 2] != DevicId_Arr_TEST[3])
        return false;

#ifdef KDEBUG
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

      handle_RPS_Rocker(f_Pkt_st, profil, firstComObj, firstParameter, index);
      break;
    }
#else
    // Normal Funktion !!!!
    switch (knx.paramByte(ENO_CHProfilSelection + firstParameter))
    {
    case u8RORG_1BS:
#ifdef KDEBUG
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

#ifdef KDEBUG
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

#ifdef KDEBUG
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[5], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[6], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.print(f_Pkt_st->u8DataBuffer[7], HEX);
      SERIAL_PORT.print(" ");
      SERIAL_PORT.println(f_Pkt_st->u8DataBuffer[8], HEX);
#endif

      handle_4BS(f_Pkt_st, profil, profil2nd, firstComObj, firstParameter);
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

#ifdef KDEBUG
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

      handle_RPS_Rocker(f_Pkt_st, profil, firstComObj, firstParameter, index);
      break;
    }
#endif
    return true;
  }
#pragma GCC diagnostic pop // I don't want a warning, just because we don't do anything here

  /*******************************************************************************************
 *  1BS
 * 
 *******************************************************************************************/

  void handle_1BS(PACKET_SERIAL_TYPE *f_Pkt_st)
  {

    ONEBS_TELEGRAM_TYPE *l1bsTlg_p = (ONEBS_TELEGRAM_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);

    knx.getGroupObject(firstComObj).value(l1bsTlg_p->u81bsTelData.State, getDPT(VAL_DPT_1));

#ifdef KDEBUG
    SERIAL_PORT.print(F("detected: 1BS State: "));
    SERIAL_PORT.println(l1bsTlg_p->u81bsTelData.State);
#endif
  }
};

#endif /* EnOceanDevice_EMPTY_H_ */
