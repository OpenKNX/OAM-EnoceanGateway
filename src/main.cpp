#include "Logic.h"
#include "OpenKNX.h"
#ifndef ARDUINO_ARCH_RP2040
  #include "wiring_private.h" // pinPeripheral() function
#endif
// #include "Enocean.h"
#include "EnOceanHandle.h"

#ifndef ARDUINO_ARCH_RP2040
// XIAO PINS   RX = D3  TX = D2
Uart Serial2(&sercom2, 3, 2, SERCOM_RX_PAD_3, UART_TX_PAD_2);

void SERCOM2_Handler()
{
    Serial2.IrqHandler();
}
#endif

#ifdef ARDUINO_ARCH_RP2040
    #include "FileTransferModule.h"
    #include "UsbExchangeModule.h"
#endif

// #ifdef ARDUINO_ARCH_RP2040
//     #pragma message "Pico Core Version: " ARDUINO_PICO_VERSION_STR
// #endif

EnOceanDevice device[MAX_NUMBER_OF_DEVICES] = {EnOceanDevice()}; 
void setup()
{
    const uint8_t firmwareRevision = 0;

#ifdef ARDUINO_ARCH_RP2040
    // die defines musst du dann noch setzen
    Serial2.setRX(ENO_UART_RX_PIN);
    Serial2.setTX(ENO_UART_TX_PIN);
#endif

    openknx.init(firmwareRevision);
    openknx.addModule(1, openknxLogic);
    openknx.addModule(2, enOcean);
#ifdef ARDUINO_ARCH_RP2040
    openknx.addModule(3, openknxFileTransferModule);
    openknx.addModule(8, openknxUsbExchangeModule);
#endif

    openknx.setup();

#ifdef LED_YELLOW_PIN
    pinMode(LED_YELLOW_PIN, OUTPUT);
    digitalWrite(LED_YELLOW_PIN, HIGH);
#endif

    // start Enocean
    for (int i = 0; i < MAX_NUMBER_OF_DEVICES; i++)
    {
        enOcean.configureDevice(device[i], i);
    }

    Serial2.begin(57600); // Change to Serial wenn original Platine
                          // Assign pins 2 & 3 SERCOM functionality
#ifndef ARDUINO_ARCH_RP2040
    pinPeripheral(2, PIO_SERCOM_ALT);
    pinPeripheral(3, PIO_SERCOM_ALT);
#endif
    enOcean.initSerial(Serial2);
    enOcean.init2();

    // Set own BAse-ID for each Channel
    for (int i = 0; i < MAX_NUMBER_OF_DEVICES; i++)
    {
        enOcean.configureDeviceBaseID(device[i], i);
    }

    

}

void loop()
{
    openknx.loop();
}

