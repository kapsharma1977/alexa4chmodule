

// first include macros.h
#include "macros.h"

//#include <Arduino.h>
#include <Ticker.h>
////////////////
#include "fauxmoESP.h"
#include <ESPAsyncWebServer.h>
#include "serverFactory.h"
#include "devicestruct.h"
#include "espdevices.h"
#include "rtcds1307.h"
#include "encryp.h"

#ifndef EEPROM_SIZE_T
#define EEPROM_SIZE_T 512
#endif


bool first_time_boot = false;
Ticker secondTick;
volatile int watchdogCount = 0;
volatile unsigned long pairmillis = 0;
fauxmoESP *fauxmo = nullptr;
;
AsyncWebServer *localserver = nullptr;
serverFactory sf;

/* Put IP Address details */
// IPAddress local_ip(192,168,4,1);
// IPAddress gateway(192,168,4,1);
// IPAddress subnet(255,255,255,0);

struct DeviceStruct devwifieeprom;

espDevices nodemcu_esp;
ds1307rtc rtc;
//uint8_t tmo = 33;
bool flagreconnect = false;  // only for first time execution of connect function. it is guranteed that it will execute in setup()
//uint8_t alexa_d_c = ALEXA_DELAY_COUNT;

//uint32_t Chipid;
//uint32_t flashid;


void ISRwatchdog() {
  watchdogCount++;
  if (watchdogCount == 25) {
    // Only print to serial when debugging
    DBGPRINTLN("The dog bites!");
    ESP.reset();
  }
}

void restconfig() {
  nodemcu_esp.Reset();
  default_wifi(devwifieeprom);
  // work remaing for SoftAP and other
}

void setup() {
// put your setup code here, to run once:
// Init serial port and clean garbage
#if DEBUG
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial)
    ;  // Wait for Serial Monitor
  DBGPRINTLN("");
  DBGPRINTLN("Debug Mode: ON");
#endif
  delay(500);

  // DS1307 RTC initalization
  rtc.initRTC();

  // if DS1307 is working then read from DS1307 AT24C32 32KBits aka 4096 Bytes of page size 32 bytes at I2C address 0x50
  if (rtc.isAT24C32())
    rtc.loadDeviceStruct();
  else                           // read flash memory as esp8266 do not have EEPROM instead it uses flash memory as EEPROM(Emulated)
    read_eeprom(devwifieeprom);  // read flash memory as esp8266 do not have EEPROM instead it uses flash memory as EEPROM(Emulated
  delay(500);
  print_DeviceStruct(devwifieeprom);



  // encryption
  encryp cryp;  // create/compute key
  // check MCU is powed for first time in it's life. // check esp8266 is powed on first time in its life
  if (cryp.IsKey())
    for (uint8_t i = 0; i < LEN; i++)
      if (devwifieeprom.key[i] != cryp.charAt(i)) {
        default_DeviceStruct(devwifieeprom);
        first_time_boot = true;
        DBGPRINTLN("MCU is powed for first time in it's life.");
        print_DeviceStruct(devwifieeprom);
        break;
      }
  // try to connect to WiFi and update clock
  if (nodemcu_esp.reconnectSTA())
    rtc.checkandupdateRTC(10);  // attempts 10

  nodemcu_esp.setup();
  delay(500);
  nodemcu_esp.Print();
  nodemcu_esp.Print_status();


  if (nodemcu_esp.IsAlexaEnable()) {
    if (nodemcu_esp.reconnectSTA()) {
      fauxmo = serverFactory::createfauxmo(fauxmo);
      sf.setfauxmo(fauxmo);
      nodemcu_esp.statusled.Off();
      DBGPRINTLN("Alexa Starts : fauxmo");
    }
  } else {
    DBGPRINTLN("nodemcu_esp.StationAP()");
    nodemcu_esp.StationAP();
    localserver = serverFactory::createlocalserver(localserver);
    sf.setlocalserver(localserver);
    nodemcu_esp.statusled.On();
    DBGPRINTLN("localserver = serverFactory::setuplocalserver();");
  }

  delay(500);
  DBGPRINTLN("Off All Deviecs");
  nodemcu_esp.OffAllDev();
  // DBGPRINTLN("On All Deviecs");
  // nodemcu_esp.OnAllDev();
#if DEBUG
  DBGPRINTLN("###################################");
  DBGPRINT("[MAIN] Free heap bytes : ");
  DBGPRINTLN(ESP.getFreeHeap());
#else
  ESP.getFreeHeap();
#endif
}

void loop() {
  // put your main code here, to run repeatedly:

  // Feeding the dog
  watchdogCount = 0;

  // Alexa comunication
  sf.fauxmoHandel();

  // MCU Action
  nodemcu_esp.Loop();
}
