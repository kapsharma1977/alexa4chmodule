#pragma once

#include "macros.h"
#include "mcupin.h"
#include "devicestruct.h"
#include "rtcds1307.h"
#include "Device.h"
//#include <ESPAsyncWebServer.h>
//#include "fauxmoESP.h"


// push button very short press count for DEBUG purpose only
#ifndef VERYSHORTPRESS
  #define VERYSHORTPRESS 3
#endif
// push button short press count for entring into Paring Mode for Alexa Device Discovery
#ifndef SHORTPRESS
  #define SHORTPRESS 9
#endif

extern struct DeviceStruct devwifieeprom;
extern ds1307rtc rtc;

class espDevices{
  private:
    void _D7GPIO13ButtonLoop();
  public:
    uint8_t *mcumode; // == //ALEXA_MODE, SCHEDULER_MODE or ONOF_MODE
    volatile unsigned long lastD7millis; volatile unsigned long clocksyncmillis;
    uint8_t pair; // Counter for device discovery in Alexa
    Device D_3;// config :D3_GPIO0, no external resistor false, IOmode = OUTPUT, true(pull-up)) //Purpose: triggering rlay // Limitation/Boot Behavior : Pull-up during boot so flickering problum 
    Device D_4;// config :D4_GPIO2, no external resistor false, IOmode = OUTPUT, true(pull-up)) //Purpose: triggering rlay // Limitation/Boot Behavior : Pull-up during boot so flickering problum 
    Device D_5;// config :D5_GPIO14, no external resistor false, IOmode = OUTPUT, true(pull-up)) //Purpose: triggering rlay 
    Device D_6; // config :D6_GPIO12, no external resistor false, IOmode = OUTPUT, true(pull-up)) //Purpose: triggering rlay
    PushButton modes;
    mcustatus statusled;
    #if EXSITING_PCB
    espDevices(): D_3(D3_GPIO0),D_4(D4_GPIO2),D_5(D5_GPIO14),D_6(D6_GPIO12) \
      ,modes(D7_GPIO13,false,INPUT,true) \
      ,statusled(D8_GPIO15,true,OUTPUT,false) \
      { lastD7millis = 0;clocksyncmillis = 0;pair=0;}
      // config :D7_GPIO13, no external resistor false, IOmode = INPUT, true(pull-up)) //Purpose: foR paring mode, fectory reset, and debug print
      // config :D8_GPIO15,external resistor 10K true, IOmode = OUTPUT, false(pull-down)) //Purpose: mcu alexa state; led off alexa is connected; blinking then paring mode
      // Important : 
          // circuit connection is GND --- Push Button --- D7
          // GND --- 10K --- D8
          // GND --- 220ohm --- (- red led +) --- D8
    #else // new PCB
    espDevices(): D_3(D3_GPIO0),D_4(D4_GPIO2),D_5(D5_GPIO14),D_6(D6_GPIO12) \
      ,statusled(D7_GPIO13,false,OUTPUT,true) \
      ,modes(D8_GPIO15,true,INPUT,false) \
      { lastD7millis = 0;clocksyncmillis = 0;pair=0;}
      // config :D7_GPIO13, no external resistor false, IOmode = OUTPUT, true(pull-up)) //Purpose: mcu alexa state; led off alexa is connected; blinking then paring mode
      // config :D8_GPIO15,external resistor 10K true, IOmode = INPUT, false(pull-down)) //Purpose: foR paring mode, fectory reset, and debug print
      // Important : 
          // circuit connection is 3.3v --- 220ohm --- (+ red led -) --- D7
          // LOW(D7) : current will flow because D7 sinks current as it become GND
          // HIGH : no current flow because D7 become source it will emit current at 3.3v so both end same voltage this emply no current
    #endif
    // // #if EXSITING_PCB
    // // PushButton D7;// config :D7_GPIO13, no external resistor false, IOmode = INPUT, true(pull-up)) //Purpose: foR paring mode, fectory reset, and debug print
    // // mcustatus D8; // D8_GPIO15,external resistor 10K true, IOmode = OUTPUT, false(pull-down)) //Purpose: mcu alexa state; led off alexa is connected; blinking then paring mode
    // // espDevices(): D_3(D3_GPIO0),D_4(D4_GPIO2),D_5(D5_GPIO14),D_6(D6_GPIO12) \
    // //   ,D7(D7_GPIO13,false,INPUT,true) \
    // //   ,D8(D8_GPIO15,true,OUTPUT,false) \
    // //   { lastD7millis = 0;clocksyncmillis = 0;pair=0;}
    // // #else
    // // mcustatus D7; // D8_GPIO15,external resistor 10K true, IOmode = OUTPUT, false(pull-down)) //Purpose: mcu alexa state; led off alexa is connected; blinking then paring mode
    // // PushButton D8;// config :D7_GPIO13, no external resistor false, IOmode = INPUT, true(pull-up)) //Purpose: foR paring mode, fectory reset, and debug print
    // // espDevices(): D_3(D3_GPIO0),D_4(D4_GPIO2),D_5(D5_GPIO14),D_6(D6_GPIO12) \
    // //   // ,D7(D7_GPIO13,false,INPUT,true)
    // //   // ,D8(D8_GPIO15,true,OUTPUT,false)
    // //   ,D7(D7_GPIO13,false,INPUT,true) \
    // //   ,D8(D8_GPIO15,true,OUTPUT,false) \
    // //   { lastD7millis = 0;clocksyncmillis = 0;pair=0;}
    // // #endif

    // // espDevices(): D_3(D3_GPIO0),D_4(D4_GPIO2),D_5(D5_GPIO14),D_6(D6_GPIO12) \
    // //   // ,D7(D7_GPIO13,false,INPUT,true)
    // //   // ,D8(D8_GPIO15,true,OUTPUT,false)
    // //   ,D7(D7_GPIO13,false,INPUT,true) \
    // //   ,D8(D8_GPIO15,true,OUTPUT,false) \
    // //   { lastD7millis = 0;clocksyncmillis = 0;pair=0;}
    void setup();
    void Loop();
    void Reset(){D_3.reset(); D_4.reset(); D_5.reset(); D_6.reset();} // Alexa Device will be factory name 
    void FactoryRest(){ResetEEPROM(devwifieeprom);delay(500);ESP.reset();}
    
    void Enable(){D_3.setEnable(true);D_4.setEnable(true);D_5.setEnable(true); D_6.setEnable(true);} // call it to dissable alexa; bool      *_enable; // local(non alexa) on off schedule is enable or not; // do noting; logic for onof is disabled; device is controlled manualy or by alexa
    void Disable(); // call for alexa mode; 
    
    //void setmcumode(uint8_t mode){ *mcumode = ((mode == ALEXA_MODE) || (mode == ONOF_MODE) || (mode == SCHEDULER_MODE)) ? mode : *mcumode;}
    void setmcumode(uint8_t mode){ *mcumode = ((mode == ALEXA_MODE) || (mode == CONFIG_MODE) ) ? mode : *mcumode;}
    
    void EnableAlexa(){ devwifieeprom.mcumode = ALEXA_MODE; }
    void EnableConfig(){devwifieeprom.mcumode = CONFIG_MODE;}
    // void EnableAlexa(){ (*mcumode != ALEXA_MODE) ? setmcumode(ALEXA_MODE) : (void)0;}
    // void EnableConfig(){(*mcumode != CONFIG_MODE) ? setmcumode(CONFIG_MODE) : (void)0;}
    //void EnableManulMode(); // imply on off by user clicking; disable loacl schedule timings stored in memory DeviceStruct
    //void EnableSchedulerMode(); // imply enable loacl schedule timings stored in memory DeviceStruct
    
    bool IsAlexaEnable(){return (devwifieeprom.mcumode == ALEXA_MODE) ? true : false;}
    bool IsConfigEnable(){return (devwifieeprom.mcumode == CONFIG_MODE) ? true : false;}
    //bool IsManulMode(){return (*mcumode == ONOF_MODE) ? true : false;}
    //bool IsSchedulerMode(){return (*mcumode == SCHEDULER_MODE) ? true : false;}

    //bool setDeviceManualMode(Device d_){return (*mcumode == ONOF_MODE) ? !d_.setEnable(false) : false;}
    //bool setDeviceSchedulerMode(Device d_){return (*mcumode == SCHEDULER_MODE) ? d_.setEnable(true) : false;}

    //bool IsDeviceManualMode(Device d_){return (*mcumode == ONOF_MODE) ? !d_.IsEnable() : false;}
    //bool IsDeviceSchedulerMode(Device d_){return (*mcumode == SCHEDULER_MODE) ? d_.IsEnable() : false;}

    // bool IsD3ManualMode(){return IsDeviceManualMode(D_3);}
    // bool IsD4ManualMode(){return IsDeviceManualMode(D_4);}
    // bool IsD5ManualMode(){return IsDeviceManualMode(D_5);}
    // bool IsD6ManualMode(){return IsDeviceManualMode(D_6);}
    
    // bool IsD3SchedulerMode(){return IsDeviceSchedulerMode(D_3);}
    // bool IsD4SchedulerMode(){return IsDeviceSchedulerMode(D_4);}
    // bool IsD5SchedulerMode(){return IsDeviceSchedulerMode(D_5);}
    // bool IsD6SchedulerMode(){return IsDeviceSchedulerMode(D_6);}
    
    bool reconnectSTA(uint8_t re_tmo = TMO);
    bool StationAP();
    // void clockSync(ds1307rtc& d1307rtc, uint8_t attempts = 5);
    // void esp_clocksync(DeviceStruct& conf,ds1307rtc& d1307rtc);

    void OffAllDev();
    void OnAllDev();
    void Print();
    void Print_status();
};