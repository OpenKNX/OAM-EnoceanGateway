#pragma

#include "EnoceanProfils.h"

uint8_t handle_4BS(PACKET_SERIAL_TYPE *f_Pkt_st, uint8_t profil, uint8_t profil2nd, uint8_t firstComObj, uint8_t firstParameter)
{

      uint16_t lux;
      float luxfloat;
      float temp;
      float hum;
      int8_t value;

      FOURBS_A5_02_TYPE *fourBsA5_02_Tlg_p;
      FOURBS_A5_02_2030TYPE *fourBsA5_02_2030_Tlg_p;
      FOURBS_A5_04_TYPE *fourBsA5_04_Tlg_p;
      FOURBS_A5_04_03_TYPE *fourBsA5_04_03_Tlg_p;
      FOURBS_A5_06_01_TYPE *fourBsA5_06_01_Tlg_p;
      FOURBS_A5_06_01_TYPE *fourBsA5_06_02_Tlg_p;
      FOURBS_A5_06_03_TYPE *fourBsA5_06_03_Tlg_p;
      FOURBS_A5_07_01_TYPE *fourBsTlg2_p;
      FOURBS_A5_07_02_TYPE *fourBsTlg_p;
      FOURBS_A5_07_03_TYPE *fourBsTlg3_p;
      FOURBS_A5_14_01_06_TYPE *fourBsA5_17_01_06_Tlg_p;
      FOURBS_A5_14_07_08_TYPE *fourBsA5_17_07_08_Tlg_p;
      FOURBS_A5_14_09_0A_TYPE *fourBsA5_17_09_0A_Tlg_p;
      FOURBS_A5_20_06_TYPE *fourBsA5_20_06_Tlg_p;

#ifdef KDEBUG
      SERIAL_PORT.print("Profil: 4BS - ");
      SERIAL_PORT.println(firstParameter + ENO_CHProfilSelection4BS);
#endif

#ifndef EnOceanTEST
      switch (knx.paramWord(firstParameter + ENO_CHProfilSelection4BS))
#else
      switch (profil)
#endif
      {
      case A5_02:
#ifdef KDEBUG
            SERIAL_PORT.print("A5-02-");
#endif
            switch (knx.paramWord(firstParameter + ENO_CHProfil4BS02))
            {
            //**************************************************************
            // ----------------- Profil: A5-02-01 --------------------------
            //**************************************************************
            case A5_02_01:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -6.375);
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("01");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-02 --------------------------
            //**************************************************************
            case A5_02_02:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -6.375) + 10.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("02");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-03 --------------------------
            //**************************************************************
            case A5_02_03:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -6.375) + 20.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("03");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-04 --------------------------
            //**************************************************************
            case A5_02_04:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -6.375) + 30.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("04");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-05 --------------------------
            //**************************************************************
            case A5_02_05:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -6.375) + 40.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("05");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-06 --------------------------
            //**************************************************************
            case A5_02_06:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -6.375) + 50.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("06");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-07 --------------------------
            //**************************************************************
            case A5_02_07:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -6.375) + 60.0;
                  knx.getGroupObject(firstComObj).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("07");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-08 --------------------------
            //**************************************************************
            case A5_02_08:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -6.375) + 70.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("08");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-09 --------------------------
            //**************************************************************
            case A5_02_09:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -6.375) + 80.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("09");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-0A --------------------------
            //**************************************************************
            case A5_02_0A:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -6.375) + 90.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("0A");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-0B --------------------------
            //**************************************************************
            case A5_02_0B:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -6.375) + 100.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("0B");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-10 --------------------------
            //**************************************************************
            case A5_02_10:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -3.1875) + 20.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("10");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-11 --------------------------
            //**************************************************************
            case A5_02_11:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -3.1875) + 30.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("11");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-12 --------------------------
            //**************************************************************
            case A5_02_12:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -3.1875) + 40.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("12");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-13 --------------------------
            //**************************************************************
            case A5_02_13:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -3.1875) + 50.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("13");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-14 --------------------------
            //**************************************************************
            case A5_02_14:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -3.1875) + 60.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("14");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-15 --------------------------
            //**************************************************************
            case A5_02_15:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -3.1875) + 70.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("15");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-16 --------------------------
            //**************************************************************
            case A5_02_16:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -3.1875) + 80.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("16");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-17 --------------------------
            //**************************************************************
            case A5_02_17:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -3.1875) + 90.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("17");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-18 --------------------------
            //**************************************************************
            case A5_02_18:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -3.1875) + 100.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("18");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-19 --------------------------
            //**************************************************************
            case A5_02_19:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -3.1875) + 110.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("19");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-1A --------------------------
            //**************************************************************
            case A5_02_1A:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -3.1875) + 120.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("1A");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-1B --------------------------
            //**************************************************************
            case A5_02_1B:
                  fourBsA5_02_Tlg_p = (FOURBS_A5_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_02_Tlg_p->Temp / -3.1875) + 130.0;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("1B");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-20 --------------------------
            //**************************************************************
            case A5_02_20:
                  fourBsA5_02_2030_Tlg_p = (FOURBS_A5_02_2030TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(((uint16_t)fourBsA5_02_2030_Tlg_p->TempMSB << 8 | fourBsA5_02_2030_Tlg_p->TempLSB) / -20.0) + 41.2;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("20");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-02-30 --------------------------
            //**************************************************************
            case A5_02_30:
                  fourBsA5_02_2030_Tlg_p = (FOURBS_A5_02_2030TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(((uint16_t)fourBsA5_02_2030_Tlg_p->TempMSB << 8 | fourBsA5_02_2030_Tlg_p->TempLSB) / -10.0) + 62.3;
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("30");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
#endif
                  break;
            default:
                  break;
            }
            break; // ENDE A5-02-XX

      case A5_04:
#ifdef KDEBUG
            SERIAL_PORT.print("A5-04-");
#endif
            switch (knx.paramWord(firstParameter + ENO_CHProfil4BS04))
            {
            //**************************************************************
            // ----------------- Profil: A5-04-01 --------------------------
            //**************************************************************
            case A5_04_01:
                  fourBsA5_04_Tlg_p = (FOURBS_A5_04_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_04_Tlg_p->Temp / -6.25) + 40.0;
                  hum = (float)(fourBsA5_04_Tlg_p->Hum / 2.5);
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 2).value(hum, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("01");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
                  SERIAL_PORT.print("Hum: ");
                  SERIAL_PORT.println(hum);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-04-02 --------------------------
            //**************************************************************
            case A5_04_02:
                  fourBsA5_04_Tlg_p = (FOURBS_A5_04_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(fourBsA5_04_Tlg_p->Temp / -3.125) + 60.0;
                  hum = (float)(fourBsA5_04_Tlg_p->Hum / 2.5);
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 2).value(hum, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("02");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
                  SERIAL_PORT.print("Hum: ");
                  SERIAL_PORT.println(hum);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-04-03 --------------------------
            //**************************************************************
            case A5_04_03:
                  fourBsA5_04_03_Tlg_p = (FOURBS_A5_04_03_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  temp = (float)(((uint16_t)fourBsA5_04_03_Tlg_p->TempMSB << 8 | fourBsA5_04_03_Tlg_p->TempLSB) / 12.7875) - 20.0;
                  hum = (float)(fourBsA5_04_03_Tlg_p->Hum / 2.55);
                  knx.getGroupObject(firstComObj + 1).value(temp, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 2).value(hum, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.println("03");
                  SERIAL_PORT.print("Temp: ");
                  SERIAL_PORT.println(temp);
                  SERIAL_PORT.print("Hum: ");
                  SERIAL_PORT.println(hum);
#endif
                  break;
            default:
                  break;
            }
            break; // ENDE A5-04-XX
      case A5_06:
#ifdef KDEBUG
            SERIAL_PORT.print("A5-06-");
#endif
            switch (knx.paramWord(firstParameter + ENO_CHProfil4BS06))
            {
            //**************************************************************
            // ----------------- Profil: A5-06-01 --------------------------
            //**************************************************************
            case A5_06_01:
#ifdef KDEBUG
                  SERIAL_PORT.println(F("01"));
#endif
                  fourBsA5_06_01_Tlg_p = (FOURBS_A5_06_01_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  switch (fourBsA5_06_01_Tlg_p->u84BsTelData.RS)
                  {
                  case 0: // Range 600 ... 60000 lux
                        luxfloat = (float)(fourBsA5_06_01_Tlg_p->u8Illumination1 * 232.941176) + 600.0;
                        break;
                  case 1: // Range 300 ... 30000 lux
                        luxfloat = (float)(fourBsA5_06_01_Tlg_p->u8Illumination2 * 116.470588) + 300.0;
                        break;
                  default:
                        break;
                  }
                  knx.getGroupObject(firstComObj + 1).value(luxfloat, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 2).value(fourBsA5_06_01_Tlg_p->u8SupplyVoltage / 50.0, getDPT(VAL_DPT_9));

#ifdef KDEBUG
                  SERIAL_PORT.print(F("RS: "));
                  SERIAL_PORT.println(fourBsA5_06_01_Tlg_p->u84BsTelData.RS);
                  SERIAL_PORT.print(F("LUX: "));
                  SERIAL_PORT.println(luxfloat);
                  SERIAL_PORT.print(F("Bat: "));
                  SERIAL_PORT.println(fourBsA5_06_01_Tlg_p->u8SupplyVoltage / 50.0);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-06-02 --------------------------
            //**************************************************************
            case A5_06_02:
#ifdef KDEBUG
                  SERIAL_PORT.println(F("02"));
#endif
                  fourBsA5_06_02_Tlg_p = (FOURBS_A5_06_01_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  switch (fourBsA5_06_02_Tlg_p->u84BsTelData.RS)
                  {
                  case 0: // Range 0 ... 1020 lux
                        luxfloat = (float)(fourBsA5_06_02_Tlg_p->u8Illumination1 / 0.25);
                        break;
                  case 1: // Range 0 ... 510 lux
                        luxfloat = (float)(fourBsA5_06_02_Tlg_p->u8Illumination2 / 0.5);
                        break;
                  default:
                        break;
                  }
                  knx.getGroupObject(firstComObj + 1).value(luxfloat, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 2).value(fourBsA5_06_02_Tlg_p->u8SupplyVoltage / 50.0, getDPT(VAL_DPT_9));

#ifdef KDEBUG
                  SERIAL_PORT.print(F("LUX: "));
                  SERIAL_PORT.println(luxfloat);
                  SERIAL_PORT.print(F("Bat: "));
                  SERIAL_PORT.println(fourBsA5_06_02_Tlg_p->u8SupplyVoltage / 50.0);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-06-03 --------------------------
            //**************************************************************
            case A5_06_03:
#ifdef KDEBUG
                  SERIAL_PORT.println(F("03"));
#endif
                  fourBsA5_06_03_Tlg_p = (FOURBS_A5_06_03_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);

                  // ........Illumination.......................................
                  lux = (uint16_t)fourBsA5_06_03_Tlg_p->u8LuxMSB << 2 | fourBsA5_06_03_Tlg_p->u84BsTelData.LUX;
                  luxfloat = (float)lux * 1.0;

                  knx.getGroupObject(firstComObj + 1).value(luxfloat, getDPT(VAL_DPT_9));
                  knx.getGroupObject(firstComObj + 2).value(fourBsA5_06_03_Tlg_p->u8SupplyVoltage / 51.0, getDPT(VAL_DPT_9));

#ifdef KDEBUG
                  SERIAL_PORT.print(F("LUX: "));
                  SERIAL_PORT.println(luxfloat);
                  SERIAL_PORT.print(F("Bat: "));
                  SERIAL_PORT.println(fourBsA5_06_03_Tlg_p->u8SupplyVoltage / 51.0);
#endif
                  break;
            default:
                  break;
            }
            break; // ENDE A5-06-XX
      case A5_07:
#ifdef KDEBUG
            SERIAL_PORT.print("A5-07-");
#endif
            switch (knx.paramWord(firstParameter + ENO_CHProfil4BS07))
            {
            //**************************************************************
            // ----------------- Profil: A5-07-01 --------------------------
            //**************************************************************
            case A5_07_01:
                  fourBsTlg2_p = (FOURBS_A5_07_01_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
                  // .......PIR Status.........................................
                  if (fourBsTlg2_p->PIR < 128)
                        knx.getGroupObject(firstComObj).value(false, getDPT(VAL_DPT_1_18));
                  else
                        knx.getGroupObject(firstComObj).value(true, getDPT(VAL_DPT_1_18));
#ifdef KDEBUG
                  SERIAL_PORT.println(F("01"));
                  SERIAL_PORT.print(F("PIR: "));
                  SERIAL_PORT.println(fourBsTlg2_p->PIR);
#endif
                  break; // ENDE A5-07-01
            //**************************************************************
            // ----------------- Profil: A5-07-02 --------------------------
            //**************************************************************
            case A5_07_02:
                  fourBsTlg_p = (FOURBS_A5_07_02_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
#ifdef KDEBUG
                  SERIAL_PORT.println(F("02"));
#endif
                  // .......PIR Status.........................................
                  knx.getGroupObject(firstComObj).value(fourBsTlg_p->u84BsTelData.PIR, getDPT(VAL_DPT_1_18));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("PIR: "));
                  SERIAL_PORT.println(fourBsTlg_p->u84BsTelData.PIR);
#endif
                  // ...................  Supply Voltage ......................
                  knx.getGroupObject(firstComObj + 2).value(fourBsTlg_p->u8SupplyVoltage * 20.0, getDPT(VAL_DPT_9_20));
#ifdef KDEBUG
                  SERIAL_PORT.print("Supply Voltage: ");
                  SERIAL_PORT.println(fourBsTlg_p->u8SupplyVoltage / 50.0);
#endif
                  break; // ENDE A5-07-02

            //**************************************************************
            // ----------------- Profil: A5-07-03 --------------------------
            //**************************************************************
            case A5_07_03:
                  fourBsTlg3_p = (FOURBS_A5_07_03_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
#ifdef KDEBUG
                  SERIAL_PORT.println(F("03"));
#endif
                  // .......PIR Status.........................................
                  knx.getGroupObject(firstComObj).value(fourBsTlg3_p->u84BsTelData.PIR, getDPT(VAL_DPT_1_18));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("PIR:"));
                  SERIAL_PORT.println(fourBsTlg3_p->u84BsTelData.PIR);
#endif
                  // ........Illumination.......................................
                  lux = (uint16_t)fourBsTlg3_p->u8LuxMSB << 2 | fourBsTlg3_p->u84BsTelData2.LUX;
                  luxfloat = (float)lux * 1.0;
                  knx.getGroupObject(firstComObj + 1).value(luxfloat, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("LUX: "));
                  SERIAL_PORT.println(lux);
#endif
                  // ...................  Supply Voltage .......................
                  knx.getGroupObject(firstComObj + 2).value(fourBsTlg3_p->u8SupplyVoltage * 20.0, getDPT(VAL_DPT_9_20));
#ifdef KDEBUG
                  SERIAL_PORT.print("Supply Voltage: ");
                  SERIAL_PORT.println(fourBsTlg3_p->u8SupplyVoltage / 50.0);
#endif
                  break; // ENDE A5-07-03

            default:
#ifdef KDEBUG
                  SERIAL_PORT.println("ERROR");
#endif
                  break;
            }
            break; // ENDE A5-07-XX

      case A5_14:
#ifdef KDEBUG
            SERIAL_PORT.print("A5-14-");
#endif
            switch (knx.paramWord(firstParameter + ENO_CHProfil4BS07))
            {
            //**************************************************************
            // ----------------- Profil: A5-14-01 --------------------------
            //**************************************************************
            case A5_14_01:
                  fourBsA5_17_01_06_Tlg_p = (FOURBS_A5_14_01_06_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
#ifdef KDEBUG
                  SERIAL_PORT.println(F("01"));
#endif
                  // ...................  Supply Voltage .......................
                  knx.getGroupObject(firstComObj + 4).value(fourBsA5_17_01_06_Tlg_p->u8SupplyVoltage * 20.0, getDPT(VAL_DPT_9_20));
#ifdef KDEBUG
                  SERIAL_PORT.print("Supply Voltage: ");
                  SERIAL_PORT.println(fourBsA5_17_01_06_Tlg_p->u8SupplyVoltage / 50.0);
#endif
                  // ........Contact..............................................
                  knx.getGroupObject(firstComObj + 3).value(fourBsA5_17_01_06_Tlg_p->u84BsTelData.CT, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("detected: State: "));
                  SERIAL_PORT.println(fourBsA5_17_01_06_Tlg_p->u84BsTelData.CT);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-14-02 --------------------------
            //**************************************************************
            case A5_14_02:
                  fourBsA5_17_01_06_Tlg_p = (FOURBS_A5_14_01_06_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
#ifdef KDEBUG
                  SERIAL_PORT.println(F("02"));
#endif
                  // ...................  Supply Voltage .......................
                  knx.getGroupObject(firstComObj + 4).value(fourBsA5_17_01_06_Tlg_p->u8SupplyVoltage * 20.0, getDPT(VAL_DPT_9_20));
#ifdef KDEBUG
                  SERIAL_PORT.print("Supply Voltage: ");
                  SERIAL_PORT.println(fourBsA5_17_01_06_Tlg_p->u8SupplyVoltage / 50.0);
#endif
                  // ........Illumination.......................................
                  lux = (uint16_t)fourBsA5_17_01_06_Tlg_p->u8Lux * 4;
                  luxfloat = (float)lux * 1.0;
                  knx.getGroupObject(firstComObj + 1).value(luxfloat, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("LUX: "));
                  SERIAL_PORT.println(lux);
#endif
                  // ........Contact..............................................
                  knx.getGroupObject(firstComObj + 3).value(fourBsA5_17_01_06_Tlg_p->u84BsTelData.CT, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("detected: State: "));
                  SERIAL_PORT.println(fourBsA5_17_01_06_Tlg_p->u84BsTelData.CT);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-14-03 --------------------------
            //**************************************************************
            case A5_14_03:
                  fourBsA5_17_01_06_Tlg_p = (FOURBS_A5_14_01_06_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
#ifdef KDEBUG
                  SERIAL_PORT.println(F("03"));
#endif
                  // ...................  Supply Voltage .......................
                  knx.getGroupObject(firstComObj + 4).value(fourBsA5_17_01_06_Tlg_p->u8SupplyVoltage * 20.0, getDPT(VAL_DPT_9_20));
#ifdef KDEBUG
                  SERIAL_PORT.print("Supply Voltage: ");
                  SERIAL_PORT.println(fourBsA5_17_01_06_Tlg_p->u8SupplyVoltage / 50.0);
#endif
                  // ........Contact..............................................
                  knx.getGroupObject(firstComObj + 3).value(fourBsA5_17_01_06_Tlg_p->u84BsTelData.CT, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("detected: State: "));
                  SERIAL_PORT.println(fourBsA5_17_01_06_Tlg_p->u84BsTelData.CT);
#endif
                  // ........Vibration Det.........................................
                  knx.getGroupObject(firstComObj).value(fourBsA5_17_01_06_Tlg_p->u84BsTelData.VIB, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Vibration det: "));
                  SERIAL_PORT.println(fourBsA5_17_01_06_Tlg_p->u84BsTelData.VIB);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-14-04 --------------------------
            //**************************************************************
            case A5_14_04:
                  fourBsA5_17_01_06_Tlg_p = (FOURBS_A5_14_01_06_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
#ifdef KDEBUG
                  SERIAL_PORT.println(F("04"));
#endif
                  // ...................  Supply Voltage .......................
                  knx.getGroupObject(firstComObj + 4).value(fourBsA5_17_01_06_Tlg_p->u8SupplyVoltage * 20.0, getDPT(VAL_DPT_9_20));
#ifdef KDEBUG
                  SERIAL_PORT.print("Supply Voltage: ");
                  SERIAL_PORT.println(fourBsA5_17_01_06_Tlg_p->u8SupplyVoltage / 50.0);
#endif
                  // ........Illumination.......................................
                  lux = (uint16_t)fourBsA5_17_01_06_Tlg_p->u8Lux * 4;
                  luxfloat = (float)lux * 1.0;
                  knx.getGroupObject(firstComObj + 1).value(luxfloat, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("LUX: "));
                  SERIAL_PORT.println(lux);
#endif
                  // ........Contact..............................................
                  knx.getGroupObject(firstComObj + 3).value(fourBsA5_17_01_06_Tlg_p->u84BsTelData.CT, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("detected: State: "));
                  SERIAL_PORT.println(fourBsA5_17_01_06_Tlg_p->u84BsTelData.CT);
#endif
                  // ........Vibration Det.........................................
                  knx.getGroupObject(firstComObj).value(fourBsA5_17_01_06_Tlg_p->u84BsTelData.VIB, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Vibration det: "));
                  SERIAL_PORT.println(fourBsA5_17_01_06_Tlg_p->u84BsTelData.VIB);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-14-05 --------------------------
            //**************************************************************
            case A5_14_05:
                  fourBsA5_17_01_06_Tlg_p = (FOURBS_A5_14_01_06_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
#ifdef KDEBUG
                  SERIAL_PORT.println(F("05"));
#endif
                  // ...................  Supply Voltage .......................
                  knx.getGroupObject(firstComObj + 4).value(fourBsA5_17_01_06_Tlg_p->u8SupplyVoltage * 20.0, getDPT(VAL_DPT_9_20));
#ifdef KDEBUG
                  SERIAL_PORT.print("Supply Voltage: ");
                  SERIAL_PORT.println(fourBsA5_17_01_06_Tlg_p->u8SupplyVoltage / 50.0);
#endif
                  // ........Vibration Det.........................................
                  knx.getGroupObject(firstComObj).value(fourBsA5_17_01_06_Tlg_p->u84BsTelData.VIB, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Vibration det: "));
                  SERIAL_PORT.println(fourBsA5_17_01_06_Tlg_p->u84BsTelData.VIB);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-14-06 --------------------------
            //**************************************************************
            case A5_14_06:
                  fourBsA5_17_01_06_Tlg_p = (FOURBS_A5_14_01_06_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
#ifdef KDEBUG
                  SERIAL_PORT.println(F("06"));
#endif
                  // ...................  Supply Voltage .......................
                  knx.getGroupObject(firstComObj + 4).value(fourBsA5_17_01_06_Tlg_p->u8SupplyVoltage * 20.0, getDPT(VAL_DPT_9_20));
#ifdef KDEBUG
                  SERIAL_PORT.print("Supply Voltage: ");
                  SERIAL_PORT.println(fourBsA5_17_01_06_Tlg_p->u8SupplyVoltage / 50.0);
#endif
                  // ........Illumination.......................................
                  lux = (uint16_t)fourBsA5_17_01_06_Tlg_p->u8Lux * 4;
                  luxfloat = (float)lux * 1.0;
                  knx.getGroupObject(firstComObj + 1).value(luxfloat, getDPT(VAL_DPT_9));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("LUX: "));
                  SERIAL_PORT.println(lux);
#endif
                  // ........Vibration Det.........................................
                  knx.getGroupObject(firstComObj).value(fourBsA5_17_01_06_Tlg_p->u84BsTelData.VIB, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Vibration det: "));
                  SERIAL_PORT.println(fourBsA5_17_01_06_Tlg_p->u84BsTelData.VIB);
#endif
                  break;

            //**************************************************************
            // ----------------- Profil: A5-14-07 --------------------------
            //**************************************************************
            case A5_14_07:
                  fourBsA5_17_07_08_Tlg_p = (FOURBS_A5_14_07_08_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
#ifdef KDEBUG
                  SERIAL_PORT.println(F("07"));
#endif
                  // ...................  Supply Voltage .......................
                  knx.getGroupObject(firstComObj + 4).value(fourBsA5_17_07_08_Tlg_p->u8SupplyVoltage * 20.0, getDPT(VAL_DPT_9_20));
#ifdef KDEBUG
                  SERIAL_PORT.print("Supply Voltage: ");
                  SERIAL_PORT.println(fourBsA5_17_07_08_Tlg_p->u8SupplyVoltage / 50.0);
#endif
                  // ........DOOR Contact..............................................
                  knx.getGroupObject(firstComObj + 1).value(fourBsA5_17_07_08_Tlg_p->u84BsTelData.DCT, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Door State: "));
                  SERIAL_PORT.println(fourBsA5_17_07_08_Tlg_p->u84BsTelData.DCT);
#endif
                  // ........Lock Contact.........................................
                  knx.getGroupObject(firstComObj + 3).value(fourBsA5_17_07_08_Tlg_p->u84BsTelData.LCT, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Lock State: "));
                  SERIAL_PORT.println(fourBsA5_17_07_08_Tlg_p->u84BsTelData.LCT);
#endif
                  break;
            //**************************************************************
            // ----------------- Profil: A5-14-08 --------------------------
            //**************************************************************
            case A5_14_08:
                  fourBsA5_17_07_08_Tlg_p = (FOURBS_A5_14_07_08_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
#ifdef KDEBUG
                  SERIAL_PORT.println(F("08"));
#endif
                  // ...................  Supply Voltage .......................
                  knx.getGroupObject(firstComObj + 4).value(fourBsA5_17_07_08_Tlg_p->u8SupplyVoltage * 20.0, getDPT(VAL_DPT_9_20));
#ifdef KDEBUG
                  SERIAL_PORT.print("Supply Voltage: ");
                  SERIAL_PORT.println(fourBsA5_17_07_08_Tlg_p->u8SupplyVoltage / 50.0);
#endif
                  // ........Vibration Det.........................................
                  knx.getGroupObject(firstComObj).value(fourBsA5_17_01_06_Tlg_p->u84BsTelData.VIB, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Vibration det: "));
                  SERIAL_PORT.println(fourBsA5_17_01_06_Tlg_p->u84BsTelData.VIB);
#endif
                  // ........DOOR Contact..............................................
                  knx.getGroupObject(firstComObj + 1).value(fourBsA5_17_07_08_Tlg_p->u84BsTelData.DCT, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Door State: "));
                  SERIAL_PORT.println(fourBsA5_17_07_08_Tlg_p->u84BsTelData.DCT);
#endif
                  // ........Lock Contact.........................................
                  knx.getGroupObject(firstComObj + 3).value(fourBsA5_17_07_08_Tlg_p->u84BsTelData.LCT, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                  SERIAL_PORT.print(F("Lock State: "));
                  SERIAL_PORT.println(fourBsA5_17_07_08_Tlg_p->u84BsTelData.LCT);
#endif
                  break;
                  //**************************************************************
            // ----------------- Profil: A5-14-09 --------------------------
            //**************************************************************
            case A5_14_09:
                  fourBsA5_17_09_0A_Tlg_p = (FOURBS_A5_14_09_0A_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
#ifdef KDEBUG
                  SERIAL_PORT.println(F("09"));
#endif
                  // ...................  Supply Voltage .......................
                  knx.getGroupObject(firstComObj + 4).value(fourBsA5_17_09_0A_Tlg_p->u8SupplyVoltage * 20.0, getDPT(VAL_DPT_9_20));
#ifdef KDEBUG
                  SERIAL_PORT.print("Supply Voltage: ");
                  SERIAL_PORT.println(fourBsA5_17_09_0A_Tlg_p->u8SupplyVoltage / 50.0);

                   SERIAL_PORT.println(fourBsA5_17_09_0A_Tlg_p->u84BsTelData.CT);
#endif
                  switch (fourBsA5_17_09_0A_Tlg_p->u84BsTelData.CT)
                  {
                  case 0x00: //close
                        knx.getGroupObject(firstComObj + 1).value(false, getDPT(VAL_DPT_1));
                        knx.getGroupObject(firstComObj + 2).value(false, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                        SERIAL_PORT.println("State: close");
#endif
                        break;
                  case 0x01: //tilt
                        knx.getGroupObject(firstComObj + 1).value(false, getDPT(VAL_DPT_1));
                        knx.getGroupObject(firstComObj + 2).value(true, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                        SERIAL_PORT.println("State: gekippt");
#endif
                        break;
                  case 3: //open
                        knx.getGroupObject(firstComObj + 1).value(true, getDPT(VAL_DPT_1));
                        knx.getGroupObject(firstComObj + 2).value(false, getDPT(VAL_DPT_1));
#ifdef KDEBUG
                        SERIAL_PORT.println("State: open");
#endif
                        break;

                  default:
                        break;
                  }
                  break;

            default:
#ifdef KDEBUG
                  SERIAL_PORT.println("ERROR");
#endif
                  break;
            }
            break; // ENDE A5-14-XX
      case A5_20:
#ifdef KDEBUG
            SERIAL_PORT.print("A5-20-");
#endif
#ifndef EnOceanTEST
            switch (knx.paramWord(firstParameter + ENO_CHProfil4BS20))
#else
            switch (profil2nd)
#endif
            {
            //**************************************************************
            // ----------------- Profil: A5-20-06 --------------------------
            //**************************************************************
            case A5_20_06:
                  fourBsA5_20_06_Tlg_p = (FOURBS_A5_20_06_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);
#ifdef KDEBUG
                  SERIAL_PORT.println(F("06"));
#endif
                  // ......Current Position.........................................
#ifndef EnOceanTEST
                  knx.getGroupObject(firstComObj + 7).value(fourBsA5_20_06_Tlg_p->u8CurrentPos, getDPT(VAL_DPT_5));
                  #endif
#ifdef KDEBUG
                  SERIAL_PORT.print(F("current Pos: "));
                  SERIAL_PORT.print(fourBsA5_20_06_Tlg_p->u8CurrentPos);
                  SERIAL_PORT.println("%");
#endif

                  // ......Temperature Offset.........................................
                  if (fourBsA5_20_06_Tlg_p->u84BsTelData.LOM == 1)
                  {
#ifndef EnOceanTEST
                        knx.getGroupObject(firstComObj + 8).value(fourBsA5_20_06_Tlg_p->u84BsTelData.Offset / 2.0, getDPT(VAL_DPT_9));
#endif
#ifdef KDEBUG
                        SERIAL_PORT.print(F("Local offset: "));
                        SERIAL_PORT.print(fourBsA5_20_06_Tlg_p->u84BsTelData.Offset / 2.0);
                        SERIAL_PORT.println("°C");
#endif
                  }
                  else if (fourBsA5_20_06_Tlg_p->u84BsTelData.LOM == 0)
                  {
                        switch (fourBsA5_20_06_Tlg_p->u84BsTelData.Offset)
                        {
                        case 0x0:
                              value = 0;
                              break;
                        case 0x1:
                              value = 1;
                              break;
                        case 0x2:
                              value = 2;
                              break;
                        case 0x3:
                              value = 3;
                              break;
                        case 0x4:
                              value = 4;
                              break;
                        case 0x5:
                              value = 5;
                              break;
                        case 0x7B:
                              value = -5;
                              break;
                        case 0x7C:
                              value = -4;
                              break;
                        case 0x7D:
                              value = -3;
                              break;
                        case 0x7E:
                              value = -2;
                              break;
                        case 0x7F:
                              value = -1;
                              break;

                        default:
                              break;
                        }
#ifndef EnOceanTEST
                        knx.getGroupObject(firstComObj + 8).value(value, getDPT(VAL_DPT_9));
#endif
#ifdef KDEBUG
                        SERIAL_PORT.print(F("Local offset: "));
                        SERIAL_PORT.print(value);
                        SERIAL_PORT.println("%");
#endif
                  }
#ifndef EnOceanTEST
                  // ......Temperature.........................................
                  knx.getGroupObject(firstComObj + 6).value(fourBsA5_20_06_Tlg_p->u8Temp/2.0, getDPT(VAL_DPT_9));
                  // ......Status Bits.........................................
                  knx.getGroupObject(firstComObj + 9).value(fourBsA5_20_06_Tlg_p->u8StatusBits, getDPT(VAL_DPT_5));
#endif

#ifdef KDEBUG
                  SERIAL_PORT.print(F("Temperature: "));
                  SERIAL_PORT.println(fourBsA5_20_06_Tlg_p->u8Temp/2.0);
                  SERIAL_PORT.print(F("Status Bits (MSB-LSB): "));
                  SERIAL_PORT.println(fourBsA5_20_06_Tlg_p->u8StatusBits,BIN);
#endif
                  break; // ENDE A5-20-06
            default:
                  break;
#ifdef KDEBUG
                  SERIAL_PORT.println("ERROR");
#endif
                  break;
            }
            return msg_A5_20_06;
            break; // ENDE A5-20-XX

      //**************************************************************
      default:
#ifdef KDEBUG
            SERIAL_PORT.println(F("Profil: ERROR"));
#endif
            break;
      }

      return 0;
}
