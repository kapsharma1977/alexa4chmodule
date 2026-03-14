#ifndef H_MACROS
  #define H_MACROS
  #include "macros.h"
#endif

#ifndef H_ARDUINO
  #define H_ARDUINO
  #include <Arduino.h>
#endif

#ifndef H_DEVICESTUCT
  #define H_DEVICESTUCT
  #include "devicestruct.h"
#endif

// #define MON                 0x01 //1;   00000001
// #define TUS                 0x02 //2;   00000010
// #define WED                 0x04 //4;   00000100
// #define THU                 0x08 //8;   00001000
// #define FRI                 0x10 //16;  00010000
// #define SAT                 0x20 //32;  00100000
// #define SUN                 0x40 //64;  01000000

// #ifndef ALLDAYS
//   #define ALLDAYS           0x7F //01111111     //////0x80 //128; 10000000
// #endif
// // #define UNMON               0xFE //254 bitwise &; 11111110
// // #define UNTUS               0xFD // 253 11111101
// // #define UNWED               0xFB // 11111011
// // #define UNTHU               0xF7 // 11110111
// // #define UNFRI               0xEF // 11101111
// // #define UNSAT               0xDF // 11011111
// // #define UNSUN               0x8F //10111111
///////////#define UNDAYS              0x7F //01111111



class Device{
  private:
    uint8_t _gpio;
    uint8_t _IOmode;
  public:
    volatile bool onof; // bool &presentState; esp8266 Pin present state
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
    //Device(uint8_t gpio){_gpio = gpio;onof = false;newonof=false;_name=NULL;_week=NULL;_nh=NULL;_nm=NULL;_fh=NULL;_fm=NULL;_enable=NULL;_flip=NULL;}
    Device(uint8_t gpio,uint8_t IOmode = OUTPUT){_gpio = gpio;_IOmode=IOmode;onof = false;newonof=false;_name=nullptr;_deviceid=nullptr;_enable=nullptr;}
    void setup(DeviceStruct& conf);
    //void init(DeviceStruct& conf);
    void SetPintoOutMode(){pinMode(_gpio, OUTPUT);}
    void SetPintoINPUTMode(){pinMode(_gpio, INPUT);}
    
    bool On(); // update pin and onof member based on onof
    bool Off(); // update pin and onof member based on onof
    bool trigared();
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
    void loop(){ (onof != newonof) ? (newonof)? On() : Off() : 0;}
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