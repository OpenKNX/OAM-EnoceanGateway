
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

  uint8_t buttonStateSimulation1 = SIMULATE_NOTHING;
  uint8_t buttonStateSimulation2 = SIMULATE_NOTHING;
  bool buttonMessage1 = false;
  bool buttonMessage2 = false;
  uint32_t buttonLastPushTime1 = 0;
  uint32_t buttonLastPushTime2 = 0;
  uint8_t koIndex1;
  uint8_t koIndex2;

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
      SERIAL_PORT.println(knx.paramByte(ENO_CHProfilSelection1BS + firstParameter));
      break;
    case u8RORG_4BS:
      SERIAL_PORT.print(F("Profil: 4BS "));
      SERIAL_PORT.println(knx.paramByte(ENO_CHProfilSelection4BS + firstParameter));
      break;
    case u8RORG_RPS:
      SERIAL_PORT.print(F("Profil: RPS "));
      SERIAL_PORT.println(knx.paramByte(ENO_CHProfilSelectionRPS + firstParameter));
      break;
    case u8RORG_VLD:
      SERIAL_PORT.print(F("Profil: VLD "));
      SERIAL_PORT.println(knx.paramByte(ENO_CHProfilSelectionVLD + firstParameter));
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

    if (buttonStateSimulation1 == SIMULATE_RELEASE)
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
      send_RPS_Taster(enOcean.getBaseId() , koIndex2, buttonMessage2, true); // +koIndex zum Anpassen der BaseID damit jeder CH seine eigene ID hat
      buttonLastPushTime2 = millis();
      buttonStateSimulation2 = SIMULATE_RELEASE;
#ifdef KDEBUG
      SERIAL_PORT.print(F("Triggered "));
      SERIAL_PORT.print(firstComObj + 1);
      SERIAL_PORT.println(F(" Push"));
#endif
    }

    if (buttonStateSimulation2 == SIMULATE_RELEASE)
    {
      if (millis() - buttonLastPushTime2 >= SIMULATE_PAUSE_BEFORE_RELEASE)
      {
        send_RPS_Taster(enOcean.getBaseId() , koIndex2, buttonMessage2, false);
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
    switch (knx.paramByte(ENO_CHProfilSelection + firstParameter))
    {
    case u8RORG_VLD:
      switch (knx.paramByte(firstParameter + ENO_CHProfilSelectionVLD))
      {
      case D2_01:
        switch (koNr)
        {
        case 1: // Schalten Aktor CH1
          koIndex1 = (index*5) +1;
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
        case 2: // Schalten Aktor CH2
          koIndex2 = (index*5) +2;
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
        case 5: // Request Aktor Status
          getStatusActors(enOcean.getBaseId() , (index*5) +1);
          break;
        default:
          break;
        }
        break;
      }
      break;

    case u8RORG_Rocker:
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
    // Get rid of messages we can't handle
    if (f_Pkt_st->u8DataBuffer[0] != knx.paramByte(ENO_CHProfilSelection + firstParameter) && knx.paramByte(ENO_CHProfilSelection + firstParameter) != u8RORG_Rocker)
      return false;

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

      handle_4BS(f_Pkt_st, profil, firstComObj, firstParameter);
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
