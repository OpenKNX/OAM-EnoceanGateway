#pragma once

#include "EnoceanProfils.h"

void handle_VLD(PACKET_SERIAL_TYPE *f_Pkt_st, uint8_t profil, uint8_t firstComObj, uint8_t firstParameter)
{

      uint8_t mem;
      uint8_t valueBat;
      float temp_s;
      float hum_s;
      float voc1_s;
      float voc2_s;

      VLD_D2_01_TELEGRAM_CMD_04_TYPE *ActStatResp;
      VLD_D2_04_00_TELEGRAM *SenVal_D2_04;
      VLD_D2_05_00_TELEGRAM_CMD_04_TYPE *ActStatResp_D2_05;

      switch (knx.paramByte(firstParameter + ENO_CHProfilSelectionVLD))
      {
      case D2_01:
            //**************************************************************
            // ----------------- Profil: D2-01-xx --------------------------
            //**************************************************************
            switch (knx.paramByte(firstParameter + ENO_CHProfilVLD04))
            {
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
                              if(ActStatResp->u8VldTelActResp.outputValue == 0)
                              knx.getGroupObject(firstComObj +2).value(false, getDPT(VAL_DPT_1));
                              else
                              knx.getGroupObject(firstComObj +2).value(true, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                              SERIAL_PORT.print(F("CH1: "));
                              SERIAL_PORT.println(ActStatResp->u8VldTelActResp.outputValue);
#endif
                        }
                        else if (ActStatResp->u8VldTelActResp2.IOChannel == 1) // Abfrage ob CH2
                        {
                              if(ActStatResp->u8VldTelActResp.outputValue == 0)
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

      case D2_04:
            //**************************************************************
            // ----------------- Profil: D2-04-xx --------------------------
            //**************************************************************

            SenVal_D2_04 = (VLD_D2_04_00_TELEGRAM *)&(f_Pkt_st->u8DataBuffer[1]);

            temp_s = (float)(SenVal_D2_04->temp / 5.0);
            hum_s = (float)(SenVal_D2_04->hum / 2.0);
            voc1_s = (float)(SenVal_D2_04->voc) * 7.84;
            voc2_s = (float)(SenVal_D2_04->voc) * 19.6;

            switch (knx.paramByte(firstParameter + ENO_CHProfilVLD04))
            {
            case D2_04_00:
                  knx.getGroupObject(firstComObj).value(voc1_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-00"));
#endif
                  break;
            case D2_04_01:
                  knx.getGroupObject(firstComObj).value(voc1_s, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-01"));
#endif
                  break;
            case D2_04_02:
                  knx.getGroupObject(firstComObj).value(voc1_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-02"));
#endif
                  break;
            case D2_04_03:
                  knx.getGroupObject(firstComObj).value(voc1_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(SenVal_D2_04, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj+2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-03"));
#endif
                  break;
            case D2_04_04:
                  knx.getGroupObject(firstComObj).value(voc1_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj+2).value(hum_s, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-04"));
#endif
                  break;
            case D2_04_05:
                  knx.getGroupObject(firstComObj).value(voc1_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-05"));
#endif
                  break;
            case D2_04_06:
                  knx.getGroupObject(firstComObj).value(voc1_s, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-06"));
#endif
                  break;
            case D2_04_07:
                  knx.getGroupObject(firstComObj).value(voc1_s, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-07"));
#endif
                  break;
            case D2_04_08:
                  knx.getGroupObject(firstComObj).value(voc2_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-08"));
#endif
                  break;
            case D2_04_09:
                  knx.getGroupObject(firstComObj).value(voc2_s, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-09"));
#endif
                  break;
            case D2_04_10:
                  knx.getGroupObject(firstComObj).value(voc2_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-10"));
#endif
                  break;
            case D2_04_0A:
                  knx.getGroupObject(firstComObj).value(voc2_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-0A"));
#endif
                  break;
            case D2_04_0B:
                  knx.getGroupObject(firstComObj).value(voc2_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  // knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-0B"));
#endif
                  break;
            case D2_04_0C:
                  knx.getGroupObject(firstComObj).value(voc2_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-0C"));
#endif
                  break;
            case D2_04_0D:
                  knx.getGroupObject(firstComObj).value(voc2_s, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-0D"));
#endif
                  break;
            case D2_04_0E:
                  knx.getGroupObject(firstComObj).value(voc2_s, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj + 1).value(temp_s, getDPT(VAL_DPT_9));
                  //knx.getGroupObject(firstComObj + 2).value(hum_s, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 3).value(SenVal_D2_04->u8VldTelSenSta.daynight, getDPT(VAL_DPT_1));
                  knx.getGroupObject(firstComObj + 4).value(SenVal_D2_04->u8VldTelSenSta.batt, getDPT(VAL_DPT_5));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("Profil: D2-04-0E"));
#endif
                  break;

            default:
                  break;
            } // ENDE SWITCH D2-04-xx
#ifdef KDEBUG
            SERIAL_PORT.print(F("Temp: "));
            SERIAL_PORT.println(temp_s);
            SERIAL_PORT.print(F("Hum: "));
            SERIAL_PORT.println(hum_s);
            SERIAL_PORT.print(F("VOC1: "));
            SERIAL_PORT.println(voc1_s);
            SERIAL_PORT.print(F("VOC2: "));
            SERIAL_PORT.println(voc2_s);
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

      default:
#ifdef KDEBUG
            SERIAL_PORT.println(F("Profil: ERROR"));
#endif
            break;
      } // ENDE D2-xx
}
