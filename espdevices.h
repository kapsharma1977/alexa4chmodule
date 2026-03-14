#ifndef H_DEVICESTUCT
  #define H_DEVICESTUCT
  #include "devicestruct.h"
#endif
#ifndef H_RTCDS1307
  #define H_RTCDS1307
  #include "rtcds1307.h"
#endif

#ifndef H_DEVICE
  #define H_DEVICE
  #include "Device.h"
#endif

#ifndef H_ASYNCWEBSERVER
  #define H_ASYNCWEBSERVER
  #include <ESPAsyncWebServer.h>
#endif
#ifndef FAUXMOESP_H
  #define FAUXMOESP_H
  #include "fauxmoESP.h"
#endif

#ifndef H_MACROS
  #define H_MACROS
  #include "macros.h"
#endif


// #ifndef RESET
//   #define RESET          1
// #endif
// #ifndef NON_PATINING
//   #define NON_PATINING      0
// #endif
extern struct DeviceStruct devwifieeprom;
extern ds1307rtc rtc;

class PushButton{
  private:
    uint8_t _gpio;
    uint8_t _IOmode; // OUTPUT OR INPUT
    bool _pull; // Pin is pulled-up(true) or pulld-down(false)
    // button pressed counter
    volatile uint8_t _bpc;
    //volatile unsigned long lastmillis;
  public:
    PushButton(uint8_t gpio, uint8_t IOmode, uint8_t pull = true){_gpio = gpio; _IOmode = IOmode;_pull = pull;_bpc=0;}// default pulled up
    bool isPressed(){ return ( (_pull && (digitalRead(_gpio) == 0)) || (!_pull && (digitalRead(_gpio) == 1)) ) ? true : false;}
    // button pressed counter; zero if button is released
    void inc_bpc(){_bpc = (_bpc >= 255) ? 0 : ++_bpc;}
    uint8_t get_bpc() const {return _bpc;}
    void clr_bpc(){_bpc=0;}
};
// bool PushButton::isPressed(){
  
//   if( (_pull && (digitalRead(_gpio) == 0)) || (!_pull && (digitalRead(_gpio) == 1)) ){
//     _bpc = (_bpc >= 255) ? 0 : ++_bpc;
//       return true;
//   }
//   else
//     return false;
  
//   // if(_pull)
//   //   if(digitalRead(_gpio) == 0){
//   //     _bpc = (_bpc >= 255) ? 0 : ++_bpc;
//   //     return true;
//   //   }else
//   //     return false;
//   // else
//   //   if(digitalRead(_gpio) == 1){
//   //     _bpc = (_bpc >= 255) ? 0 : ++_bpc;
//   //     return true;
//   //   }else
//   //     return false;
// }
class espDevices{
  private:
    void _D7GPIO13ButtonLoop();
  public:
    uint8_t *mcumode; // == //ALEXA_MODE, SCHEDULER_MODE or ONOF_MODE
    uint8_t nonalexa_counter; // it will count to zero from 255 to check internet connection and update clock.
    int D7resetpin; volatile unsigned long lastD7millis; volatile unsigned long clocksyncmillis;
    uint8_t pair;
    Device D_3, D_4, D_5, D_6;
    PushButton D7;
    espDevices(): D_3(D3_GPIO0),D_4(D4_GPIO2),D_5(D5_GPIO14),D_6(D6_GPIO12),D7(D7_GPIO13,INPUT){nonalexa_counter = NONALEXA_COUNTER; D7resetpin = 0;lastD7millis = 0;clocksyncmillis = 0;pair=0;}
    //void setup();
    void setup();
    //void init(DeviceStruct& conf);
    //void Loop(ds1307rtc & rtc, DeviceStruct& conf,bool IslocalServer, bool Isfauxmo);
    void Loop();
    void Reset(){D_3.reset(); D_4.reset(); D_5.reset(); D_6.reset();}
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

    void Print();
    void Print_status();
};