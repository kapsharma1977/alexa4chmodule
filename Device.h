#pragma once         // Tells the compiler: "Only include this file once"

#include "macros.h"
#include "devicestruct.h"
#include "mcupin.h"

class Device : public mcuPin {
  // private:
  //   uint8_t _gpio;
  //   uint8_t _IOmode;
  public:
    //volatile bool onof; // bool &presentState; esp8266 Pin present state
    volatile bool newonof; // next new state of onof, that is to be set; input given by alexa or manual by user or scheduler 
    char      *_name; // Alexa name
    unsigned char *_deviceid;
    uint8_t   *_week;
    uint8_t   *_nh; // on Hour sunset trigger point 20:30
    uint8_t   *_nm; // on minute
    uint8_t   *_fh; // off Hour sunrise trigger point e.g. 07:30
    uint8_t   *_fm; // off minute
    bool      *_enable; // local(non alexa) on off schedule is enable or not; // do noting; logic for onof is disabled; device is controlled manualy or by alexa
    bool      *_flip; // off in night and on in day
    
    Device(uint8_t gpio,uint8_t IOmode = OUTPUT) : mcuPin(gpio,false,OUTPUT,true) {newonof=false;_name=nullptr;_deviceid=nullptr;_enable=nullptr;} // // _gpio,external resistor false(internal resistor), IOmode = OUTPUT, true(pull-up))
    void setup(DeviceStruct& conf);
    //void init(DeviceStruct& conf);
        
    //bool On(); // update pin and onof member based on onof
    //bool Off(); // update pin and onof member based on onof
    //bool trigared();
    //bool _midnighttosunrise(ds1307rtc & rtc);
    bool midnighttosunrise(uint8_t h, uint8_t m);
    bool eveningtomidnight(uint8_t h, uint8_t m);
    bool isdaytime(uint8_t h, uint8_t m);
    // bool is_bulbturn_day(ds1307rtc & rtc);
    // void setDay(uint8_t set){*_week = *_week | set;}// to set Monday setDay(MON); to unset Monday
    // void unsetDay(uint8_t unset){*_week = *_week & unset;}// to unset Monday setDay(UNMON);
    bool isday(uint8_t day){return *_week & day ? 1 : 0;} //to check isDay(MON)
    // bool isweekofday(uint8_t weekofday);
    // bool ONOFF(ds1307rtc & rtc);  //, uint8_t p, bool &presentState)
    //void loop(ds1307rtc & rtc); // flip is implemented 
    void loop(){ if(_active != newonof) (newonof) ? On() : Off();}
    String print();
    void reset(String devname = DEVICENAME);

    String getName();
    uint8_t getgpio(){return _gpio;}
    
    void setName(String name);
    bool setEnable(bool enable_){ return *_enable = (*_enable != enable_) ? enable_ : *_enable;}
    void setFlip(bool flip_){*_flip = (*_flip != flip_) ? flip_ : *_flip;}
    bool IsEnable(){return (_enable != nullptr) ? *_enable : false;}
    bool IsFlip(){return (_flip != NULL) ? *_flip : false;}
    
    uint8_t Week(){return (_week != NULL) ? *_week : ALLDAYS;}
    uint8_t getOnHour(){return (_nh != NULL) ? *_nh : ON_HOUR;}
    uint8_t getOnMinute(){ return (_nm != NULL) ? *_nm : ON_MINUTE;}
    uint8_t getOffHour(){return (_fh != NULL) ? *_fh : OFF_HOUR;}
    uint8_t getOffMinute(){return (_fm != NULL) ? *_fm : OFF_MINUTE;}
    
    void setWeek(uint8_t week){if(_week != NULL){*_week = week;}}
    void setOnHour(uint8_t nh){if(_nh != NULL){*_nh = (nh > 0 && nh < 24) ? nh : *_nh;}}
    void setOnMinute(uint8_t nm){if(_nm != NULL){*_nm = (nm > 0 && nm < 60) ? nm : *_nm;}}
    void setOffHour(uint8_t fh){if(_fh != NULL){*_fh = (fh > 0 && fh < 24) ? fh : *_fh;}}
    void setOffMinute(uint8_t fm){if(_fm != NULL){*_fm = (fm > 0 && fm < 60) ? fm : *_fm;}}
};

// class Device{
//   private:
//     uint8_t _gpio;
//     uint8_t _IOmode;
//   public:
//     volatile bool onof; // bool &presentState; esp8266 Pin present state
//     volatile bool newonof; // next new state of onof, that is to be set; input given by alexa or manual by user or scheduler 
//     char      *_name; // Alexa name
//     unsigned char *_deviceid;
//     uint8_t   *_week;
//     uint8_t   *_nh; // on Hour sunset trigger point 20:30
//     uint8_t   *_nm; // on minute
//     uint8_t   *_fh; // off Hour sunrise trigger point e.g. 07:30
//     uint8_t   *_fm; // off minute
//     bool      *_enable; // local(non alexa) on off schedule is enable or not; // do noting; logic for onof is disabled; device is controlled manualy or by alexa
//     bool      *_flip; // off in night and on in day
//     //Device(uint8_t gpio){_gpio = gpio;onof = false;newonof=false;_name=NULL;_week=NULL;_nh=NULL;_nm=NULL;_fh=NULL;_fm=NULL;_enable=NULL;_flip=NULL;}
//     Device(uint8_t gpio,uint8_t IOmode = OUTPUT){_gpio = gpio;_IOmode=IOmode;onof = false;newonof=false;_name=nullptr;_deviceid=nullptr;_enable=nullptr;}
//     void setup(DeviceStruct& conf);
//     //void init(DeviceStruct& conf);
//     void SetPintoOutMode(){pinMode(_gpio, OUTPUT);}
//     void SetPintoINPUTMode(){pinMode(_gpio, INPUT);}
    
//     bool On(); // update pin and onof member based on onof
//     bool Off(); // update pin and onof member based on onof
//     //bool trigared();
//     //bool _midnighttosunrise(ds1307rtc & rtc);
//     bool midnighttosunrise(uint8_t h, uint8_t m);
//     bool eveningtomidnight(uint8_t h, uint8_t m);
//     bool isdaytime(uint8_t h, uint8_t m);
//     // bool is_bulbturn_day(ds1307rtc & rtc);
//     // void setDay(uint8_t set){*_week = *_week | set;}// to set Monday setDay(MON); to unset Monday
//     // void unsetDay(uint8_t unset){*_week = *_week & unset;}// to unset Monday setDay(UNMON);
//     bool isday(uint8_t day){return *_week & day ? 1 : 0;} //to check isDay(MON)
//     // bool isweekofday(uint8_t weekofday);
//     // bool ONOFF(ds1307rtc & rtc);  //, uint8_t p, bool &presentState)
//     //void loop(ds1307rtc & rtc); // flip is implemented 
//     void loop(){ (onof != newonof) ? (newonof)? On() : Off() : 0;}
//     String print();
//     void reset(String devname = DEVICENAME);

//     String getName();
//     uint8_t getgpio(){return _gpio;}
    
//     void setName(String name);
//     bool setEnable(bool enable_){ return *_enable = (*_enable != enable_) ? enable_ : *_enable;}
//     void setFlip(bool flip_){*_flip = (*_flip != flip_) ? flip_ : *_flip;}
//     bool IsEnable(){return (_enable != nullptr) ? *_enable : false;}
//     bool IsFlip(){return (_flip != NULL) ? *_flip : false;}
    
//     uint8_t Week(){return (_week != NULL) ? *_week : ALLDAYS;}
//     uint8_t getOnHour(){return (_nh != NULL) ? *_nh : ON_HOUR;}
//     uint8_t getOnMinute(){ return (_nm != NULL) ? *_nm : ON_MINUTE;}
//     uint8_t getOffHour(){return (_fh != NULL) ? *_fh : OFF_HOUR;}
//     uint8_t getOffMinute(){return (_fm != NULL) ? *_fm : OFF_MINUTE;}
    
//     void setWeek(uint8_t week){if(_week != NULL){*_week = week;}}
//     void setOnHour(uint8_t nh){if(_nh != NULL){*_nh = (nh > 0 && nh < 24) ? nh : *_nh;}}
//     void setOnMinute(uint8_t nm){if(_nm != NULL){*_nm = (nm > 0 && nm < 60) ? nm : *_nm;}}
//     void setOffHour(uint8_t fh){if(_fh != NULL){*_fh = (fh > 0 && fh < 24) ? fh : *_fh;}}
//     void setOffMinute(uint8_t fm){if(_fm != NULL){*_fm = (fm > 0 && fm < 60) ? fm : *_fm;}}
// };