#include "devicestruct.h"

void wipe_eeprom(){
  // It is still necessary to call begin
  // or no wipe will be performed
  EEPROM.begin(EEPROM_MIN_SIZE);

  boolean result = EEPROM.wipe();
  if (result)
    DBGPRINTLN("All EEPROM data wiped");
  else
    DBGPRINTLN("EEPROM data could not be wiped from flash store");
}
void read_eeprom(DeviceStruct &conf){
  // All the library functions are accessed via the EEPROM object created when
  // you include the library header ESP_EEPROM.h

  // The library needs to know what size you need for your EEPROM variables
  // Using a structure makes this easy.

  // The begin() call will find the data previously saved in EEPROM if the same size
  // as was previously committed. If the size is different then the EEEPROM data is cleared. 
  // Note that this is not made permanent until you call commit();
  EEPROM.begin(sizeof(DeviceStruct));
  // Check if the EEPROM contains valid data from another run
  // If so, overwrite the 'default' values set up in our struct

  if(EEPROM.percentUsed()>=0) {
    EEPROM.get(0, conf);
    delay(500);
    DBGPRINTLN("EEPROM has data from a previous run.");
    DBGPRINT(EEPROM.percentUsed());
    DBGPRINTLN("% of ESP flash space currently used");
  }
  else 
    DBGPRINTLN("EEPROM size changed - EEPROM data zeroed - commit() to make permanent");
}
void write_eeprom(DeviceStruct& conf){
  // All the library functions are accessed via the EEPROM object created when
  // you include the library header ESP_EEPROM.h

  // The library needs to know what size you need for your EEPROM variables
  // Using a structure makes this easy.

  // The begin() call is required to initialise the EEPROM library
  EEPROM.begin(sizeof(DeviceStruct));
  //
  // (some code that might change the EEPROM data)
  //

  // set the EEPROM data ready for writing
  EEPROM.put(0, conf);
  // write the data to EEPROM
  boolean ok = EEPROM.commit();
  delay(500);
  DBGPRINTLN((ok) ? "Commit OK" : "Commit failed");
}
void default_wifi(DeviceStruct& conf,String company){
  uint8_t len = company.length();
  if(len < LEN - 1)
    for(uint8_t i = 0; i < LEN; i++)
      if(i >= len){
        conf.ssid[i] = '\0';
        conf.pass[i] = '\0';
        conf.APssid[i] = '\0';
        conf.APpass[i] = '\0';
      }
      else{
        conf.ssid[i] = company.charAt(i);
        conf.pass[i] = company.charAt(i);
        conf.APssid[i] = company.charAt(i);
        conf.APpass[i] = company.charAt(i);
      }
}
void default_DeviceStruct(DeviceStruct &conf, String company,String dev){
  default_wifi(conf,company);
  uint8_t len = dev.length();
  if(len < LEN - 2)// because we are adding one char '1'/'2'/'3'/'4' to name
    for(uint8_t i = 0; i < LEN; i++)
      if(i == len){
        conf.D3name[i] = '1';
        conf.D4name[i] = '2';
        conf.D5name[i] = '3';
        conf.D6name[i] = '4';
      }else{
        if(i > len){
          conf.D3name[i] = '\0';
          conf.D4name[i] = '\0';
          conf.D5name[i] = '\0';
          conf.D6name[i] = '\0';
        }else{
          conf.D3name[i] = dev.charAt(i);
          conf.D4name[i] = dev.charAt(i);
          conf.D5name[i] = dev.charAt(i);
          conf.D6name[i] = dev.charAt(i);
        }
      }
  // D_3 Device
  conf.D3enable = true; //
  conf.D3device_id = (unsigned)'0';
  conf.D3week = ALLDAYS;
  conf.D3nh = ON_HOUR; // on Hour          01 bytes
  conf.D3nm = ON_MINUTE; // on minute        01 bytes
  conf.D3fh = OFF_HOUR;// off Hour         01 bytes
  conf.D3fm = OFF_MINUTE; // off minute       01 bytes
  conf.D3flip = false;
  // D_4 Device
  conf.D4enable = true; //                   01 bytes  local(non alexa) on off schedule is enable or not
  conf.D4device_id = (unsigned)'0';
  conf.D4week = ALLDAYS;
  conf.D4nh = ON_HOUR; // on Hour          01 bytes
  conf.D4nm = ON_MINUTE; // on minute        01 bytes
  conf.D4fh = OFF_HOUR;// off Hour         01 bytes
  conf.D4fm = OFF_MINUTE; // off minute       01 bytes
  // D_5 Device
  conf.D5enable = true; //                   01 bytes  local(non alexa) on off schedule is enable or not
  conf.D5device_id = (unsigned)'0';
  conf.D5week = ALLDAYS;
  conf.D5nh = ON_HOUR; // on Hour          01 bytes
  conf.D5nm = ON_MINUTE; // on minute        01 bytes
  conf.D5fh = OFF_HOUR;// off Hour         01 bytes
  conf.D5fm = OFF_MINUTE; // off minute       01 bytes
  // D_6 Device
  conf.D6enable = true; //                   01 bytes  local(non alexa) on off schedule is enable or not
  conf.D6device_id = (unsigned)'0';
  conf.D6week = ALLDAYS;
  conf.D6nh = ON_HOUR; // on Hour          01 bytes
  conf.D6nm = ON_MINUTE; // on minute        01 bytes
  conf.D6fh = OFF_HOUR;// off Hour         01 bytes
  conf.D6fm = OFF_MINUTE; // off minute       01 bytes
  // save key in memory. // MCU is booted first time in life if no key is found in memory.
  // IsKey() && i > 0 && i < LEN
  encryp _cryp;
  if(_cryp.IsKey())
    for(uint8_t i = 0; i < LEN; i++)
      conf.key[i] = _cryp.charAt(i);
  else
    for(uint8_t i = 0; i < LEN; i++)
      conf.key[i] = '\0';
  // SoftAP IP address
  conf.fo1 = FIRST_OCTET;
  conf.so2 = SECOND_OCTET;
  conf.to3 = THIRD_OCTET;
  conf.fo4 = FOURTH_OCTET;
  //MCU Mode
  conf.mcumode = CONFIG_MODE; //ALEXA_MODE
}
void ResetEEPROM(DeviceStruct &conf){
  wipe_eeprom();
  delay(500);
  default_DeviceStruct(conf);
  write_eeprom(conf);
  delay(500);
}
void print_DeviceStruct(DeviceStruct &conf){
  DBGPRINT("Wifi SSID = ");
  DBGPRINTLN(String(conf.ssid));
  DBGPRINT("Wifi PASS = ");
  DBGPRINTLN(conf.pass);
  DBGPRINT("Wifi Device 1. Name = ");
  DBGPRINTLN(conf.D3name);
  DBGPRINT("Wifi Device 2. Name = ");
  DBGPRINTLN(conf.D4name);
  DBGPRINT("Wifi Device 3. Name = ");
  DBGPRINTLN(conf.D5name);
  DBGPRINT("Wifi Device 4. Name = ");
  DBGPRINTLN(conf.D6name);
}
void changeDeviceStruct_CarrayLEN(char* DeviceStructarray, String& new_){// (DeviceStruct.ssid, "ffhgh")
  uint8_t len = new_.length();
  if(len < LEN - 1 & len > 0)
    for(uint8_t i = 0; i < LEN; i++)
      DeviceStructarray[i] = (i >= len) ? '\0' : new_.charAt(i);
}
