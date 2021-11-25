#ifndef ENOCEANDEVICE_T_H_VOC_H_
#define ENOCEANDEVICE_T_H_VOC_H_

#include "EnOcean.h"
#include "hardwareENO.h"

// ################################################
// ### DEBUG CONFIGURATION
// ################################################
// DEBUG-MODE
//#define KDEBUG // comment this line to disable DEBUG mode

class EnOceanDevice_T_H_VOC : public IEnOceanDevice
{
private:
  uint32_t sensorId = 0;
  uint8_t sensorId_Arr[4] = {0, 0, 0, 0};

  uint32_t nextPollTime = 0;
  uint32_t pollFrequencyMillis = (uint32_t)600000; // 10 minutes

  uint8_t valueBat;
  float temp_s;
  float hum_s;
  float voc_s;

  bool status_request = false;

public:
  EnOceanDevice_T_H_VOC()
  {
    numberOfComObjects = 4;
    numberOfParameters = 2;
  }

  uint8_t getSensorID(uint8_t pos)
  {
    return sensorId_Arr[pos];
  }

  // pre-configure device, set sender-id etc.
  void init(uint8_t startAtComObj, uint8_t startAtParameter)
  {
    firstComObj = startAtComObj;
    firstParameter = startAtParameter;
    // we only store the first comObj as this information is sufficient to identify all the com-objects

    // regarding configuration:
    // sensorId = ((uint32_t) Konnekting.getUINT32Param(startAtParameter));  //***************************************************************
    sensorId_Arr[0] = sensorId >> 24;
    sensorId_Arr[1] = sensorId >> 16;
    sensorId_Arr[2] = sensorId >> 8;
    sensorId_Arr[3] = sensorId;

#ifdef KDEBUG
    SERIAL_PORT.print("I am THVOC: ");
    for (int i = 0; i < BASEID_BYTES; i++)
    {
      SERIAL_PORT.print("0x");
      SERIAL_PORT.print(sensorId_Arr[i]);
    }
    SERIAL_PORT.println("");
#endif

    // ignore second config-item for the moment
  }

  void getStatusActors(uint8_t *mySenderId)
  {
    PACKET_SERIAL_TYPE lRdBaseIDPkt_st;

    uint8_t lu8SndBuf[15];

    lRdBaseIDPkt_st.u16DataLength = 0x0008;
    lRdBaseIDPkt_st.u8OptionLength = 0x07;
    lRdBaseIDPkt_st.u8Type = u8RADIO_ERP1;
    lRdBaseIDPkt_st.u8DataBuffer = &lu8SndBuf[0];

    lu8SndBuf[0] = u8RORG_VLD;
    lu8SndBuf[1] = VLD_CMD_ID_03;
    lu8SndBuf[2] = 0x1e;

    lu8SndBuf[3] = mySenderId[0];
    lu8SndBuf[4] = mySenderId[1];
    lu8SndBuf[5] = mySenderId[2];
    lu8SndBuf[6] = mySenderId[3];
    lu8SndBuf[7] = 0x00;
    // optional data
    lu8SndBuf[8] = 0x03;
    lu8SndBuf[9] = getSensorID(0);
    lu8SndBuf[10] = getSensorID(1);
    lu8SndBuf[11] = getSensorID(2);
    lu8SndBuf[12] = getSensorID(3);
    lu8SndBuf[13] = 0x00;
    lu8SndBuf[14] = 0x00;

    if (!enOcean.sendPacket(&lRdBaseIDPkt_st))
    {
#ifdef KDEBUG
      SERIAL_PORT.println("Sending telegram failed.");
#endif
    }
    else
    {
#ifdef KDEBUG
      SERIAL_PORT.print("Requested status from Fensterkontakt");
      SERIAL_PORT.println(sensorId, HEX);
#endif
    }

    nextPollTime = millis() + pollFrequencyMillis;
  }

  // things that need to be done regularly
  void task()
  {
    if (status_request == false)
    {
      status_request = true;
      getStatusActors(enOcean.getBaseId());
    }
    if (millis() > nextPollTime)
      getStatusActors(enOcean.getBaseId());
  }

  // something happened on the bus, let's react
#pragma GCC diagnostic push // I don't want a warning, just because we don't do anything here
#pragma GCC diagnostic ignored "-Wunused-parameter"

  void handleKnxEvents(byte index)
  {
    // nothing to listen for from Knx
    return;
  }

#pragma GCC diagnostic pop

  // decode EnOcean message. Fail fast!
  bool handleEnOceanPacket(PACKET_SERIAL_TYPE *f_Pkt_st)
  {
    if (f_Pkt_st->u8DataBuffer[0] != u8RORG_VLD)
      return false;

    //      {
    //        // swap two bytes
    //        uint8_t temp;
    //        temp = f_Pkt_st->u8DataBuffer[2];
    //        f_Pkt_st->u8DataBuffer[2] = f_Pkt_st->u8DataBuffer[3];
    //        f_Pkt_st->u8DataBuffer[3] = temp;
    //      }
    VLD_D2_04_00_TELEGRAM *SenValResp = (VLD_D2_04_00_TELEGRAM *)&(f_Pkt_st->u8DataBuffer[1]);

#ifdef KDEBUG
    SERIAL_PORT.print("ID:");
    SERIAL_PORT.print(SenValResp->u8SenderId_p[0], HEX);
    SERIAL_PORT.print(SenValResp->u8SenderId_p[1], HEX);
    SERIAL_PORT.print(SenValResp->u8SenderId_p[2], HEX);
    SERIAL_PORT.println(SenValResp->u8SenderId_p[3], HEX);
#endif

    // Get rid of messages not intended for us
    if (SenValResp->u8SenderId_p[0] != sensorId_Arr[0])
      return false;
    if (SenValResp->u8SenderId_p[1] != sensorId_Arr[1])
      return false;
    if (SenValResp->u8SenderId_p[2] != sensorId_Arr[2])
      return false;
    if (SenValResp->u8SenderId_p[3] != sensorId_Arr[3])
      return false;

#ifdef KDEBUG
    SERIAL_PORT.print("detect: Sensor_");
    SERIAL_PORT.println(sensorId);
#endif

    valueBat = SenValResp->u8VldTelSenSta.batt;
    temp_s = (float)(SenValResp->temp / 5.0);
    hum_s = (float)(SenValResp->hum / 2.0);
    voc_s = (float)(SenValResp->voc);

    //Knx.write(firstComObj, temp_s );      //***************************************************************
    //Knx.write(firstComObj + 1, hum_s);
    //Knx.write(firstComObj + 2, voc_s);

#ifdef KDEBUG
    SERIAL_PORT.print("Temp: ");
    SERIAL_PORT.println(SenValResp->temp / 5.0, 1);
    SERIAL_PORT.print("Hum: ");
    SERIAL_PORT.println(SenValResp->hum / 2.0, 1);
    SERIAL_PORT.print("VOC: ");
    SERIAL_PORT.println(SenValResp->voc / 2.0, 1);
    SERIAL_PORT.print("Batterie: ");
    SERIAL_PORT.println(valueBat);
#endif

    return false;
  }
};

#endif /* ENOCEANDEVICE_T_H_VOC_H_ */
