#include <Arduino.h>

#include <knx.h>
#include "hardware.h"
#include "KnxHelper.h"
#include "EnoceanGateway.h"
#include "EnoceanProfils.h"

#include "Enocean.h"

#include "EnOceanDevice_T_H_VOC.h"

uint32_t heartbeatDelay = 0;
uint32_t startupDelay = 0;

// true solgange der Start des gesamten Moduls verzögert werden soll
bool startupDelayfunc()
{
  return !delayCheck(startupDelay, knx.paramInt(ENO_StartupDelay) * 1000);
}

void ProcessHeartbeat()
{
  // the first heartbeat is send directly after startup delay of the device
  if (heartbeatDelay == 0 || delayCheck(heartbeatDelay, knx.paramInt(ENO_Heartbeat) * 1000))
  {
    // we waited enough, let's send a heartbeat signal
    knx.getGroupObject(ENO_KoHeartbeat).value(true, getDPT(VAL_DPT_1));

    heartbeatDelay = millis();
  }
}

void ProcessKoCallback(GroupObject &iKo)
{
  for (int koIndex = 0; koIndex < MAX_NUMBER_OF_DEVICES; koIndex++)
  {

    if (iKo.asap() == ENO_KoOffset + (ENO_KoGO_BASE__1 + (koIndex * ENO_KoBlockSize)))
    {
      SERIAL_PORT.println("reviev KO_0");
      enOcean.handleKnxEvents(koIndex, 0, iKo);
    }
    else if (iKo.asap() == ENO_KoOffset + (ENO_KoGO_BASE__2 + (koIndex * ENO_KoBlockSize)))
    {
      SERIAL_PORT.println("reviev KO_1");
      enOcean.handleKnxEvents(koIndex, 1, iKo);
    }
    else if (iKo.asap() == ENO_KoOffset + (ENO_KoGO_BASE__3 + (koIndex * ENO_KoBlockSize)))
    {
      SERIAL_PORT.println("reviev KO_2");
      enOcean.handleKnxEvents(koIndex, 2, iKo);
    }
    else if (iKo.asap() == ENO_KoOffset + (ENO_KoGO_BASE__4 + (koIndex * ENO_KoBlockSize)))
    {
      SERIAL_PORT.println("reviev KO_3");
      enOcean.handleKnxEvents(koIndex, 3, iKo);
    }
    else if (iKo.asap() == ENO_KoOffset + (ENO_KoGO_BASE__5 + (koIndex * ENO_KoBlockSize)))
    {
      SERIAL_PORT.println("reviev KO_4");
      enOcean.handleKnxEvents(koIndex, 4, iKo);
    }
  }
}

void appSetup()
{
  if (knx.configured())
  {
    if (GroupObject::classCallback() == 0)
      GroupObject::classCallback(ProcessKoCallback);
  }
}

void appLoop()
{
#ifdef KNXenable
  if (!knx.configured())
    return;

  // handle KNX stuff
  if (startupDelayfunc())
    return;

  // at this point startup-delay is done
  // we process heartbeat
  ProcessHeartbeat();
#endif

  enOcean.task();
}
