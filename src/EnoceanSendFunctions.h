#pragma once

#include "EnOcean.h"
#include "EnoceanGateway.h"
#include "hardwareENO.h"

void send_4BS_Msg(uint8_t *fui8_BaseID_p, uint8_t Index, uint8_t *inputs, uint8_t baseID_CH)
{
    PACKET_SERIAL_TYPE l_TestPacket_st;
    uint8_t l_TestBuf_p[10];
    l_TestPacket_st.u16DataLength = 0x000A;
    l_TestPacket_st.u8OptionLength = 0x00;
    l_TestPacket_st.u8Type = u8RADIO_ERP1;
    l_TestPacket_st.u8DataBuffer = &l_TestBuf_p[0];

    l_TestBuf_p[0] = u8RORG_4BS;

    l_TestBuf_p[1] = inputs[0];
    l_TestBuf_p[2] = inputs[1];
    l_TestBuf_p[3] = inputs[2];
    l_TestBuf_p[4] = inputs[3];

    for (int i = 0; i < 4; i++)
    {
        l_TestBuf_p[i + 5] = fui8_BaseID_p[i];
    }
    if (baseID_CH <= 4) // anpassen der Sende ID -> baseID_CH max 4 !!!
        l_TestBuf_p[8] = l_TestBuf_p[8] + baseID_CH;

    enOcean.sendPacket(&l_TestPacket_st);
}

void send_RPS_Taster(uint8_t *fui8_BaseID_p, boolean state, boolean pressed, uint8_t baseID_CH)
{
    PACKET_SERIAL_TYPE l_TestPacket_st;
    uint8_t l_TestBuf_p[7];
    l_TestPacket_st.u16DataLength = 0x0007;
    l_TestPacket_st.u8OptionLength = 0x00;
    l_TestPacket_st.u8Type = u8RADIO_ERP1;
    l_TestPacket_st.u8DataBuffer = &l_TestBuf_p[0];

    l_TestBuf_p[0] = u8RORG_RPS;

    if (state == true)
    {
        if (pressed == true)
        {
            l_TestBuf_p[1] = 0x10;
            l_TestBuf_p[6] = 0x30;
        }
        else
        {
            l_TestBuf_p[1] = 0x00;
            l_TestBuf_p[6] = 0x20;
        }
    }
    else
    {
        if (pressed == true)
        {
            l_TestBuf_p[1] = 0x30;
            l_TestBuf_p[6] = 0x30;
        }
        else
        {
            l_TestBuf_p[1] = 0x00;
            l_TestBuf_p[6] = 0x20;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        l_TestBuf_p[i + 2] = fui8_BaseID_p[i];
    }

    if (baseID_CH <= 4) // anpassen der Sende ID -> baseID_CH max 4 !!!
        l_TestBuf_p[5] = l_TestBuf_p[5] + baseID_CH;

#ifdef KDEBUG
    SERIAL_PORT.print(F("Send-ID: "));
    SERIAL_PORT.print(l_TestBuf_p[2], HEX);
    SERIAL_PORT.print(F(" "));
    SERIAL_PORT.print(l_TestBuf_p[3], HEX);
    SERIAL_PORT.print(F(" "));
    SERIAL_PORT.print(l_TestBuf_p[4], HEX);
    SERIAL_PORT.print(F(" "));
    SERIAL_PORT.println(l_TestBuf_p[5], HEX);
#endif

    enOcean.sendPacket(&l_TestPacket_st);
}

void getStatusActors(uint8_t *mySenderId, uint8_t idExtra)
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
    lu8SndBuf[6] = mySenderId[3] + idExtra;
    lu8SndBuf[7] = 0x00;
    // optional data
    lu8SndBuf[8] = 0x03;
    lu8SndBuf[9] = 0xFF;
    lu8SndBuf[10] = 0xFF;
    lu8SndBuf[11] = 0xFF;
    lu8SndBuf[12] = 0xFF;
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
        SERIAL_PORT.print(F("Requested status"));
#endif
    }
}