#pragma once

#include "EnoceanProfils.h"
#include "KnxHelper.h"

//Global Var for Rocker SW to detect with Rocker SW was pressed
uint8_t rockerNr[MAX_NUMBER_OF_DEVICES] = {0};

uint8_t szeneNr;

uint8_t dpt3value = 0;

void release_AI(uint8_t firstParameter, uint8_t firstComObj)
{
#ifdef KDEBUG
  SERIAL_PORT.print(F("AI "));
#endif
  switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionA))
  {
  case 3: // Dimmen Heller/Dunkler (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Dim Dunkler"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 7, 1));
    dpt3value = 0;
    knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 7, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 4: // Dimmen Dunkler/Heller (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Dim Heller"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 7, 1));
    dpt3value = 8;
    knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 7, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 5: // Jalousie AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Jal AB"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 8;
    knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 6: // Jalousie AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Jal AUF"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 0;
    knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  default:
    break;
  }
}

void release_AO(uint8_t firstParameter, uint8_t firstComObj)
{
#ifdef KDEBUG
  SERIAL_PORT.print(F("AO "));
#endif
  switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionA))
  {
  case 3: // Dimmen Heller/Dunkler (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Dim Heller"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 7, 1));
    dpt3value = 8;
    knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 7, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 4: // Dimmen Dunkler/Heller (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Dim Dunkler"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 7, 1));
    dpt3value = 0;
    knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 7, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 5: // Jalousie AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Jal AUF"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 0;
    knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 6: // Jalousie AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Jal AB"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 8;
    knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  default:
    break;
  }
}

void release_BI(uint8_t firstParameter, uint8_t firstComObj)
{
#ifdef KDEBUG
  SERIAL_PORT.print(F("BI "));
#endif
  switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionB))
  {
  case 3: // Dimmen Heller/Dunkler (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Dim Dunkler"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 7, 1));
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 7, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 4: // Dimmen Dunkler/Heller (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Dim Heller"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 7, 1));
    dpt3value = 8;
    knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 7, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 5: // Jalousie AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Jal AB"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 8;
    knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 6: // Jalousie AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Jal AUF"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  default:
    break;
  }
}

void release_BO(uint8_t firstParameter, uint8_t firstComObj)
{
#ifdef KDEBUG
  SERIAL_PORT.print(F("BO "));
#endif
  switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionB))
  {
  case 3: // Dimmen Heller/Dunkler (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Dim Heller"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 7, 1));
    dpt3value = 8;
    knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 7, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 4: // Dimmen Dunkler/Heller (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Dim Dunkler"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 7, 1));
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 7, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 5: // Jalousie AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Jal AUF"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 6: // Jalousie AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Jal AB"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 8;
    knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  default:
    break;
  }
}

void release_CI(uint8_t firstParameter, uint8_t firstComObj)
{
#ifdef KDEBUG
  SERIAL_PORT.print(F("CI "));
#endif
  switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionC))
  {
  case 3: // Dimmen Heller/Dunkler (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Dim Dunkler"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 7, 1));
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 7, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 4: // Dimmen Dunkler/Heller (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Dim Heller"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 7, 1));
    dpt3value = 8;
    knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 7, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 5: // Jalousie AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Jal AB"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 8;
    knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 6: // Jalousie AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Jal AUF"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  default:
    break;
  }
}

void release_CO(uint8_t firstParameter, uint8_t firstComObj)
{
#ifdef KDEBUG
  SERIAL_PORT.print(F("CO "));
#endif
  switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionC))
  {
  case 3: // Dimmen Heller/Dunkler (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Dim Heller"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 7, 1));
    dpt3value = 8;
    knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 7, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 4: // Dimmen Dunkler/Heller (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Dim Dunkler"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 7, 1));
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 7, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 5: // Jalousie AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Jal AUF"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 6: // Jalousie AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("release Jal AB"));
#endif
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 8;
    knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  default:
    break;
  }
}

void handle_RPS_Rocker(PACKET_SERIAL_TYPE *f_Pkt_st, uint8_t profil, uint8_t firstComObj, uint8_t firstParameter, uint8_t index)
{

  switch (f_Pkt_st->u8DataBuffer[1])
  {
  case AI_pressed:
#ifdef KDEBUG
    SERIAL_PORT.print(F("AI "));
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionA))
    {
    case 1: // Schalten EIN/AUS (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Schalten AUS"));
#endif
      knx.getGroupObject(firstComObj).value(false, getDPT(VAL_DPT_1));
      break;

    case 2: // Schalten AUS/EIN (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Schalten EIN"));
#endif
      knx.getGroupObject(firstComObj).value(true, getDPT(VAL_DPT_1));
      break;

    case 3: // Dimmen Heller/Dunkler (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Dimmen Dunkler"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 7, 1));
      dpt3value = 0;
      knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 7, 0));
      rockerNr[index] = 10;
      break;

    case 4: // Dimmen Dunkler/Heller (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Dimmen Heller"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 7, 1));
      dpt3value = 8;
      knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 7, 0));
      rockerNr[index] = 10;
      break;

    case 5: // Jalousie AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Jalousie AB"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 8, 1));
      dpt3value = 8;
      knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
      rockerNr[index] = 10;
      break;

    case 6: // Jalousie AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Jalousie AUF"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 8, 1));
      dpt3value = 0;
      knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
      rockerNr[index] = 10;
      break;

    case 7: // Szene a/b (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.print(F("Szene b: "));
      SERIAL_PORT.println(knx.paramByte(firstParameter + ENO_CHRockerASzeneB));
#endif
      szeneNr = (knx.paramByte(firstParameter + ENO_CHRockerASzeneB)) - 1;
      knx.getGroupObject(firstComObj).value(szeneNr, getDPT(VAL_DPT_17));
      break;

    default:
      break;
    }
    break;

  case AO_pressed:
#ifdef KDEBUG
    SERIAL_PORT.print(F("AO "));
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionA))
    {
    case 1: // Schalten EIN/AUS (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Schalten EIN"));
#endif
      knx.getGroupObject(firstComObj).value(true, getDPT(VAL_DPT_1));
      break;

    case 2: // Schalten AUS/EIN (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Schalten AUS"));
#endif
      knx.getGroupObject(firstComObj).value(false, getDPT(VAL_DPT_1));
      break;

    case 3: // Dimmen Heller/Dunkler (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Dimmen Heller"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 7, 1));
      dpt3value = 8;
      knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 7, 0));
      rockerNr[index] = 20;
      break;

    case 4: // Dimmen Dunkler/Heller (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Dimmen Dunkler"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 7, 1));
      dpt3value = 0;
      knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 7, 0));
      rockerNr[index] = 20;
      break;

    case 5: // Jalousie AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Jalousie AUF"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 8, 1));
      dpt3value = 0;
      knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
      rockerNr[index] = 20;
      break;

    case 6: // Jalousie AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Jalousie AB"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 8, 1));
      dpt3value = 8;
      knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
      rockerNr[index] = 20;
      break;

    case 7: // Szene a/b (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.print(F("Szene a: "));
      SERIAL_PORT.println(knx.paramByte(firstParameter + ENO_CHRockerASzeneA));
#endif
      szeneNr = (knx.paramByte(firstParameter + ENO_CHRockerASzeneA)) - 1;
      knx.getGroupObject(firstComObj).value(szeneNr, getDPT(VAL_DPT_17));
      break;

    default:
      break;
    }
    break;

  case BI_pressed:
#ifdef KDEBUG
    SERIAL_PORT.print(F("BI "));
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionB))
    {
    case 1: // Schalten EIN/AUS (Oben/unten Wippe)
      knx.getGroupObject(firstComObj + 1).value(false, getDPT(VAL_DPT_1));
#ifdef KDEBUG
      SERIAL_PORT.println(F("Schalten AUS"));
#endif
      break;

    case 2: // Schalten AUS/EIN (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Schalten EIN"));
#endif
      knx.getGroupObject(firstComObj + 1).value(true, getDPT(VAL_DPT_1));
      break;

    case 3: // Dimmen Heller/Dunkler (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Dimmen Dunkler"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 7, 1));
      dpt3value = 0;
      knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 7, 0));
      rockerNr[index] = 30;
      break;

    case 4: // Dimmen Dunkler/Heller (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Dimmen Heller"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 7, 1));
      dpt3value = 8;
      knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 7, 0));
      rockerNr[index] = 30;
      break;

    case 5: // Jalousie AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Jalousie AB"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 8, 1));
      dpt3value = 8;
      knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
      rockerNr[index] = 30;
      break;

    case 6: // Jalousie AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Jalousie AUF"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 8, 1));
      dpt3value = 0;
      knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
      rockerNr[index] = 30;
      break;

    case 7: // Szene a/b (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.print(F("Szene b: "));
      SERIAL_PORT.println(knx.paramByte(firstParameter + ENO_CHRockerBSzeneB));
#endif
      szeneNr = (knx.paramByte(firstParameter + ENO_CHRockerBSzeneB)) - 1;
      knx.getGroupObject(firstComObj + 1).value(szeneNr, getDPT(VAL_DPT_17));
      break;

    default:
      break;
    }
    break;

  case BO_pressed:
#ifdef KDEBUG
    SERIAL_PORT.print(F("BO "));
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionB))
    {
    case 1: // Schalten EIN/AUS (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Schalten EIN"));
#endif
      knx.getGroupObject(firstComObj + 1).value(true, getDPT(VAL_DPT_1));
      break;

    case 2: // Schalten AUS/EIN (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Schalten AUS"));
#endif
      knx.getGroupObject(firstComObj + 1).value(false, getDPT(VAL_DPT_1));
      break;

    case 3: // Dimmen Heller/Dunkler (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Dimmen Heller"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 7, 1));
      dpt3value = 8;
      knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 7, 0));
      rockerNr[index] = 40;
      break;

    case 4: // Dimmen Dunkler/Heller (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Dimmen Dunkler"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 7, 1));
      dpt3value = 0;
      knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 7, 0));
      rockerNr[index] = 40;
      break;

    case 5: // Jalousie AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Jalousie AUF"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 8, 1));
      dpt3value = 0;
      knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
      rockerNr[index] = 40;
      break;

    case 6: // Jalousie AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Jalousie AB"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 8, 1));
      dpt3value = 8;
      knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
      rockerNr[index] = 40;
      break;

    case 7: // Szene a (Oben Wippe)
#ifdef KDEBUG
      SERIAL_PORT.print(F("Szene a: "));
      SERIAL_PORT.println(knx.paramByte(firstParameter + ENO_CHRockerBSzeneA));
#endif
      szeneNr = (knx.paramByte(firstParameter + ENO_CHRockerBSzeneA)) - 1;
      knx.getGroupObject(firstComObj + 1).value(szeneNr, getDPT(VAL_DPT_17));
      break;

    default:
      break;
    }
    break;

  case CI_pressed:
#ifdef KDEBUG
    SERIAL_PORT.print(F("CI "));
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionC))
    {
    case 1: // Schalten EIN/AUS (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Schalten AUS"));
#endif
      knx.getGroupObject(firstComObj + 2).value(false, getDPT(VAL_DPT_1));
      break;

    case 2: // Schalten AUS/EIN (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Schalten EIN"));
#endif
      knx.getGroupObject(firstComObj + 2).value(true, getDPT(VAL_DPT_1));
      break;

    case 3: // Dimmen Heller/Dunkler (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Dimmen Dunkler"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 7, 1));
      dpt3value = 0;
      knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 7, 0));
      rockerNr[index] = 50;
      break;

    case 4: // Dimmen Dunkler/Heller (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Dimmen Heller"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 7, 1));
      dpt3value = 8;
      knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 7, 0));
      rockerNr[index] = 50;
      break;

    case 5: // Jalousie AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Jalousie AB"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 8, 1));
      dpt3value = 8;
      knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
      rockerNr[index] = 50;
      break;

    case 6: // Jalousie AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Jalousie AUF"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 8, 1));
      dpt3value = 0;
      knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
      rockerNr[index] = 50;
      break;

    case 7: // Szene a/b (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.print(F("Szene b: "));
      SERIAL_PORT.println(knx.paramByte(firstParameter + ENO_CHRockerCSzeneB));
#endif
      szeneNr = (knx.paramByte(firstParameter + ENO_CHRockerCSzeneB)) - 1;
      knx.getGroupObject(firstComObj + 2).value(szeneNr, getDPT(VAL_DPT_17));
      break;

    default:
      break;
    }
    break;

  case CO_pressed:
#ifdef KDEBUG
    SERIAL_PORT.print(F("CO "));
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionC))
    {
    case 1: // Schalten EIN/AUS (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Schalten EIN"));
#endif
      knx.getGroupObject(firstComObj + 2).value(true, getDPT(VAL_DPT_1));
      break;

    case 2: // Schalten AUS/EIN (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Schalten AUS"));
#endif
      knx.getGroupObject(firstComObj + 2).value(false, getDPT(VAL_DPT_1));
      break;

    case 3: // Dimmen Heller/Dunkler (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Dimmen Heller"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 7, 1));
      dpt3value = 8;
      knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 7, 0));
      rockerNr[index] = 60;
      break;

    case 4: // Dimmen Dunkler/Heller (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Dimmen Dunkler"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 7, 1));
      dpt3value = 0;
      knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 7, 0));
      rockerNr[index] = 60;
      break;

    case 5: // Jalousie AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Jalousie AUF"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 8, 1));
      dpt3value = 0;
      knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
      rockerNr[index] = 60;
      break;

    case 6: // Jalousie AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.println(F("Jalousie AB"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 8, 1));
      dpt3value = 8;
      knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
      rockerNr[index] = 60;
      break;

    case 7: // Szene a/b (Oben/unten Wippe)
#ifdef KDEBUG
      SERIAL_PORT.print(F("Szene a: "));
      SERIAL_PORT.println(knx.paramByte(firstParameter + ENO_CHRockerCSzeneA));
#endif
      szeneNr = (knx.paramByte(firstParameter + ENO_CHRockerCSzeneA)) - 1;
      knx.getGroupObject(firstComObj + 2).value(szeneNr, getDPT(VAL_DPT_17));
      break;

    default:
      break;
    }
    break;

    //*************************************************************************************************************************************
    //
    //    RELEASE
    //
    //*************************************************************************************************************************************

    // Release AI = 0 --> most of the Rocker Switches answer a "0" for all Buttons (AI, AO, BI, BO, CI, CO)
  case AI_release:
    switch (rockerNr[index])
    {
    case 10:
      release_AI(firstParameter, firstComObj);
      break;
    case 20:
      release_AO(firstParameter, firstComObj);
      break;
    case 30:
      release_BI(firstParameter, firstComObj);
      break;
    case 40:
      release_BO(firstParameter, firstComObj);
      break;
    case 50:
      release_CI(firstParameter, firstComObj);
      break;
    case 60:
      release_CO(firstParameter, firstComObj);
      break;
    default:
      break;
    }
    rockerNr[index] = 0;
    break;

  case AO_release:
    release_AO(firstParameter, firstComObj);
    rockerNr[index] = 0;
    break;

  case BI_release:
    release_BI(firstParameter, firstComObj);
    rockerNr[index] = 0;
    break;

  case BO_release:
    release_BO(firstParameter, firstComObj);
    rockerNr[index] = 0;
    break;

  case CI_release:
    release_CI(firstParameter, firstComObj);
    rockerNr[index] = 0;
    break;

  case CO_release:
    release_CO(firstParameter, firstComObj);
    rockerNr[index] = 0;
    break;

  default:
    break;
  }
}

void handle_F6_05_0x(uint8_t firstComObj, uint8_t value)
{
  switch (value)
  {
  case 0x00: // Alarm OFF
    knx.getGroupObject(firstComObj + 3).value(true, getDPT(VAL_DPT_1));
    break;
  case 0x10: // Alarm ON
    knx.getGroupObject(firstComObj + 3).value(false, getDPT(VAL_DPT_1));
    break;
  case 0x11: // Water detected
    knx.getGroupObject(firstComObj + 3).value(true, getDPT(VAL_DPT_1));
    break;
  case 0x30: // Energy LOW
    knx.getGroupObject(firstComObj + 4).value(true, getDPT(VAL_DPT_1));
    break;
  default:
    break;
  }
}

void handle_RPS(PACKET_SERIAL_TYPE *f_Pkt_st, uint8_t profil, uint8_t firstComObj, uint8_t firstParameter)
{
  RPS_F6_10_00_TYPE *lRpsTlg_p;
  RPS_F6_10_01_TYPE *lRpsTlg2_p;

  bool bvalue;

#ifndef EnOceanTEST
  switch (knx.paramWord(firstParameter + ENO_CHProfilSelectionRPS))
#else
  switch (profil)
#endif
  {
  case F6_02_01:
#ifdef KDEBUG
    SERIAL_PORT.println(F("Profil: F6-02-01"));
#endif
    break;
  case F6_02_02:
#ifdef KDEBUG
    SERIAL_PORT.println(F("Profil: F6-02-02"));
#endif
    break;
  case F6_03_01:
#ifdef KDEBUG
    SERIAL_PORT.println(F("Profil: F6-03-01"));
#endif
    break;
  case F6_03_02:
#ifdef KDEBUG
    SERIAL_PORT.println(F("Profil: F6-03-02"));
#endif
    break;
  //**************************************************************
  // ----------------- Profil: F6-05-00   ------------------------
  //**************************************************************
  case F6_05_00:
#ifdef KDEBUG
    SERIAL_PORT.println(F("Profil: F6-05-00"));
#endif
    handle_F6_05_0x(firstComObj, f_Pkt_st->u8DataBuffer[1]);
    break;
  //**************************************************************
  // ----------------- Profil: F6-05-01 --------------------------
  //**************************************************************
  case F6_05_01:
#ifdef KDEBUG
    SERIAL_PORT.println(F("Profil: F6-05-01"));
#endif
    handle_F6_05_0x(firstComObj, f_Pkt_st->u8DataBuffer[1]);
    break;
    //**************************************************************
  // ----------------- Profil: F6-05-02   ------------------------
  //**************************************************************
  case F6_05_02:
#ifdef KDEBUG
    SERIAL_PORT.println(F("Profil: F6-05-02"));
#endif
    handle_F6_05_0x(firstComObj, f_Pkt_st->u8DataBuffer[1]);
    break;
  //**************************************************************
  // ----------------- Profil: F6-10-00 --------------------------
  //**************************************************************
  case F6_10_00:

#ifdef KDEBUG
    SERIAL_PORT.println(F("Profil: F6-10-00"));
#endif

    lRpsTlg_p = (RPS_F6_10_00_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);

#ifdef KDEBUG
    SERIAL_PORT.println(lRpsTlg_p->u8RpsTelData.STATE);
    SERIAL_PORT.println(lRpsTlg_p->u8RpsTelData.NA);
#endif

    switch (lRpsTlg_p->u8RpsTelData.STATE)
    {
    case 0b1111: // Hebel unten
      knx.getGroupObject(firstComObj + 1).value(false, getDPT(VAL_DPT_1));
      knx.getGroupObject(firstComObj + 2).value(false, getDPT(VAL_DPT_1));
      if (((knx.paramByte(firstParameter + ENO_CHWindowcloseValue))>>ENO_CHWindowcloseValueShift) & 1)
        bvalue = true;
      else
        bvalue = false;
      knx.getGroupObject(firstComObj + 3).value(bvalue, getDPT(VAL_DPT_1));
#ifdef KDEBUG
      SERIAL_PORT.println("Hebel unten");
#endif
      break;
    case 0b1101: // Hebel Oben
      knx.getGroupObject(firstComObj + 1).value(false, getDPT(VAL_DPT_1));
      knx.getGroupObject(firstComObj + 2).value(true, getDPT(VAL_DPT_1));
      if (((knx.paramByte(firstParameter + ENO_CHWindowcloseValue))>>ENO_CHWindowcloseValueShift) & 1)
        bvalue = false;
      else
        bvalue = true;
      knx.getGroupObject(firstComObj + 3).value(bvalue, getDPT(VAL_DPT_1));
#ifdef KDEBUG
      SERIAL_PORT.println("Hebel oben");
#endif
      break;
    case 0b1100: // Hebel Mitte
      knx.getGroupObject(firstComObj + 1).value(true, getDPT(VAL_DPT_1));
      knx.getGroupObject(firstComObj + 2).value(false, getDPT(VAL_DPT_1));
      if (((knx.paramByte(firstParameter + ENO_CHWindowcloseValue))>>ENO_CHWindowcloseValueShift) & 1)
        bvalue = false;
      else
        bvalue = true;
      knx.getGroupObject(firstComObj + 3).value(bvalue, getDPT(VAL_DPT_1));
#ifdef KDEBUG
      SERIAL_PORT.println("Hebel mitte");
#endif
      break;
    case 0b1110: // Hebel Mitte
      knx.getGroupObject(firstComObj + 1).value(true, getDPT(VAL_DPT_1));
      knx.getGroupObject(firstComObj + 2).value(false, getDPT(VAL_DPT_1));
      if (((knx.paramByte(firstParameter + ENO_CHWindowcloseValue))>>ENO_CHWindowcloseValueShift) & 1)
        bvalue = false;
      else
        bvalue = true;
      knx.getGroupObject(firstComObj + 3).value(bvalue, getDPT(VAL_DPT_1));
#ifdef KDEBUG
      SERIAL_PORT.println("Hebel mitte");
#endif
      break;
    default:
#ifdef KDEBUG
      SERIAL_PORT.println("RPS ERROR");
#endif
      break;
    }
    break;
  //**************************************************************
  // ----------------- Profil: F6-10-01 --------------------------
  //**************************************************************
  case F6_10_01:
    SERIAL_PORT.println(F("Profil: F6-10-01"));

    lRpsTlg2_p = (RPS_F6_10_01_TYPE *)&(f_Pkt_st->u8DataBuffer[1]);

    switch (lRpsTlg_p->u8RpsTelData.STATE)
    {
    case 0b11: // Hebel unten
      knx.getGroupObject(firstComObj + 2).value(false, getDPT(VAL_DPT_1));
      knx.getGroupObject(firstComObj + 3).value(false, getDPT(VAL_DPT_1));
      if (knx.paramByte(firstParameter + ENO_CHWindowcloseValue))
        bvalue = true;
      else
        bvalue = false;
      knx.getGroupObject(firstComObj + 3).value(bvalue, getDPT(VAL_DPT_1));  
#ifdef KDEBUG
      SERIAL_PORT.println("Hebel unten");
#endif
      break;
    case 0b01: // Hebel Oben
      knx.getGroupObject(firstComObj + 2).value(false, getDPT(VAL_DPT_1));
      knx.getGroupObject(firstComObj + 3).value(true, getDPT(VAL_DPT_1));
      if (knx.paramByte(firstParameter + ENO_CHWindowcloseValue))
        bvalue = false;
      else
        bvalue = true;
      knx.getGroupObject(firstComObj + 3).value(bvalue, getDPT(VAL_DPT_1));
#ifdef KDEBUG
      SERIAL_PORT.println("Hebel oben");
#endif
      break;
    case 0b00: // Hebel Mitte
      knx.getGroupObject(firstComObj + 2).value(true, getDPT(VAL_DPT_1));
      knx.getGroupObject(firstComObj + 3).value(false, getDPT(VAL_DPT_1));
      if (knx.paramByte(firstParameter + ENO_CHWindowcloseValue))
        bvalue = false;
      else
        bvalue = true;
      knx.getGroupObject(firstComObj + 3).value(bvalue, getDPT(VAL_DPT_1));
#ifdef KDEBUG
      SERIAL_PORT.println("Hebel mitte");
#endif
      break;
    case 0b10: // Hebel Mitte
      knx.getGroupObject(firstComObj + 2).value(true, getDPT(VAL_DPT_1));
      knx.getGroupObject(firstComObj + 3).value(false, getDPT(VAL_DPT_1));
      if (knx.paramByte(firstParameter + ENO_CHWindowcloseValue))
        bvalue = false;
      else
        bvalue = true;
      knx.getGroupObject(firstComObj + 3).value(bvalue, getDPT(VAL_DPT_1));
#ifdef KDEBUG
      SERIAL_PORT.println("Hebel mitte");
#endif
      break;
    default:
      // Statement(s)
      break;
    }
    break;
  default:
#ifdef KDEBUG
    SERIAL_PORT.println(F("Profil: ERROR"));
#endif
    break;
  }
}
