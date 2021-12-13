#include <Arduino.h>

#include <knx.h>
#include "hardwareENO.h"
#include "Helper.h"
#include "KnxHelper.h"
#include "EnoceanGateway.h"
#include "EnoceanProfils.h"

#include "Enocean.h"
#include "EnOceanDevice_T_H_VOC.h"

#include "Logic.h"

const uint8_t cFirmwareMajor = 1;    // 0-31
const uint8_t cFirmwareMinor = 0;    // 0-31
const uint8_t cFirmwareRevision = 0; // 0-63

Logic gLogic;

uint32_t heartbeatDelay = 0;
uint32_t startupDelay = 0;

// true solgange der Start des gesamten Moduls verzögert werden soll
bool startupDelayfunc()
{
  return !delayCheck(startupDelay, knx.paramWord(LOG_StartupDelay) * 1000);
}

void ProcessHeartbeat()
{
  // the first heartbeat is send directly after startup delay of the device
  if (heartbeatDelay == 0 || delayCheck(heartbeatDelay, knx.paramInt(LOG_Heartbeat) * 1000))
  {
    // we waited enough, let's send a heartbeat signal
    knx.getGroupObject(LOG_KoHeartbeat).value(true, getDPT(VAL_DPT_1));

    heartbeatDelay = millis();
  }
}

bool processDiagnoseCommand()
{
  char *lBuffer = gLogic.getDiagnoseBuffer();
  bool lOutput = false;
  if (lBuffer[0] == 'v')
  {
    // Command v: retrun fimware version, do not forward this to logic,
    // because it means firmware version of the outermost module
    sprintf(lBuffer, "VER [%d] %d.%d", cFirmwareMajor, cFirmwareMinor, cFirmwareRevision);
    lOutput = true;
  }
  else
  {
    // let's check other modules for this command
    lOutput = gLogic.processDiagnoseCommand();
  }
  return lOutput;
}

void ProcessDiagnoseCommand(GroupObject &iKo)
{
  // this method is called as soon as iKo is changed
  // an external change is expected
  // because this iKo also is changed within this method,
  // the method is called again. This might result in
  // an endless loop. This is prevented by the isCalled pattern.
  static bool sIsCalled = false;
  if (!sIsCalled)
  {
    sIsCalled = true;
    //diagnose is interactive and reacts on commands
    gLogic.initDiagnose(iKo);
    if (processDiagnoseCommand())
      gLogic.outputDiagnose(iKo);
    sIsCalled = false;
  }
};

void ProcessKoCallback(GroupObject &iKo)
{
  bool callLogic = false;
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
    else
    {
      callLogic = true;
    }
  }
  if (callLogic)
  {
    gLogic.processInputKo(iKo);
  }
}

void appSetup()
{
  if (knx.configured())
  {
    // set Firmware Version in ETS
    knx.bau().deviceObject().version(cFirmwareMajor << 11 | cFirmwareMinor << 6 | cFirmwareRevision);

    if (GroupObject::classCallback() == 0)
      GroupObject::classCallback(ProcessKoCallback);
    // Setup Logik
    gLogic.setup(false);
  }
}

void appLoop()
{
  if (!knx.configured())
    return;

  // handle KNX stuff
  if (startupDelayfunc())
    return;

  // at this point startup-delay is done
  // we process heartbeat
  ProcessHeartbeat();

  enOcean.task();

  gLogic.loop();
}
