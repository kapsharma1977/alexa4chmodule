// https://github.com/jwrw/ESP_EEPROM
#ifndef H_ARDUINO
#define H_ARDUINO
  #include <Arduino.h>
#endif
#ifndef H_MACROS
  #define H_MACROS
  #include "macros.h"
#endif
#ifndef H_ESP_EEPROM
#define H_ESP_EEPROM
  #include <ESP_EEPROM.h>
#endif
#ifndef H_ENCRYP
  #define H_ENCRYP
  #include "encryp.h"
#endif

struct DeviceStruct {
  char  ssid[LEN]; // encrypted       16 bytes
  char  pass[LEN]; // encrypted       16 bytes
  // D_3 Device
  char  D3name[LEN]; // Alexa name    16 bytes
  uint8_t   D3week;//                 01 bytes
  unsigned char D3device_id;
  uint8_t   D3nh; // on Hour          01 bytes
  uint8_t   D3nm; // on minute        01 bytes
  uint8_t   D3fh; // off Hour         01 bytes
  uint8_t   D3fm; // off minute       01 bytes
  bool D3enable; //                   01 bytes  local(non alexa) on off schedule is enable or not
  bool D3flip; // off in night        01 bytes
  // D_4 Device
  char  D4name[LEN]; // Alexa name    16 bytes
  uint8_t   D4week;//                 01 bytes
  unsigned char D4device_id;
  uint8_t   D4nh; // on Hour          01 bytes
  uint8_t   D4nm; // on minute        01 bytes
  uint8_t   D4fh; // off Hour         01 bytes
  uint8_t   D4fm; // off minute       01 bytes
  bool D4enable; //                   01 bytes  local(non alexa) on off schedule is enable or not
  bool D4flip; // off in night        01 bytes
  // D_5 Device
  char  D5name[LEN]; // Alexa name    16 bytes
  uint8_t   D5week;//                 01 bytes
  unsigned char D5device_id;
  uint8_t   D5nh; // on Hour          01 bytes
  uint8_t   D5nm; // on minute        01 bytes
  uint8_t   D5fh; // off Hour         01 bytes
  uint8_t   D5fm; // off minute       01 bytes
  bool D5enable; //                   01 bytes  local(non alexa) on off schedule is enable or not
  bool D5flip; // off in night        01 bytes
  // D_6 Device
  char  D6name[LEN]; // Alexa name    16 bytes
  uint8_t   D6week;//                 01 bytes
  unsigned char D6device_id;
  uint8_t   D6nh; // on Hour          01 bytes
  uint8_t   D6nm; // on minute        01 bytes
  uint8_t   D6fh; // off Hour         01 bytes
  uint8_t   D6fm; // off minute       01 bytes
  bool D6enable; //                   01 bytes  local(non alexa) on off schedule is enable or not
  bool D6flip; // off in night        01 bytes
  char  APssid[LEN]; // softAP SSID
  char  APpass[LEN]; // softAP password
  char  key[LEN]; // MCU is booted first time in life if no key is found in memory.
  uint8_t fo1;//  unit8_t first_octet  01 bytes
  uint8_t so2;//                       01 bytes
  uint8_t to3;//                       01 bytes
  uint8_t fo4;//                       01 bytes
  // MCU Modes
  uint8_t mcumode; // == //ALEXA_MODE or ONOF_MODE
}; // 16 + 16 + 23 + 23 + 23 + 23 + 16 + 16 +16 + 4 = 32 + 92 + 52= 176 Bytes


// struct DeviceStruct {
//   char  ssid[LEN]; // encrypted       16 bytes
//   char  pass[LEN]; // encrypted       16 bytes
//   // D_3 Device
//   char  D3name[LEN]; // Alexa name    16 bytes
//   unsigned char D3device_id;//        01
//   bool D3enable; //                   01 bytes  Disabled if Zero
//   // D_4 Device
//   char  D4name[LEN]; // Alexa name    16 bytes
//   unsigned char D4device_id;//        01
//   bool D4enable; //                   01 bytes  Disabled if Zero
//   // D_5 Device
//   char  D5name[LEN]; // Alexa name    16 bytes
//   unsigned char D5device_id;//        01
//   bool D5enable; //                   01 bytes  Disabled if Zero
//   bool D5flip; // off in night        01 bytes
//   // D_6 Device
//   char  D6name[LEN]; // Alexa name    16 bytes
//   unsigned char D6device_id;//        01
//   bool D6enable; //                   01 bytes  Disabled if Zero
//   char  APssid[LEN]; // softAP SSID   16
//   char  APpass[LEN]; // softAP password 16
//   char  key[LEN]; // MCU is booted first time in life if no key is found in memory.
//   uint8_t fo1;//  unit8_t first_octet  01 bytes
//   uint8_t so2;//                       01 bytes
//   uint8_t to3;//                       01 bytes
//   uint8_t fo4;//                       01 bytes
//   // MCU Modes
//   uint8_t mcumode; // == //ALEXA_MODE or ONOF_MODE 01
// }; // 16 + 16 + 16 + 01 + 16 + 01 + 16 + 01 + 01 + 16 + 01 + 16 + 16 + 05 =  (16*8)128 + 14 = 142 Bytes

void wipe_eeprom();
void read_eeprom(DeviceStruct &conf);// read flash memory as esp8266 do not have EEPROM instead it uses flash memory as EEPROM(Emulated)
void write_eeprom(DeviceStruct &conf);
void default_wifi(DeviceStruct &conf,String company = COMPANY);
void default_DeviceStruct(DeviceStruct &conf,String company = COMPANY,String dev = DEVICENAME);
void ResetEEPROM(DeviceStruct &conf);
void print_DeviceStruct(DeviceStruct &conf);
void changeDeviceStruct_CarrayLEN(char* DeviceStructarray, String& new_);// (DeviceStruct.ssid, "ffhgh")