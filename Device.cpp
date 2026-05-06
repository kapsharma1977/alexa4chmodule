#include "Device.h"

void Device::setup(DeviceStruct &conf){
  // To activate the external device, D3_GPIO0 and D4_GPIO2 must be driven LOW(Active LOW); and GPIO15 must be drive HIGH (Active HIGH)
  mcuPin::setup();
  #if DEBUG
  mcuPin::print();
  #endif
  if(_gpio == D3_GPIO0){
    DBGPRINT("MCU Device 1 initalized at GPIO: ");
    DBGPRINTLN(_gpio);
    this->_name = conf.D3name;
    this->_enable = &conf.D3enable;
    this->_deviceid = &conf.D3device_id;
    this->_week = &conf.D3week;
    this->_nh = &conf.D3nh;
    this->_nm = &conf.D3nm;
    this->_fh = &conf.D3fh;
    this->_fm = &conf.D3fm;
    this->_flip = &conf.D3flip;
  }
  if(_gpio == D4_GPIO2){
    DBGPRINT("MCU Device 2 initalized at GPIO: ");
    DBGPRINTLN(_gpio);
    this->_name = conf.D4name;
    this->_enable = &conf.D4enable;
    this->_deviceid = &conf.D4device_id;
    this->_week = &conf.D4week;
    this->_nh = &conf.D4nh;
    this->_nm = &conf.D4nm;
    this->_fh = &conf.D4fh;
    this->_fm = &conf.D4fm;
    this->_flip = &conf.D4flip;
    
  }
  if(_gpio == D5_GPIO14){
    DBGPRINT("MCU Device 3 initalized at GPIO: ");
    DBGPRINTLN(_gpio);
    this->_name = conf.D5name;
    this->_enable = &conf.D5enable;
    this->_deviceid = &conf.D5device_id;
    this->_week = &conf.D5week;
    this->_nh = &conf.D5nh;
    this->_nm = &conf.D5nm;
    this->_fh = &conf.D5fh;
    this->_fm = &conf.D5fm;
    this->_flip = &conf.D5flip;
  }
  if(_gpio == D6_GPIO12){
    DBGPRINT("MCU Device 4 initalized at GPIO: ");
    DBGPRINTLN(_gpio);
    this->_name = conf.D6name;
    this->_enable = &conf.D6enable;
    this->_deviceid = &conf.D6device_id;
    this->_week = &conf.D6week;
    this->_nh = &conf.D6nh;
    this->_nm = &conf.D6nm;
    this->_fh = &conf.D6fh;
    this->_fm = &conf.D6fm;
    this->_flip = &conf.D6flip;
  }
}
// bool Device::On(){
//   if(!onof){
//     onof = true;
//     digitalWrite(_gpio, LOW);
//     #if DEBUG
//     DBG_F("digitalWrite(_gpio, HIGH); _gpio : %d Device Name: %s. Status: %d ; enable : %d\n",_gpio, getName(),onof, IsEnable());
//     #endif
//   }
//   return onof;
// }
// bool Device::Off(){
//   if(onof){
//     onof = false;
//     digitalWrite(_gpio, HIGH);
//     #if DEBUG
//     DBG_F("digitalWrite(_gpio, HIGH); _gpio : %d Device Name: %s. Status: %d ; enable : %d\n",_gpio, getName(),onof, IsEnable());
//     #endif
//   }
//   return onof;
// }
// trigger 1
bool Device::midnighttosunrise(uint8_t h, uint8_t m){ // 00:00H to *_fh:*_fm(7:30)
  //uint8_t h = rtc.RTC.now().hour();
  //uint8_t m = rtc.RTC.now().minute();
  if(h <= *_fh)
    if(h == *_fh)
      if(m < *_fm)
        return true;
      else
        return false;
    else
      return true;
  else
    return false;
}
// // trigger 2
bool Device::eveningtomidnight(uint8_t h, uint8_t m){ // *_nh:*_nm(20:30) to 00:00H
  //uint8_t h = rtc.RTC.now().hour();
  //uint8_t m = rtc.RTC.now().minute();
  if(h >= *_nh)
    if(h == *_nh)
      if(m > *_nm)
        return true;
      else
        return false;
    else
      return true;
  else
    return false;
}
bool Device::isdaytime(uint8_t h, uint8_t m){ // Time between two triggers (trigger 1 and trigger 2) *_fh:*_fm(7:30) to *_nh:*_nm(20:30)
  if( (h >= *_fh) && (h <= *_nh) ){ //*_fh(7) to *_nh(20)
    
    if(h == *_fh) // h == *_fh(7)
      if(m > *_fm)
        return true;
      else
        return false;
    //else //      return true;

    if(h == *_nh) // h == *_nh(20)
      if(m < *_nm)
        return true;
      else
        return false;
    
    return true; // (h > *_fh) && (h < *_nh)
  }else
    return false;
}
String Device::getName(){
  if(_name == nullptr)
    return "";
  char name[LEN];
  if(_name != nullptr)
    for(uint8_t i = 0; i < LEN; i++)
      if(_name[i] == '\0'){
        name[i] = '\0';
        break;
      }
      else
        name[i] = _name[i];
  return String(name);
}
void Device::setName(String name){
  if(_name == nullptr)
    return;
  uint8_t len = name.length();
  if(len < LEN - 1 && len > 0)
    for(uint8_t i = 0; i < LEN; i++){
      if(i >= len)
        _name[i] = '\0';
      else
        _name[i] = name.charAt(i);
    }
}
String Device::print(){
  //MTWTFSS
  char s[28];
  uint8_t i = 0;

  if(isday(MON)){s[i] = 'M';i++;}
  if(isday(TUS)){s[i] = 'T';i++;}
  if(isday(WED)){s[i] = 'W';i++;}
  if(isday(THU)){s[i] = 'T';i++;}
  if(isday(FRI)){s[i] = 'F';i++;}
  if(isday(SAT)){s[i] = 'S';i++;}
  if(isday(SUN)){s[i] = 'S';i++;}
  sprintf(&s[i],"%c",':');
  i++;
  int c; // no. of char printed in s
  if(*_nh > 9)
    c = sprintf(&s[i],"%2i:",*_nh);
  else
    c = sprintf(&s[i],"0%i:",*_nh);
  if(c <= 255 || c >= 0)
      i = i + (uint8_t)c;
  
  if(*_nm > 9)
    c = sprintf(&s[i],"%2i:",*_nm);
  else
    c = sprintf(&s[i],"0%i:",*_nm);
  if(c <= 255 || c >= 0)
      i = i + (uint8_t)c;
  
  if(*_fh > 9)
    c = sprintf(&s[i],"%2i:",*_fh);
  else
    c = sprintf(&s[i],"0%i:",*_fh);
  if(c <= 255 || c >= 0)
      i = i + (uint8_t)c;
  
  if(*_fm > 9)
    c = sprintf(&s[i],"%2i:",*_fm);
  else
    c = sprintf(&s[i],"0%i:",*_fm);
  if(c <= 255 || c >= 0)
      i = i + (uint8_t)c;

  sprintf(&s[i],"%1d",*_enable);
  i = i + 1;
  s[i] = '\0';

  char dno[3]; dno[2] = '\0';
  bool ps;
  if(_gpio == D3_GPIO0){dno[0]='1';dno[1]='.';}
  if(_gpio == D4_GPIO2){dno[0]='2';dno[1]='.';}
  if(_gpio == D5_GPIO14){dno[0]='3';dno[1]='.';}
  if(_gpio == D6_GPIO12){dno[0]='4';dno[1]='.';}
  
  //DBG_F("Device Name %s. : %s Status: %d :Schedule: %s",dno,getName(),this->onof,s);
  
  return String(dno) + getName() + "::" + String(s);
}
void Device::reset(String devname){
  uint8_t len = devname.length();
  if(len < LEN - 2)// because we are adding one char '1'/'2'/'3'/'4' to name
    for(uint8_t i = 0; i < LEN; i++)
      if(i == len){
        if(_gpio == D3_GPIO0){_name[i] = 1;}
        if(_gpio == D4_GPIO2){_name[i] = 2;}
        if(_gpio == D5_GPIO14){_name[i] = 3;}
        if(_gpio == D6_GPIO12){_name[i] = 4;}
      }
      else
        _name[i] = (i > len) ? '\0' : devname.charAt(i);
        
  *_enable = true; //               01 bytes  local(non alexa) on off schedule is enable or not
}