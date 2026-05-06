#pragma once
// This is Tiny RTC Real Time Clock DS1307 I2C IIC Module for Arduino. 
// It also has an Atmel 24C32 EEPROM chip. 56 Bytes of Non-volatile memory available to the user. 4KB of serial electrically erasable and programmable read-only memory (EEPROM)
#include "macros.h"
#include "RTClib.h"
#include "devicestruct.h"

extern struct DeviceStruct devwifieeprom;

#include <at24c32.h>
#define AT24C32_I2C_ADDRESS 0x50  // the I2C address of Tiny RTC AT24C32 EEPROM
#define DS1307_I2C_ADDRESS 0x68
#define HOURS_REGISTER 0x02 // To verify if your DS1307 is in 24-hour mode, you must check Bit 6 of register 02h (the Hours register); Bit 6 = 0: 24-hour mode is active

#include <Wire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define RTC_NO_DAY 255

class ds1307rtc{
  uint8_t uday; // day of ntp time from internet and adjusted RTC
  bool adjustntp;
  uint8_t ntpfailcount;
  // I2C devices adresses found in _IsI2C scan
  byte hexadd_clock, at24c32_hexadd;
  bool _IsI2C(); // MCU I2C must be able to communicate at 0x68 DS1307_I2C_ADDRESS to read/update time in DS1307; otherwise NO RTC
  
  
  AT24C32 DS1307AT24C32_eeprom; // Initialize the EEPROM object (32 = 32Kbits/4KBytes for 24C32)
  public:
    bool isRTC_DS1307;
    RTC_DS1307 RTC;

    ds1307rtc():DS1307AT24C32_eeprom(AT24C32_I2C_ADDRESS){adjustntp = true;ntpfailcount = RTC_NO_DAY;isRTC_DS1307 = false;hexadd_clock = 0; at24c32_hexadd = 0;}
    void initRTC();
    // bool IsUpdateRequired(){return (!_IsI2C()) ? false : (!RTC.isrunning()) ? false : ((uday != RTC.now().day()) || adjustntp) ? true : false;}
    bool IsUpdateRequired(){return ((adjustntp && (uday == RTC.now().day()))) ? false : (!_IsI2C()) ? false : (!RTC.isrunning()) ? false : true;}
    //bool IsRTC(){return (RTC.isrunning())isRTC_DS1307;}
    bool ntpadjustRTC(uint8_t attempts = 5);
    void checkandupdateRTC(bool connected = true, uint8_t attempts = 5);
    bool isDS1307(){return (hexadd_clock == DS1307_I2C_ADDRESS) ? true : false;}
    bool isAT24C32(){return (at24c32_hexadd == AT24C32_I2C_ADDRESS || at24c32_hexadd == 0x57) ? true : false;}
    
    
    String date(char seaprator = '/');
    String time(char seaprator = ':');
    void printRTC();
    void printIsUpdateRequired(){DBGPRINT("\tadjustntp=");DBGPRINT(adjustntp);(uday == RTC.now().day()) ? DBGPRINT("\tuday == RTC.now().day()") :DBGPRINT("\tuday != RTC.now().day()");}
    void PrintLine(){DateTime now = RTC.now(); DBG_F("%02d/%02d/%d - %02d:%02d:%02d\n",now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());}
    void saveDeviceStruct(){DS1307AT24C32_eeprom.put(0,devwifieeprom); delay(500);} // AT24C32
    void loadDeviceStruct(){DS1307AT24C32_eeprom.get(0, devwifieeprom);delay(500);}// AT24C32

    static String ddmmyyyy(uint8_t dd, uint8_t mm,uint16_t yyyy,char seaprator = '/');
    static String hhmmss(uint8_t hh, uint8_t mm, uint8_t ss, char seaprator = ':');
};
////////////////////////////////////////////////////////////////////
// https://forum.arduino.cc/t/ds1307-rtc-programming-2-using-the-56-nvsram-bytes/309494
// The DS1307 comes with 56 bytes of nvSRAM
// void Tiny_RTC_DS1307_24C32::ReadSRAMByte(byte bAddress, byte *pbValue)
// {
//   Wire.beginTransmission(DS1307_I2C_ADDRESS);
//   Wire.write(bAddress+8);
//   Wire.endTransmission();
//   uint8_t bRet = Wire.requestFrom(DS1307_I2C_ADDRESS, 1);
//   *pbValue = Wire.read();
// }

// void Tiny_RTC_DS1307_24C32::WriteSRAMByte(byte bAddress, byte bValue)
// {
//   Wire.beginTransmission(DS1307_I2C_ADDRESS);
//   Wire.write(bAddress+8);
//   Wire.write(bValue);
//   Wire.endTransmission();
// }
///////////////////////////////////////////////////////////////////
// https://forum.arduino.cc/t/at24c32-i2c-eeprom-as-found-on-tiny-rtc-ds1307-board/309537
// void Tiny_RTC_DS1307_24C32::EEPROM_write_byte( unsigned int uiAddress, byte bData ) 
// {
// 	int iCount=0;
// 	do
// 	{
// 		Wire.beginTransmission(AT24C32_I2C_ADDRESS);
// 		Wire.write((byte)(uiAddress >> 8)); // MSB
// 		Wire.write((byte)uiAddress); // LSB
// 		Wire.write(bData);
// 	} while (Wire.endTransmission()!=0 && ++iCount<10);
// }

// void Tiny_RTC_DS1307_24C32::EEPROM_read_byte( unsigned int uiAddress, byte *pbData ) 
// {
// 	int iCount=0;
// 	do
// 	{
// 		Wire.beginTransmission(AT24C32_I2C_ADDRESS);
// 		Wire.write((byte)(uiAddress >> 8)); // MSB
// 		Wire.write((byte)uiAddress); // LSB
// 	} while (Wire.endTransmission()!=0 && ++iCount<10);
// 	Wire.requestFrom(AT24C32_I2C_ADDRESS,1);
// 	*pbData = Wire.read(); 
// }