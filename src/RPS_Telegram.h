#pragma once

#include "EnoceanProfils.h"
#include "KnxHelper.h"

/*
uint8_t getRockerTyp(uint8_t type)
{
  switch (type)
  {
  case AI_pressed:
    return ROCKER_AI;
    break;
  case AI_release:
    return ROCKER_AI;
    break;  
  case AO_pressed:
    return ROCKER_AO;
    break;
  case AO_release:
    return ROCKER_AO;
    break; 
  case BI_pressed:
    return ROCKER_BI;
    break;
  case BI_release:
    return ROCKER_BI;
    break;  
  case BO_pressed:
    return ROCKER_BO;
    break;
  case BO_release:
    return ROCKER_BO;
    break; 
  case CI_pressed:
    return ROCKER_CI;
    break;
  case CI_release:
    return ROCKER_CI;
    break;  
  case CO_pressed:
    return ROCKER_CO;
    break;
  case CO_release:
    return ROCKER_CO;
    break;       
  default:
    return ROCKER_INACTIVE;
    break;
  }
}*/

void shortSend_DPT1(bool value, uint16_t firstComObj)
{
#ifdef KDEBUG
  SERIAL_PORT.print(F("DPT1: "));
  SERIAL_PORT.println(value);
  SERIAL_PORT.println(firstComObj);
#endif
  knx.getGroupObject(firstComObj).value(value, getDPT(VAL_DPT_1));
}

void SendDPT3_007(bool dir, uint16_t firstComObj)
{
  uint8_t dpt3value = 0;
  //UP = 0
  //down = 1
#ifdef KDEBUG
  SERIAL_PORT.println(F("DIM: "));
  SERIAL_PORT.println(dir);
#endif
  dpt3value = 1;
  knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 7, 1));
  if (dir) //Increase
    dpt3value = 8;
  else // decrease
    dpt3value = 0;
  knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 7, 0));
}

void shortSend_Szene(uint8_t scene, uint16_t firstParameter, uint16_t firstComObj)
{
  uint8_t szeneNr;

#ifdef KDEBUG
  SERIAL_PORT.print(F("Szene: "));
  SERIAL_PORT.println(knx.paramByte(firstParameter + scene));
  SERIAL_PORT.println(firstComObj);
#endif
  szeneNr = (knx.paramByte(firstParameter + scene)) - 1;
  knx.getGroupObject(firstComObj).value(szeneNr, getDPT(VAL_DPT_17));
}

void stopDim(uint16_t firstComObj)
{
  uint8_t dpt3value = 0;

#ifdef KDEBUG
  SERIAL_PORT.println(F("Stop DIM"));
#endif
  dpt3value = 0;
  knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 7, 1));
  dpt3value = 0;
  knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 7, 0));
}

void longStop(uint8_t rockerNr, uint16_t firstParameter, uint16_t firstComObj)
{
  switch (rockerNr)
  {
  case AO_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("A0")); // AO = normal oben
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionA))
    {
    case 3: // Long: Licht Heller
      stopDim(firstComObj);
      break;
    case 4: // Long: Licht Dunkler
      stopDim(firstComObj);
      break;
    }
    break;

  case AI_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("AI")); // AI = normal unten
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionA))
    {
    case 3: // Long: Licht Dunkler
      stopDim(firstComObj);
      break;
    case 4: // Long: Licht Heller
      stopDim(firstComObj);
      break;
    }
    break;

  case BO_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("B0")); // BO = normal oben
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionB))
    {
    case 3: // Long: Licht Heller
      stopDim(firstComObj + 2);
      break;
    case 4: // Long: Licht Dunkler
      stopDim(firstComObj + 2);
      break;
    }
    break;

  case BI_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("BI")); // BI = normal unten
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionB))
    {
    case 3: // Long: Licht Dunkler
      stopDim(firstComObj + 2);
      break;
    case 4: // Long: Licht Heller
      stopDim(firstComObj + 2);
      break;
    }
    break;

  case CO_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("C0")); // CO = normal oben
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionC))
    {
    case 3: // Long: Licht Heller
      stopDim(firstComObj + 4);
      break;
    case 4: // Long: Licht Dunkler
      stopDim(firstComObj + 4);
      break;
    }
    break;

  case CI_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("CI")); // CI = normal unten
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionC))
    {
    case 3: // Long: Licht Dunkler
      stopDim(firstComObj + 4);
      break;
    case 4: // Long: Licht Heller
      stopDim(firstComObj + 4);
      break;
    }
    break;
  default:
    break;
  }
}

bool longPress(uint8_t rockerNr, uint16_t firstParameter, uint16_t firstComObj)
{
  uint8_t szeneNr;
  switch (rockerNr)
  {
  case AO_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("A0")); // AO = normal oben
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionA))
    {
    case 1: // Schalten EIN (Oben/unten Wippe)
      shortSend_DPT1(true, firstComObj);
      break;
    case 2: // Schalten AUS (Oben/unten Wippe)
      shortSend_DPT1(false, firstComObj);
      break;
    case 3: // Long: Licht Heller
      SendDPT3_007(true, firstComObj);
      return true;
      break;
    case 4: // Long: Licht Dunkler
      SendDPT3_007(false, firstComObj);
      return true;
      break;
    case 5: // Long: open = 0
      shortSend_DPT1(false, firstComObj+1);
      break;
    case 6: // Long: close = 1
      shortSend_DPT1(true, firstComObj+1);
      break;
    case 7: // Szene a/b (Oben/unten Wippe)
      shortSend_Szene(ENO_CHRockerASzeneA, firstParameter, firstComObj);
      break;
    }
    break;

  case AI_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("AI")); // AI = normal unten
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionA))
    {
    case 1: // Schalten AUS (Oben/unten Wippe)
      shortSend_DPT1(false, firstComObj);
      break;
    case 2: // Schalten AN (Oben/unten Wippe)
      shortSend_DPT1(true, firstComObj);
      break;
    case 3: // Long: Licht Dunkler
      SendDPT3_007(false, firstComObj);
      return true;
      break;
    case 4: // Long: Licht Heller
      SendDPT3_007(true, firstComObj);
      return true;
      break;
    case 5: // Long: Close = 1
      shortSend_DPT1(true, firstComObj+1);
      break;
    case 6: // Long: Open = 0
      shortSend_DPT1(false, firstComObj+1);
      break;
    case 7: // Szene b (Oben/unten Wippe)
      shortSend_Szene(ENO_CHRockerASzeneB, firstParameter, firstComObj);
      break;
    }
    break;

  case BO_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("B0")); // BO = normal oben
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionB))
    {
    case 1: // Schalten EIN (Oben/unten Wippe)
      shortSend_DPT1(true, firstComObj + 2);
      break;
    case 2: // Schalten AUS (Oben/unten Wippe)
      shortSend_DPT1(false, firstComObj + 2);
      break;
    case 3: // Long: Licht Heller
      SendDPT3_007(true, firstComObj + 2);
      return true;
      break;
    case 4: // Long: Licht Dunkler
      SendDPT3_007(false, firstComObj + 2);
      return true;
      break;
    case 5: // Long: open = 0
      shortSend_DPT1(false, firstComObj + 3);
      break;
    case 6: // Long: close = 1
      shortSend_DPT1(true, firstComObj + 3);
      break;
    case 7: // Szene a/b (Oben/unten Wippe)
      shortSend_Szene(ENO_CHRockerASzeneA, firstParameter, firstComObj + 2);
      break;
    }
    break;

  case BI_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("BI")); // BI = normal unten
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionB))
    {
    case 1: // Schalten AUS (Oben/unten Wippe)
      shortSend_DPT1(false, firstComObj + 2);
      break;
    case 2: // Schalten AN (Oben/unten Wippe)
      shortSend_DPT1(true, firstComObj + 2);
      break;
    case 3: // Long: Licht Dunkler
      SendDPT3_007(false, firstComObj + 2);
      return true;
      break;
    case 4: // Long: Licht Heller
      SendDPT3_007(true, firstComObj + 2);
      return true;
      break;
    case 5: // Long: Close = 1
      shortSend_DPT1(true, firstComObj + 3);
      break;
    case 6: // Long: Open = 0
      shortSend_DPT1(false, firstComObj + 3);
      break;
    case 7: // Szene b (Oben/unten Wippe)
      shortSend_Szene(ENO_CHRockerASzeneB, firstParameter, firstComObj + 2);
      break;
    }
    break;

  case CO_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("C0")); // CO = normal oben
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionC))
    {
    case 1: // Schalten EIN (Oben/unten Wippe)
      shortSend_DPT1(true, firstComObj + 4);
      break;
    case 2: // Schalten AUS (Oben/unten Wippe)
      shortSend_DPT1(false, firstComObj + 4);
      break;
    case 3: // Long: Licht Heller
      SendDPT3_007(true, firstComObj + 4);
      return true;
      break;
    case 4: // Long: Licht Dunkler
      SendDPT3_007(false, firstComObj + 4);
      return true;
      break;
    case 5: // Long: open = 0
      shortSend_DPT1(false, firstComObj + 5);
      break;
    case 6: // Long: close = 1
      shortSend_DPT1(true, firstComObj + 5);
      break;
    case 7: // Szene a/b (Oben/unten Wippe)
      shortSend_Szene(ENO_CHRockerASzeneA, firstParameter, firstComObj + 4);
      break;
    }
    break;

  case CI_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("CI")); // CI = normal unten
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionC))
    {
    case 1: // Schalten AUS (Oben/unten Wippe)
      shortSend_DPT1(false, firstComObj + 4);
      break;
    case 2: // Schalten AN (Oben/unten Wippe)
      shortSend_DPT1(true, firstComObj + 4);
      break;
    case 3: // Long: Licht Dunkler
      SendDPT3_007(false, firstComObj + 4);
      return true;
      break;
    case 4: // Long: Licht Heller
      SendDPT3_007(true, firstComObj + 4);
      return true;
      break;
    case 5: // Long: Close = 1
      shortSend_DPT1(true, firstComObj + 5);
      break;
    case 6: // Long: Open = 0
      shortSend_DPT1(false, firstComObj + 5);
      break;
    case 7: // Szene b (Oben/unten Wippe)
      shortSend_Szene(ENO_CHRockerASzeneB, firstParameter, firstComObj + 4);
      break;
    }
    break;
  default:
    break;
  }
  return false;
}

void shortPress(uint8_t rockerNr, uint16_t firstParameter, uint16_t firstComObj)
{
  uint8_t szeneNr;
  switch (rockerNr)
  {
  case AO_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("A0")); // AO = normal oben
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionA))
    {
    case 1: // Schalten EIN (Oben/unten Wippe)
      shortSend_DPT1(true, firstComObj);
      break;
    case 2: // Schalten AUS (Oben/unten Wippe)
      shortSend_DPT1(false, firstComObj);
      break;
    case 3: // short: Licht AN
      shortSend_DPT1(true, firstComObj);
      break;
    case 4: // short: Licht AUS
      shortSend_DPT1(false, firstComObj);
      break;
    case 5: // short: step up = 0
      shortSend_DPT1(false, firstComObj);
      break;
    case 6: // short: step down = 1
      shortSend_DPT1(true, firstComObj);
      break;
    case 7: // Szene a/b (Oben/unten Wippe)
      shortSend_Szene(ENO_CHRockerASzeneA, firstParameter, firstComObj);
      break;
    }
    break;

  case AI_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("AI")); // AI = normal unten
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionA))
    {
    case 1: // Schalten AUS (Oben/unten Wippe)
      shortSend_DPT1(false, firstComObj);
      break;
    case 2: // Schalten AN (Oben/unten Wippe)
      shortSend_DPT1(true, firstComObj);
      break;
    case 3: // short: Licht AUS
      shortSend_DPT1(false, firstComObj);
      break;
    case 4: // short: Licht EIN
      shortSend_DPT1(true, firstComObj);
      break;
    case 5: // short: step down = 1
      shortSend_DPT1(true, firstComObj);
      break;
    case 6: // short: step up = 0
      shortSend_DPT1(false, firstComObj);
      break;
    case 7: // Szene b (Oben/unten Wippe)
      shortSend_Szene(ENO_CHRockerASzeneB, firstParameter, firstComObj);
      break;
    }
    break;

  case BO_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("B0")); // BO = normal oben
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionB))
    {
    case 1: // Schalten EIN (Oben/unten Wippe)
      shortSend_DPT1(true, firstComObj + 2);
      break;
    case 2: // Schalten AUS (Oben/unten Wippe)
      shortSend_DPT1(false, firstComObj + 2);
      break;
    case 3: // short: Licht AN
      shortSend_DPT1(true, firstComObj + 2);
      break;
    case 4: // short: Licht AUS
      shortSend_DPT1(false, firstComObj + 2);
      break;
    case 5: // short: step up = 0
      shortSend_DPT1(false, firstComObj + 2);
      break;
    case 6: // short: step down = 1
      shortSend_DPT1(true, firstComObj + 2);
      break;
    case 7: // Szene a/b (Oben/unten Wippe)
      shortSend_Szene(ENO_CHRockerASzeneA, firstParameter, firstComObj + 2);
      break;
    }
    break;

  case BI_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("BI")); // BI = normal unten
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionB))
    {
    case 1: // Schalten AUS (Oben/unten Wippe)
      shortSend_DPT1(false, firstComObj + 2);
      break;
    case 2: // Schalten AN (Oben/unten Wippe)
      shortSend_DPT1(true, firstComObj + 2);
      break;
    case 3: // short: Licht AUS
      shortSend_DPT1(false, firstComObj + 2);
      break;
    case 4: // short: Licht EIN
      shortSend_DPT1(true, firstComObj + 2);
      break;
    case 5: // short: step down = 1
      shortSend_DPT1(true, firstComObj + 2);
      break;
    case 6: // short: step up = 0
      shortSend_DPT1(false, firstComObj + 2);
      break;
    case 7: // Szene b (Oben/unten Wippe)
      shortSend_Szene(ENO_CHRockerASzeneB, firstParameter, firstComObj + 2);
      break;
    }
    break;

  case CO_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("C0")); // CO = normal oben
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionC))
    {
    case 1: // Schalten EIN (Oben/unten Wippe)
      shortSend_DPT1(true, firstComObj + 4);
      break;
    case 2: // Schalten AUS (Oben/unten Wippe)
      shortSend_DPT1(false, firstComObj + 4);
      break;
    case 3: // short: Licht AN
      shortSend_DPT1(true, firstComObj + 4);
      break;
    case 4: // short: Licht AUS
      shortSend_DPT1(false, firstComObj + 4);
      break;
    case 5: // short: step up = 0
      shortSend_DPT1(false, firstComObj + 4);
      break;
    case 6: // short: step down = 1
      shortSend_DPT1(true, firstComObj + 4);
      break;
    case 7: // Szene a/b (Oben/unten Wippe)
      shortSend_Szene(ENO_CHRockerASzeneA, firstParameter, firstComObj + 4);
      break;
    }
    break;

  case CI_pressed:
#ifdef KDEBUG
    SERIAL_PORT.println(F("CI")); // CI = normal unten
#endif
    switch (knx.paramByte(firstParameter + ENO_CHRockerFunktionC))
    {
    case 1: // Schalten AUS (Oben/unten Wippe)
      shortSend_DPT1(false, firstComObj + 4);
      break;
    case 2: // Schalten AN (Oben/unten Wippe)
      shortSend_DPT1(true, firstComObj + 4);
      break;
    case 3: // short: Licht AUS
      shortSend_DPT1(false, firstComObj + 4);
      break;
    case 4: // short: Licht EIN
      shortSend_DPT1(true, firstComObj + 4);
      break;
    case 5: // short: step down = 1
      shortSend_DPT1(true, firstComObj + 4);
      break;
    case 6: // short: step up = 0
      shortSend_DPT1(false, firstComObj + 4);
      break;
    case 7: // Szene b (Oben/unten Wippe)
      shortSend_Szene(ENO_CHRockerASzeneB, firstParameter, firstComObj + 4);
      break;
    }
    break;
  default:
    break;
  }
}

void release_Button(bool stateIO, uint16_t firstParameter, uint16_t firstComObj, uint8_t RockerFunktion, uint8_t RockerSzene, bool islong)
{
  uint8_t szeneNr;
  uint8_t dpt3value = 0;

  switch (knx.paramByte(firstParameter + RockerFunktion))
  {
  case 1:        // Schalten EIN/AUS (Oben/unten Wippe)
    if (stateIO) // Button I
    {
#ifdef KDEBUG
      SERIAL_PORT.println(F("Schalten AUS"));
#endif
      knx.getGroupObject(firstComObj + 1).value(false, getDPT(VAL_DPT_1));
    }
    else // Button O
    {
#ifdef KDEBUG
      SERIAL_PORT.println(F("Schalten EIN"));
#endif
      knx.getGroupObject(firstComObj + 1).value(true, getDPT(VAL_DPT_1));
    }
    break;

  case 2:        // Schalten AUS/EIN (Oben/unten Wippe)
    if (stateIO) // Button I
    {
#ifdef KDEBUG
      SERIAL_PORT.println(F("Schalten EIN"));
#endif
      knx.getGroupObject(firstComObj + 1).value(true, getDPT(VAL_DPT_1));
    }
    else // Button O
    {
#ifdef KDEBUG
      SERIAL_PORT.println(F("Schalten AUS"));
#endif
      knx.getGroupObject(firstComObj + 1).value(false, getDPT(VAL_DPT_1));
    }
    break;

  case 3: // short: Licht AN/AUS   long: Dimmen Heller/Dunkler (Oben/unten Wippe)
    if (islong)
    {
      if (stateIO) // Button I
      {
#ifdef KDEBUG
        SERIAL_PORT.println(F("Dimmen heller"));
#endif
        dpt3value = 1;
        knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 7, 1));
        dpt3value = 8;
        knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 7, 0));
      }
      else
      {
#ifdef KDEBUG
        SERIAL_PORT.println(F("Dimmen Dunkler"));
#endif
        dpt3value = 1;
        knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 7, 1));
        dpt3value = 0;
        knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 7, 0));
      }
    }
    else
    {
      if (stateIO) // Button I
      {
#ifdef KDEBUG
        SERIAL_PORT.println(F("Licht  AUS"));
#endif
        knx.getGroupObject(firstComObj + 1).value(false, getDPT(VAL_DPT_1));
      }
      else
      {
#ifdef KDEBUG
        SERIAL_PORT.println(F("Licht  AN"));
#endif
        knx.getGroupObject(firstComObj + 1).value(true, getDPT(VAL_DPT_1));
      }
    }
    break;

  case 4: // short: Licht AN/AUS   long: Dimmen Heller/Dunkler (Oben/unten Wippe)
    if (islong)
    {
      if (stateIO) // Button I
      {
#ifdef KDEBUG
        SERIAL_PORT.println(F("Dimmen dunkler"));
#endif
        dpt3value = 1;
        knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 7, 1));
        dpt3value = 0;
        knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 7, 0));
      }
      else
      {
#ifdef KDEBUG
        SERIAL_PORT.println(F("Dimmen heller"));
#endif
        dpt3value = 1;
        knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 7, 1));
        dpt3value = 8;
        knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 7, 0));
      }
    }
    else
    {
      if (stateIO) // Button I
      {
#ifdef KDEBUG
        SERIAL_PORT.println(F("Licht AN"));
#endif
        knx.getGroupObject(firstComObj + 1).value(true, getDPT(VAL_DPT_1));
      }
      else
      {
#ifdef KDEBUG
        SERIAL_PORT.println(F("Licht  AUS"));
#endif
        knx.getGroupObject(firstComObj + 1).value(false, getDPT(VAL_DPT_1));
      }
    }
    break;

  case 5: // short: step AB/AUF   long: Jalousie AUF/AB (Oben/unten Wippe)
    if (islong)
    {
      if (stateIO) // Button I
      {
#ifdef KDEBUG
        SERIAL_PORT.println(F("Jalousie AB"));
#endif
        knx.getGroupObject(firstComObj + 1).value(true, getDPT(VAL_DPT_1));
      }
      else
      {
#ifdef KDEBUG
        SERIAL_PORT.println(F("Jalousie AUF"));
#endif
        knx.getGroupObject(firstComObj + 1).value(false, getDPT(VAL_DPT_1));
      }
    }
    else
    {
      if (stateIO) // Button I
      {
#ifdef KDEBUG
        SERIAL_PORT.println(F("Jalousie Step AB"));
#endif
        dpt3value = 1;
        knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 8, 1));
        dpt3value = 8;
        knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 8, 0));
      }
      else
      {
#ifdef KDEBUG
        SERIAL_PORT.println(F("Jalousie Step AUF"));
#endif
        dpt3value = 1;
        knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 8, 1));
        dpt3value = 0;
        knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 8, 0));
      }
    }
    break;

  case 6: // Jalousie AB/AUF (Oben/unten Wippe)
    if (islong)
    {
      if (stateIO) // Button I
      {
#ifdef KDEBUG
        SERIAL_PORT.println(F("Jalousie AB"));
#endif
        knx.getGroupObject(firstComObj + 1).value(true, getDPT(VAL_DPT_1));
      }
      else
      {
#ifdef KDEBUG
        SERIAL_PORT.println(F("Jalousie AUF"));
#endif
        knx.getGroupObject(firstComObj + 1).value(false, getDPT(VAL_DPT_1));
      }
    }
    else
    {
      if (stateIO) // Button I
      {
#ifdef KDEBUG
        SERIAL_PORT.println(F("Jalousie Step AUF"));
#endif
        dpt3value = 1;
        knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 8, 1));
        dpt3value = 0;
        knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 8, 0));
      }
      else
      {
#ifdef KDEBUG
        SERIAL_PORT.println(F("Jalousie Step AB"));
#endif
        dpt3value = 1;
        knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 8, 1));
        dpt3value = 8;
        knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 8, 0));
      }
    }
    break;

  case 7: // Szene a/b (Oben/unten Wippe)
#ifdef KDEBUG
    if (stateIO) // Button I
    {
      SERIAL_PORT.print(F("Szene b: "));
      SERIAL_PORT.println(knx.paramByte(firstParameter + RockerSzene));
    }
    else
    {
      SERIAL_PORT.print(F("Szene a: "));
      SERIAL_PORT.println(knx.paramByte(firstParameter + RockerSzene));
    }
#endif
    szeneNr = (knx.paramByte(firstParameter + RockerSzene)) - 1;
    knx.getGroupObject(firstComObj + 1).value(szeneNr, getDPT(VAL_DPT_17));

    break;

  default:
    break;
  }
}

/*
void release_AI(uint8_t firstParameter, uint8_t firstComObj, bool islong)
{
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

  case 3: // LICHT AN/AUS (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("Licht AUS"));
#endif
    knx.getGroupObject(firstComObj).value(false, getDPT(VAL_DPT_1));
    break;

  case 4: // LICHT AUS/AN (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("Licht AN"));
#endif
    knx.getGroupObject(firstComObj).value(true, getDPT(VAL_DPT_1));
    break;

  case 5: // Jalousie Step AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("Jalousie Step AB"));
#endif
    dpt3value = 1;
    knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 8, 1)); // schreibt die Schrittweite, ohne zu senden
    dpt3value = 8;
    knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 8, 0)); // schreibt die Richtung und sendet alles (auch die Schrittweite)
    break;

  case 6: // Jalousie Step AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("Jalousie Step AUF"));
#endif
    dpt3value = 1;
    knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 8, 1)); // schreibt die Schrittweite, ohne zu senden
    dpt3value = 0;
    knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 8, 0)); // schreibt die Richtung und sendet alles (auch die Schrittweite)
    break;

  case 7: // Szene a/b (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.print(F("Szene b: "));
    SERIAL_PORT.println(knx.paramByte(firstParameter + ENO_CHRockerASzeneB));
#endif
    szeneNr = (knx.paramByte(firstParameter + ENO_CHRockerASzeneB)) - 1;
    knx.getGroupObject(firstComObj).value(szeneNr, getDPT(VAL_DPT_17));
    break;
  } // ENDE SWITCH RockerFunktionA
}

void release_AO(uint8_t firstParameter, uint8_t firstComObj, bool islong)
{
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
    SERIAL_PORT.println(F("Licht EIN"));
#endif
    knx.getGroupObject(firstComObj).value(true, getDPT(VAL_DPT_1));
    break;

  case 4: // Dimmen Dunkler/Heller (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("Licht AUS"));
#endif
    knx.getGroupObject(firstComObj).value(false, getDPT(VAL_DPT_1));
    break;

  case 5: // Jalousie AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("Jalousie AUF"));
#endif
    dpt3value = 1;
    knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 0;
    knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 6: // Jalousie AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("Jalousie AB"));
#endif
    dpt3value = 1;
    knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 8;
    knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
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
}

void release_BI(uint8_t firstParameter, uint8_t firstComObj, bool islong)
{
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
    SERIAL_PORT.println(F("Licht AUS"));
#endif
    knx.getGroupObject(firstComObj + 1).value(false, getDPT(VAL_DPT_1));
    break;

  case 4: // Dimmen Dunkler/Heller (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("Licht AN"));
#endif
    knx.getGroupObject(firstComObj + 1).value(true, getDPT(VAL_DPT_1));
    break;

  case 5: // Jalousie AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("Jalousie AB"));
#endif
    dpt3value = 1;
    knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 8;
    knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 6: // Jalousie AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("Jalousie AUF"));
#endif
    dpt3value = 1;
    knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
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
}

void release_BO(uint8_t firstParameter, uint8_t firstComObj, bool islong)
{
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
    SERIAL_PORT.println(F("Licht AN"));
#endif
    knx.getGroupObject(firstComObj + 1).value(true, getDPT(VAL_DPT_1));
    break;

  case 4: // Dimmen Dunkler/Heller (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("Licht AUS"));
#endif
    knx.getGroupObject(firstComObj + 1).value(false, getDPT(VAL_DPT_1));
    break;

  case 5: // Jalousie AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("Jalousie AUF"));
#endif
    dpt3value = 1;
    knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 6: // Jalousie AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("Jalousie AB"));
#endif
    dpt3value = 1;
    knx.getGroupObject(firstComObj + 1).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 8;
    knx.getGroupObject(firstComObj + 1).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
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
}

void release_CI(uint8_t firstParameter, uint8_t firstComObj, bool islong)
{
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
    SERIAL_PORT.println(F("Licht AUS"));
#endif
    knx.getGroupObject(firstComObj + 2).value(false, getDPT(VAL_DPT_1));
    break;

  case 4: // Dimmen Dunkler/Heller (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("Licht AN"));
#endif
    knx.getGroupObject(firstComObj + 2).value(true, getDPT(VAL_DPT_1));
    break;

  case 5: // Jalousie AUF/AB (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("Jalousie AB"));
#endif
    dpt3value = 1;
    knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 8;
    knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    break;

  case 6: // Jalousie AB/AUF (Oben/unten Wippe)
#ifdef KDEBUG
    SERIAL_PORT.println(F("Jalousie AUF"));
#endif
    dpt3value = 1;
    knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 8, 1));
    dpt3value = 0;
    knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
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
}

void release_CO(uint8_t firstParameter, uint8_t firstComObj, bool islong)
{
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

  case 3: // short: Licht AN/AUS   long: Dimmen Heller/Dunkler (Oben/unten Wippe)
    if (islong)
    {
#ifdef KDEBUG
      SERIAL_PORT.println(F("Dimmen Dunkler"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 7, 1));
      dpt3value = 0;
      knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 7, 0));
    }
    else
    {
#ifdef KDEBUG
      SERIAL_PORT.println(F("Licht  AN"));
#endif
      knx.getGroupObject(firstComObj + 2).value(true, getDPT(VAL_DPT_1));
    }
    break;

  case 4: // short: Licht AN/AUS   long: Dimmen Heller/Dunkler (Oben/unten Wippe)
    if (islong)
    {
#ifdef KDEBUG
      SERIAL_PORT.println(F("Dimmen heller"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj).valueNoSend(dpt3value, Dpt(3, 7, 1));
      dpt3value = 8;
      knx.getGroupObject(firstComObj).value(dpt3value, Dpt(3, 7, 0));
    }
    else
    {
#ifdef KDEBUG
      SERIAL_PORT.println(F("Licht  AUS"));
#endif
      knx.getGroupObject(firstComObj + 2).value(false, getDPT(VAL_DPT_1));
    }
    break;

  case 5: // short: step AB/AUF   long: Jalousie AUF/AB (Oben/unten Wippe)
    if (islong)
    {
#ifdef KDEBUG
      SERIAL_PORT.println(F("Jalousie AUF"));
#endif
      knx.getGroupObject(firstComObj + 2).value(false, getDPT(VAL_DPT_1));
    }
    else
    {
#ifdef KDEBUG
      SERIAL_PORT.println(F("Jalousie Step AUF"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 8, 1));
      dpt3value = 0;
      knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    }
    break;

  case 6: // Jalousie AB/AUF (Oben/unten Wippe)
    if (islong)
    {
#ifdef KDEBUG
      SERIAL_PORT.println(F("Jalousie AUF"));
#endif
      knx.getGroupObject(firstComObj + 2).value(false, getDPT(VAL_DPT_1));
    }
    else
    {
#ifdef KDEBUG
      SERIAL_PORT.println(F("Jalousie Step AB"));
#endif
      dpt3value = 1;
      knx.getGroupObject(firstComObj + 2).valueNoSend(dpt3value, Dpt(3, 8, 1));
      dpt3value = 8;
      knx.getGroupObject(firstComObj + 2).value(dpt3value, Dpt(3, 8, 0)); // ------------------->> VALUE ???? <--------------------------------------
    }
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
}
*/

uint8_t handle_RPS_Rocker(PACKET_SERIAL_TYPE *f_Pkt_st, uint8_t profil, uint8_t firstComObj, uint8_t firstParameter, uint8_t index)
{

  switch (f_Pkt_st->u8DataBuffer[1])
  {
  case AI_pressed:
    return AI_pressed;
    break; // ENDE AI_pressed

  case AO_pressed:
    return AO_pressed;
    break; // ENDE AO_pressed

  case BI_pressed:
    return BI_pressed;
    break; // ENDE BI_pressed

  case BO_pressed:
    return BO_pressed;
    break; // ENDE BO_pressed

  case CI_pressed:
    return CI_pressed;
    break; // ENDE CI_pressed

  case CO_pressed:
    return CO_pressed;
    break; // ENDE CO_pressed

    //*************************************************************************************************************************************
    //
    //    RELEASE
    //
    //*************************************************************************************************************************************

    /*
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
    return AI_release;
    break; // ENDE AI_release

  case AO_release:
    release_AO(firstParameter, firstComObj);
    rockerNr[index] = 0;
    return AO_release;
    break; // ENDE AO_release

  case BI_release:
    release_BI(firstParameter, firstComObj);
    rockerNr[index] = 0;
    return BI_release;
    break; // ENDE BI_release

  case BO_release:
    release_BO(firstParameter, firstComObj);
    rockerNr[index] = 0;
    return BO_release;
    break; // ENDE BO_release

  case CI_release:
    release_CI(firstParameter, firstComObj);
    rockerNr[index] = 0;
    return CI_release;
    break; // ENDE CI_release

  case CO_release:
    release_CO(firstParameter, firstComObj);
    rockerNr[index] = 0;
    return CO_release;
    break; // ENDE CO_release

  default:
  
    break;
    */
  }
}

void handle_F6_05_0x(uint16_t firstComObj, uint8_t value)
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

void handle_RPS(PACKET_SERIAL_TYPE *f_Pkt_st, uint8_t profil, uint16_t firstComObj, uint16_t firstParameter)
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
      if (((knx.paramByte(firstParameter + ENO_CHWindowcloseValue)) >> ENO_CHWindowcloseValueShift) & 1)
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
      if (((knx.paramByte(firstParameter + ENO_CHWindowcloseValue)) >> ENO_CHWindowcloseValueShift) & 1)
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
      if (((knx.paramByte(firstParameter + ENO_CHWindowcloseValue)) >> ENO_CHWindowcloseValueShift) & 1)
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
      if (((knx.paramByte(firstParameter + ENO_CHWindowcloseValue)) >> ENO_CHWindowcloseValueShift) & 1)
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
