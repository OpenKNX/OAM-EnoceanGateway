#pragma once

#include "EnoceanProfils.h"

void handle_VLD(PACKET_SERIAL_TYPE *f_Pkt_st, uint8_t profil, uint16_t firstComObj, uint16_t firstParameter)
{

      uint8_t mem;
      uint8_t mem2;
      uint8_t valueBat;
      uint16_t value;
      
      //float temp_s;
      float hum_s;
      float voc1_s;
      

      union float_value
      {
            float temp_s;
            float lux;
      }valuefloat;

      union flaot_value2
      {
            float voc2_s;
            float acc_value;
      }valuefloat2;
      

      union u32t_value
      {
            uint32_t counter;
            uint32_t lux;
      }valueU32;

      VLD_D2_01_TELEGRAM_CMD_04_TYPE *ActStatResp;
      VLD_D2_01_TELEGRAM_CMD_04_TYPE *ActStatResp_4_D2_01_0E;
      VLD_D2_01_TELEGRAM_CMD_07_TYPE *ActStatResp_7_D2_01_0E;
      VLD_D2_04_00_TELEGRAM *SenVal_D2_04;
      VLD_D2_03_0A_TELEGRAM *SenVal_D2_03_0A;
      VLD_D2_05_00_TELEGRAM_CMD_04_TYPE *ActStatResp_D2_05;
      // VLD_D2_14_00_TELEGRAM *SenVal_D2_14;
      VLD_D2_14_30_TELEGRAM *SenStat_D2_14_30;
      VLD_D2_14_41_TELEGRAM *SenStat_D2_14_41;
      VLD_D2_06_01_TELEGRAM *SenVal_D2_06_01;
      VLD_D2_06_50_Window_Status_0x01 *SenVal_D2_06_50_0x01;
      VLD_D2_06_50_Alarm_Status_0x02 *SenVal_D2_06_50_0x02;

      switch (knx.paramWord(firstParameter + ENO_CHProfilSelectionVLD))
      {
      case D2_01:
            //**************************************************************
            // ----------------- Profil: D2-01-xx --------------------------
            //**************************************************************
            switch (knx.paramWord(firstParameter + ENO_CHProfilVLD04))
            {
            case D2_01_0E:
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-01-0E"));
#endif
                  switch (f_Pkt_st->u8DataBuffer[1])
                  {
                  case 4: // CMD = 4   -->  Status AKTOR Channels
#ifdef KDEBUG
                        SERIAL_PORT.println(F("CMD = 0x04"));
#endif
                        mem = f_Pkt_st->u8DataBuffer[2];
                        f_Pkt_st->u8DataBuffer[2] = f_Pkt_st->u8DataBuffer[3];
                        f_Pkt_st->u8DataBuffer[3] = mem;

                        ActStatResp_4_D2_01_0E = (VLD_D2_01_TELEGRAM_CMD_04_TYPE *)&(f_Pkt_st->u8DataBuffer[2]);

                        if (ActStatResp_4_D2_01_0E->u8VldTelActResp2.IOChannel == 0) // Abfrage ob CH1
                        {
                              if (ActStatResp_4_D2_01_0E->u8VldTelActResp.outputValue == 0)
                                    knx.getGroupObject(firstComObj + 4).value(false, getDPT(VAL_DPT_1));
                              else
                                    knx.getGroupObject(firstComObj + 4).value(true, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                              SERIAL_PORT.print(F("State: "));
                              SERIAL_PORT.println(ActStatResp_4_D2_01_0E->u8VldTelActResp.outputValue);
#endif
                        }
                        break;
                  case 7:
#ifdef KDEBUG
                        SERIAL_PORT.println(F("CMD = 0x07"));
#endif
                        ActStatResp_7_D2_01_0E = (VLD_D2_01_TELEGRAM_CMD_07_TYPE *)&(f_Pkt_st->u8DataBuffer[2]);
                        uint32_t value = (f_Pkt_st->u8DataBuffer[3] << 24) | f_Pkt_st->u8DataBuffer[4] << 16 | f_Pkt_st->u8DataBuffer[5] << 8 | f_Pkt_st->u8DataBuffer[6];
                        switch (ActStatResp_7_D2_01_0E->u8VldTelActResp.UNIT)
                        {
                        case 0x00: // WS
                              /* code */
                              break;
                        case 0x01: // Wh
                              knx.getGroupObject(firstComObj + 5).value(value, Dpt(13, 1));
                              break;
                        case 0x02: // KWh
                              knx.getGroupObject(firstComObj + 6).value(value, Dpt(13, 1));
                              break;
                        case 0x03: // W
                              knx.getGroupObject(firstComObj + 7).value(value, Dpt(14, 1));
                              break;
                        case 0x04: // KW
                              knx.getGroupObject(firstComObj + 8).value(value, Dpt(9, 1));
                              break;
                        default:
                              break;
                        }
#ifdef KDEBUG
                        SERIAL_PORT.print(F("Unit: "));
                        SERIAL_PORT.println(ActStatResp_7_D2_01_0E->u8VldTelActResp.UNIT);
                        SERIAL_PORT.print(F("Value: "));
                        SERIAL_PORT.println(value);
#endif
                        break;
                  } // Ende SWITCH (f_Pkt_st->u8DataBuffer[1])
                  break;
            case D2_01_12:
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-01-12"));
#endif
                  switch (f_Pkt_st->u8DataBuffer[1])
                  {
                  case 4: // CMD = 4   -->  Status AKTOR Channels
// swap two bytes
#ifdef KDEBUG
                        SERIAL_PORT.println(F("CMD = 0x04"));
#endif
                        mem = f_Pkt_st->u8DataBuffer[2];
                        f_Pkt_st->u8DataBuffer[2] = f_Pkt_st->u8DataBuffer[3];
                        f_Pkt_st->u8DataBuffer[3] = mem;

                        ActStatResp = (VLD_D2_01_TELEGRAM_CMD_04_TYPE *)&(f_Pkt_st->u8DataBuffer[2]);

                        if (ActStatResp->u8VldTelActResp2.IOChannel == 0) // Abfrage ob CH1
                        {
                              if (ActStatResp->u8VldTelActResp.outputValue == 0)
                                    knx.getGroupObject(firstComObj + 2).value(false, getDPT(VAL_DPT_1));
                              else
                                    knx.getGroupObject(firstComObj + 2).value(true, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                              SERIAL_PORT.print(F("CH1: "));
                              SERIAL_PORT.println(ActStatResp->u8VldTelActResp.outputValue);
#endif
                        }
                        else if (ActStatResp->u8VldTelActResp2.IOChannel == 1) // Abfrage ob CH2
                        {
                              if (ActStatResp->u8VldTelActResp.outputValue == 0)
                                    knx.getGroupObject(firstComObj + 3).value(false, getDPT(VAL_DPT_1));
                              else
                                    knx.getGroupObject(firstComObj + 3).value(true, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                              SERIAL_PORT.print(F("CH2: "));
                              SERIAL_PORT.println(ActStatResp->u8VldTelActResp.outputValue);
#endif
                        }
                        break;
                  }
                  break; // ENDE CASE D2_01_12
            }
            break;

      case D2_03:
            //**************************************************************
            // ----------------- Profil: D2-03-xx --------------------------
            //**************************************************************
            switch (knx.paramWord(firstParameter + ENO_CHProfilVLD03))
            {
            case D2_03_0A:
                  SenVal_D2_03_0A = (VLD_D2_03_0A_TELEGRAM *)&(f_Pkt_st->u8DataBuffer[1]);
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-03-0A"));
#endif
                  // Batterie
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_03_0A->bat, getDPT(VAL_DPT_5));
                  // Buttons
                  switch (SenVal_D2_03_0A->value)
                  {
                  case 1: // single click
                        knx.getGroupObject(firstComObj + 6).value(true, getDPT(VAL_DPT_1));
                        break;
                  case 2: // double click
                        knx.getGroupObject(firstComObj + 7).value(true, getDPT(VAL_DPT_1));
                        break;
                  case 3: // long click
                        knx.getGroupObject(firstComObj + 8).value(true, getDPT(VAL_DPT_1));
                        break;
                  case 4: // long click release
                        knx.getGroupObject(firstComObj + 9).value(true, getDPT(VAL_DPT_1));
                        break;

                  default:
                        break;
                  }
                  break;

                  break; // ENDE CASE D2_03_0A
            }
            break; // ENDE CASE D2_03

      case D2_04:
            //**************************************************************
            // ----------------- Profil: D2-04-xx --------------------------
            //**************************************************************

            SenVal_D2_04 = (VLD_D2_04_00_TELEGRAM *)&(f_Pkt_st->u8DataBuffer[1]);

            valuefloat.temp_s = (float)(SenVal_D2_04->temp / 5.0);
            hum_s = (float)(SenVal_D2_04->hum / 2.0);
            voc1_s = (float)(SenVal_D2_04->voc) * 7.84;
            valuefloat2.voc2_s = (float)(SenVal_D2_04->voc) * 19.6;

            switch (knx.paramWord(firstParameter + ENO_CHProfilVLD04))
            {
            case D2_04_00:
                  knx.getGroupObject(firstComObj).value(voc1_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(valuefloat.temp_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-00"));
#endif
                  break;
            case D2_04_01:
                  knx.getGroupObject(firstComObj).value(voc1_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-01"));
#endif
                  break;
            case D2_04_02:
                  knx.getGroupObject(firstComObj).value(voc1_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(valuefloat.temp_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-02"));
#endif
                  break;
            case D2_04_03:
                  knx.getGroupObject(firstComObj).value(voc1_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(SenVal_D2_04, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj+2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-03"));
#endif
                  break;
            case D2_04_04:
                  knx.getGroupObject(firstComObj).value(voc1_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(valuefloat.temp_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj+2).value(hum_s, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-04"));
#endif
                  break;
            case D2_04_05:
                  knx.getGroupObject(firstComObj).value(voc1_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(valuefloat.temp_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-05"));
#endif
                  break;
            case D2_04_06:
                  knx.getGroupObject(firstComObj).value(voc1_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-06"));
#endif
                  break;
            case D2_04_07:
                  knx.getGroupObject(firstComObj).value(voc1_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-07"));
#endif
                  break;
            case D2_04_08:
                  knx.getGroupObject(firstComObj).value(valuefloat2.voc2_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(valuefloat.temp_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-08"));
#endif
                  break;
            case D2_04_09:
                  knx.getGroupObject(firstComObj).value(valuefloat2.voc2_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-09"));
#endif
                  break;
            case D2_04_10:
                  knx.getGroupObject(firstComObj).value(valuefloat2.voc2_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(valuefloat.temp_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-10"));
#endif
                  break;
            case D2_04_1A:
                  knx.getGroupObject(firstComObj).value(valuefloat2.voc2_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(valuefloat.temp_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-1A"));
#endif
                  break;
            case D2_04_1B:
                  knx.getGroupObject(firstComObj).value(valuefloat2.voc2_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(valuefloat.temp_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-1B"));
#endif
                  break;
            case D2_04_1C:
                  knx.getGroupObject(firstComObj).value(valuefloat2.voc2_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(valuefloat.temp_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-1C"));
#endif
                  break;
            case D2_04_1D:
                  knx.getGroupObject(firstComObj).value(valuefloat2.voc2_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-1D"));
#endif
                  break;
            case D2_04_1E:
                  knx.getGroupObject(firstComObj).value(valuefloat2.voc2_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-1E"));
#endif
                  break;

            default:
                  break;
            } // ENDE SWITCH D2-04-xx
#ifdef KDEBUG
            SERIAL_PORT.print(F("Temp: "));
            SERIAL_PORT.println(valuefloat.temp_s);
            SERIAL_PORT.print(F("Hum: "));
            SERIAL_PORT.println(hum_s);
            SERIAL_PORT.print(F("VOC1: "));
            SERIAL_PORT.println(voc1_s);
            SERIAL_PORT.print(F("VOC2: "));
            SERIAL_PORT.println(valuefloat2.voc2_s);
            SERIAL_PORT.print(F("Batterie: "));
            SERIAL_PORT.println(SenVal_D2_04->u8VldTelSenSta.batt);
            SERIAL_PORT.print(F("Day: "));
            SERIAL_PORT.println(SenVal_D2_04->u8VldTelSenSta.daynight);
#endif

            break; // ENDE case D2_04_xx:

      case D2_05:
            //**************************************************************
            // ----------------- Profil: D2-05-xx --------------------------
            //**************************************************************
            switch (f_Pkt_st->u16DataLength)
            {
            case 3: // 3 Byte länge
#ifdef KDEBUG
                  SERIAL_PORT.print(F("3Byte "));
#endif

                  ActStatResp_D2_05 = (VLD_D2_05_00_TELEGRAM_CMD_04_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);

                  switch (ActStatResp_D2_05->u8VldTelSenSta.CMD)
                  {
                  case 4: // CMD = 4   -->  Status Jaousie Angle / Pos
                          // swap two bytes
                        mem = f_Pkt_st->u8DataBuffer[2];
                        f_Pkt_st->u8DataBuffer[2] = f_Pkt_st->u8DataBuffer[3];
                        f_Pkt_st->u8DataBuffer[3] = mem;

                        knx.getGroupObject(firstComObj).value(ActStatResp_D2_05->angle, getDPT(VAL_DPT_5));
                        knx.getGroupObject(firstComObj + 1).value(ActStatResp_D2_05->pos, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                        SERIAL_PORT.print(F(" CMD=4 "));
                        SERIAL_PORT.print(F(" Pos: "));
                        SERIAL_PORT.print(ActStatResp_D2_05->pos);
                        SERIAL_PORT.print(F(" | Angle: "));
                        SERIAL_PORT.println(ActStatResp_D2_05->angle);
#endif

                        break;
                  }

                  break;

            default:
                  break;
            }
            break; // ENDE D2-05

      case D2_06:
            //**************************************************************
            // ----------------- Profil: D2-06-xx --------------------------
            //**************************************************************
            switch (knx.paramWord(firstParameter + ENO_CHProfilVLD06))
            {
            //**************************************************************
            // ----------------- Profil: D2-06-01 --------------------------
            //**************************************************************
            case D2_06_01:
                  SenVal_D2_06_01 = (VLD_D2_06_01_TELEGRAM *)&(f_Pkt_st->u8DataBuffer[1]);
                  if (SenVal_D2_06_01->u8MT == 0x00)
                  {
#ifdef KDEBUG
                        SERIAL_PORT.println(F("Profil: D2-06-01"));
#endif
                        // Alarm
                        if (SenVal_D2_06_01->u8VldTelAlarm.BAL || SenVal_D2_06_01->u8VldTelAlarm.PPAL)
                        {
                              knx.getGroupObject(firstComObj + 3).value(true, getDPT(VAL_DPT_1));
                        }
                        else
                        {
                              knx.getGroupObject(firstComObj + 3).value(false, getDPT(VAL_DPT_1));
                        }
#ifdef KDEBUG
                        SERIAL_PORT.print(F("Alarm: "));
                        SERIAL_PORT.println(SenVal_D2_06_01->u8VldTelAlarm.BAL);
#endif
                        // Window State
                        // closed = State1 = 0 & State2 = 0
                        if (SenVal_D2_06_01->u8VldTelWindow.WS == 0x1 && SenVal_D2_06_01->u8VldTelWindow.HP == 0x2) // Window not Tilted = close & Handle down = close
                        {
                              knx.getGroupObject(firstComObj + 1).value(false, getDPT(VAL_DPT_1)); // KO Fenster Status 1
                              knx.getGroupObject(firstComObj + 2).value(false, getDPT(VAL_DPT_1)); // KO Fenster Status 2
                        }
                        else if (SenVal_D2_06_01->u8VldTelWindow.WS == 0x2) // Window  Tilted = open
                        {
                              knx.getGroupObject(firstComObj + 1).value(true, getDPT(VAL_DPT_1));  // KO Fenster Status 1
                              knx.getGroupObject(firstComObj + 2).value(false, getDPT(VAL_DPT_1)); // KO Fenster Status 2
                        }
                        else
                        {
                              knx.getGroupObject(firstComObj + 1).value(true, getDPT(VAL_DPT_1)); // KO Fenster Status 1
                              knx.getGroupObject(firstComObj + 2).value(true, getDPT(VAL_DPT_1)); // KO Fenster Status 2
                        }

                        // Position of Handle
                        // if (SenVal_D2_06_01->u8VldTelWindow.HP == 0x1) // Handle up = open
                        // if (SenVal_D2_06_01->u8VldTelWindow.HP == 0x2) // Handle down = close
                        // if (SenVal_D2_06_01->u8VldTelWindow.HP == 0x3) // Handle left = open
                        // if (SenVal_D2_06_01->u8VldTelWindow.HP == 0x4) // Handle right = open
                        // state of Window
                        // if (SenVal_D2_06_01->u8VldTelWindow.WS == 0x1) // Window not Tilted = close
                        // if (SenVal_D2_06_01->u8VldTelWindow.WS == 0x2) // Window  Tilted = open
#ifdef KDEBUG
                        SERIAL_PORT.print(F("handle Pos: "));
                        SERIAL_PORT.println(SenVal_D2_06_01->u8VldTelWindow.HP);
                        SERIAL_PORT.print(F("Window tilted: "));
                        SERIAL_PORT.println(SenVal_D2_06_01->u8VldTelWindow.WS - 1);
#endif
                        // Button Right
                        if (SenVal_D2_06_01->u8VldTelButton.BR == 0x1)
                        {
                              mem = true;
                              knx.getGroupObject(firstComObj + 8).value(mem, getDPT(VAL_DPT_1));
                        }
                        if (SenVal_D2_06_01->u8VldTelButton.BR == 0x2)
                        {
                              mem = false;
                              knx.getGroupObject(firstComObj + 8).value(mem, getDPT(VAL_DPT_1));
                        }
                        if (SenVal_D2_06_01->u8VldTelButton.BR == 0x0)
                        {
                              knx.getGroupObject(firstComObj + 8).value(mem, getDPT(VAL_DPT_1));
                        }
#ifdef KDEBUG
                        SERIAL_PORT.print(F("But R: "));
                        SERIAL_PORT.println(SenVal_D2_06_01->u8VldTelButton.BR);
#endif
                        // Button Left
                        if (SenVal_D2_06_01->u8VldTelButton.BL == 0x1)
                        {
                              mem2 = true;
                              knx.getGroupObject(firstComObj + 9).value(mem2, getDPT(VAL_DPT_1));
                        }
                        if (SenVal_D2_06_01->u8VldTelButton.BL == 0x2)
                        {
                              mem2 = false;
                              knx.getGroupObject(firstComObj + 9).value(mem2, getDPT(VAL_DPT_1));
                        }
                        if (SenVal_D2_06_01->u8VldTelButton.BL == 0x0)
                        {
                              knx.getGroupObject(firstComObj + 9).value(mem2, getDPT(VAL_DPT_1));
                        }
#ifdef KDEBUG
                        SERIAL_PORT.print(F("But L: "));
                        SERIAL_PORT.println(SenVal_D2_06_01->u8VldTelButton.BL);
#endif
                        // Motion
                        knx.getGroupObject(firstComObj).value(SenVal_D2_06_01->u8VldTelMotionMode.M, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                        SERIAL_PORT.print(F("Motion Det: "));
                        SERIAL_PORT.println(SenVal_D2_06_01->u8VldTelMotionMode.M);
#endif
                        // Temp
                        valuefloat.temp_s = SenVal_D2_06_01->u8Temp * 0.320 - 20.0;
                        knx.getGroupObject(firstComObj + 6).value(valuefloat.temp_s, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                        SERIAL_PORT.print(F("Temp: "));
                        SERIAL_PORT.println(valuefloat.temp_s);
#endif
                        // Hum
                        hum_s = SenVal_D2_06_01->u8Hum / 2;
                        knx.getGroupObject(firstComObj + 7).value(hum_s, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                        SERIAL_PORT.print(F("Hum: "));
                        SERIAL_PORT.println(hum_s);
#endif
                        // Lux
                        value = (((uint16_t)SenVal_D2_06_01->u8LuxMSB) << 8 | SenVal_D2_06_01->u8LuxLSB);
                        knx.getGroupObject(firstComObj + 5).value(value, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                        SERIAL_PORT.print(F("Lux: "));
                        SERIAL_PORT.println(value);
#endif
                        // Bat
                        valueBat = SenVal_D2_06_01->u8VldTelBat.BS * 5;
                        knx.getGroupObject(firstComObj + 4).value(valueBat, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                        SERIAL_PORT.print(F("Bat(%): "));
                        SERIAL_PORT.println(valueBat);
#endif
                  }
                  break;
            //**************************************************************
            // ----------------- Profil: D2-06-50 --------------------------
            //**************************************************************
            case D2_06_50:

                  SenVal_D2_06_50_0x01 = (VLD_D2_06_50_Window_Status_0x01 *)&(f_Pkt_st->u8DataBuffer[1]);

                  if (SenVal_D2_06_50_0x01->u8MT == 0x01)
                  {
#ifdef KDEBUG
                        SERIAL_PORT.println(F("Profil: D2-06-50 (0x01)"));
#endif
                        // Window State
                        knx.getGroupObject(firstComObj + 1).value(SenVal_D2_06_50_0x01->u8VldTelWindow.WDS, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                        SERIAL_PORT.print(F("Window State: "));
                        SERIAL_PORT.println(SenVal_D2_06_50_0x01->u8VldTelWindow.WDS);
#endif
                        // Counter Status
                        valueU32.counter = (SenVal_D2_06_50_0x01->u8CT_4 << 24) | (SenVal_D2_06_50_0x01->u8CT_3 << 16) | (SenVal_D2_06_50_0x01->u8CT_2 << 8) | SenVal_D2_06_50_0x01->u8CT_1;
                        knx.getGroupObject(firstComObj + 5).value(valueU32.counter, getDPT(VAL_DPT_12));
#ifdef KDEBUG
                        SERIAL_PORT.print(F("State Counter: "));
                        SERIAL_PORT.println(valueU32.counter);
#endif
                        // Change Battery
                        knx.getGroupObject(firstComObj + 4).value(SenVal_D2_06_50_0x01->u8VldTelBattery.CB, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                        SERIAL_PORT.print(F("change Bat: "));
                        SERIAL_PORT.println(SenVal_D2_06_50_0x01->u8VldTelBattery.CB);
#endif
                        // Battery State %
                        knx.getGroupObject(firstComObj).value(SenVal_D2_06_50_0x01->u8VldTelBattery.BS, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                        SERIAL_PORT.print(F("Bat(%): "));
                        SERIAL_PORT.println(SenVal_D2_06_50_0x01->u8VldTelBattery.BS);
#endif
                        // Status Bits
                        knx.getGroupObject(firstComObj + 9).value(SenVal_D2_06_50_0x01->u8StatusBits, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                        SERIAL_PORT.print(F("Status Byte: "));
                        SERIAL_PORT.println(SenVal_D2_06_50_0x01->u8StatusBits, BIN);
#endif
                  }
                  if (SenVal_D2_06_50_0x01->u8MT == 0x02)
                  {
                        SenVal_D2_06_50_0x02 = (VLD_D2_06_50_Alarm_Status_0x02 *)&(f_Pkt_st->u8DataBuffer[1]);
#ifdef KDEBUG
                        SERIAL_PORT.println(F("Profil: D2-06-50 (0x02)"));
                        SERIAL_PORT.print(F("Alarm: "));
                        SERIAL_PORT.println(SenVal_D2_06_50_0x02->u8VldTelAlarm.BA);
#endif
                        knx.getGroupObject(firstComObj + 3).value(SenVal_D2_06_50_0x02->u8VldTelAlarm.BA, getDPT(VAL_DPT_1));
                  }
                  break;

            default:
                  break;
            }
            break; // ENDE D2-06

      case D2_14:
            //**************************************************************
            // ----------------- Profil: D2-14-xx --------------------------
            //**************************************************************
            switch (knx.paramWord(firstParameter + ENO_CHProfilVLD14))
            {
            //**************************************************************
            // ----------------- Profil: D2-14-30 --------------------------
            //**************************************************************
            case D2_14_30:
                  SenStat_D2_14_30 = (VLD_D2_14_30_TELEGRAM *)&(f_Pkt_st->u8DataBuffer[1]);
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-14-30"));
#endif
                  // Smoke Alarm
                  knx.getGroupObject(firstComObj + 3).value(SenStat_D2_14_30->u8VldTelSenSta5.smokeAlarm, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Smoke Alarm: "));
                  SERIAL_PORT.println(SenStat_D2_14_30->u8VldTelSenSta5.smokeAlarm);
#endif
                  // Status Bits
                  knx.getGroupObject(firstComObj + 9).value(SenStat_D2_14_30->u8VldTelSenSta5.statusbits, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Status Bits: "));
                  SERIAL_PORT.println(SenStat_D2_14_30->u8VldTelSenSta5.statusbits);
#endif
                  // Energy Storage (Battery)
                  knx.getGroupObject(firstComObj + 4).value(SenStat_D2_14_30->u8VldTelSenSta4.ES, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Battery: "));
                  SERIAL_PORT.println(SenStat_D2_14_30->u8VldTelSenSta4.ES);
#endif
                  // IAQTH indoor AIR Quality
                  knx.getGroupObject(firstComObj + 7).value(SenStat_D2_14_30->u8VldTelSenSta.IAQTH, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Air Quality: "));
                  SERIAL_PORT.println(SenStat_D2_14_30->u8VldTelSenSta.IAQTH);
#endif
                  // HUM
                  mem = (SenStat_D2_14_30->u8VldTelSenSta2.HUM_MSB << 7) | SenStat_D2_14_30->u8VldTelSenSta1.HUM_LSB;
                  hum_s = mem / 2.0;
                  knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Hum: "));
                  SERIAL_PORT.println(hum_s);
#endif
                  // TEMP
                  mem = (SenStat_D2_14_30->u8VldTelSenSta3.Temp_MSB << 7) | SenStat_D2_14_30->u8VldTelSenSta2.Temp_LSB;
                  valuefloat.temp_s = mem / 5.0;
                  knx.getGroupObject(firstComObj + 1).value(valuefloat.temp_s, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Temp: "));
                  SERIAL_PORT.println(valuefloat.temp_s);
#endif
                  // HCI
                  mem = (SenStat_D2_14_30->u8VldTelSenSta1.HCI_MSB << 1) | SenStat_D2_14_30->u8VldTelSenSta.HCI_LSB;
                  knx.getGroupObject(firstComObj + 6).value(mem, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("HCI: "));
                  SERIAL_PORT.println(mem);
#endif
                  // RPLT
                  mem = (SenStat_D2_14_30->u8VldTelSenSta4.RPLT_MSB << 7) | SenStat_D2_14_30->u8VldTelSenSta3.RPLT_LSB;
                  knx.getGroupObject(firstComObj + 5).value(mem, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("RPLT: "));
                  SERIAL_PORT.println(mem);
#endif
                  // time since last Event
                  mem = (SenStat_D2_14_30->u8VldTelSenSta5.LastEvent_MSB << 5) | SenStat_D2_14_30->u8VldTelSenSta4.LastEvent_LSB;
                  knx.getGroupObject(firstComObj + 8).value(mem, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Time last Event: "));
                  SERIAL_PORT.println(mem);
#endif
                  break;

            //**************************************************************
            // ----------------- Profil: D2-14-41 --------------------------
            //**************************************************************
            case D2_14_41:
                  SenStat_D2_14_41 = (VLD_D2_14_41_TELEGRAM *)&(f_Pkt_st->u8DataBuffer[1]);
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-14-41"));
#endif
                  // Temp-Sensor  
                  value = SenStat_D2_14_41->Temp_MSB<<2;
                  value = value^SenStat_D2_14_41->u8VldTelSenSta7.Temp_LSB;
                  valuefloat.temp_s = (float)(value / 10.230) - 40;
                  knx.getGroupObject(firstComObj + 1).value(valuefloat.temp_s, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Temp: "));
                  SERIAL_PORT.print(valuefloat.temp_s);
                  SERIAL_PORT.print(F("  RAW: "));
                  SERIAL_PORT.println(value);
#endif
                  // Hum-Sensor  
                  mem = SenStat_D2_14_41->u8VldTelSenSta7.Hum_MSB << 2;
                  mem = mem^SenStat_D2_14_41->u8VldTelSenSta6.Hum_LSB;
                  hum_s = (float)mem/2;
                  knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Hum: "));
                  SERIAL_PORT.print(hum_s);
                  SERIAL_PORT.print(F("  RAW: "));
                  SERIAL_PORT.println(mem);
#endif
                  // LUX-Sensor  
                  valueU32.lux = SenStat_D2_14_41->u8VldTelSenSta6.LUX_MSB << 8;
                  valueU32.lux = valueU32.lux^SenStat_D2_14_41->LUX;
                  valueU32.lux = valueU32.lux << 3;
                  valueU32.lux = valueU32.lux^SenStat_D2_14_41->u8VldTelSenSta4.LUX_LSB;
                  valueU32.lux = valueU32.lux & 0x1ffff;
                  valuefloat.lux = (float)valueU32.lux;
                  knx.getGroupObject(firstComObj + 5).value(valueU32.lux, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("LUX: "));
                  SERIAL_PORT.println(valuefloat.lux);
                  SERIAL_PORT.print(F("  RAW: "));
                  SERIAL_PORT.println(valueU32.lux);
#endif
                  // Sensor Status
                  knx.getGroupObject(firstComObj + 9).value(SenStat_D2_14_41->u8VldTelSenSta4.SensorStatus, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Sensor Status: "));
                  SERIAL_PORT.println(SenStat_D2_14_41->u8VldTelSenSta4.SensorStatus);
#endif
                  // ACC_X
                  value = SenStat_D2_14_41->u8VldTelSenSta4.ACCX_MSB << 7;
                  value = value^SenStat_D2_14_41->u8VldTelSenSta3.ACCX_LSB;
                  valuefloat2.acc_value = (value-500)*0.005;
                  knx.getGroupObject(firstComObj + 6).value(valuefloat2.acc_value, getDPT(VAL_DPT_14));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("ACC_X: "));
                  SERIAL_PORT.println(valuefloat2.acc_value);
                  SERIAL_PORT.print(F("  RAW: "));
                  SERIAL_PORT.println(value);
#endif                  
                  // ACC_Y
                  value = SenStat_D2_14_41->u8VldTelSenSta3.ACCY_MSB << 8;
                  value = value^SenStat_D2_14_41->u8VldTelSenSta2.ACCY_MID;
                  value = (value << 1);
                  value = value^SenStat_D2_14_41->u8VldTelSenSta1.ACCY_LSB;
                  valuefloat2.acc_value = (value-500)*0.005;
                  knx.getGroupObject(firstComObj + 7).value(valuefloat2.acc_value, getDPT(VAL_DPT_14));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("ACC_Y: "));
                  SERIAL_PORT.println(valuefloat2.acc_value);
                  SERIAL_PORT.print(F("  RAW: "));
                  SERIAL_PORT.println(value);
#endif    
                  // ACC_Z
                  value = SenStat_D2_14_41->u8VldTelSenSta1.ACCZ_MSB << 3;
                  value = value^SenStat_D2_14_41->u8VldTelSenSta.ACCZ_LSB;
                  valuefloat2.acc_value = (value-500)*0.005;
                  knx.getGroupObject(firstComObj + 8).value(valuefloat2.acc_value, getDPT(VAL_DPT_14));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("ACC_Z: "));
                  SERIAL_PORT.println(valuefloat2.acc_value);
                  SERIAL_PORT.print(F("  RAW: "));
                  SERIAL_PORT.println(value);
#endif    

                  // Contact
                  knx.getGroupObject(firstComObj + 3).value(!SenStat_D2_14_41->u8VldTelSenSta.contact, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Contact: "));
                  SERIAL_PORT.println(!SenStat_D2_14_41->u8VldTelSenSta.contact);
#endif
                  break;

            default:
                  break;
            }
            break; // ENDE D2-14
      default:
#ifdef KDEBUG
            SERIAL_PORT.println(F("Profil: ERROR"));
#endif
            break;
      } // ENDE D2-xx
}
