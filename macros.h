#ifndef H_ARDUINO
  #define H_ARDUINO
  #include <Arduino.h>
#endif

#ifdef ESP32
  #include <WiFi.h>
#else
  #ifndef H_ESP8266WiFi
    #define H_ESP8266WiFi
    #include <ESP8266WiFi.h>
  #endif
#endif

#ifndef COMPANY
  #define COMPANY "DeziWebApp"
#endif

#ifndef DEVICENAME
  #define DEVICENAME "Light"
#endif

#ifndef DEBUG
   #define DEBUG 0
   #ifndef DEBUG_FAUXMO_VERBOSE_TCP
    #define DEBUG_FAUXMO_VERBOSE_TCP  true
  #endif
  #ifndef DEBUG_FAUXMO_VERBOSE_UDP
    #define DEBUG_FAUXMO_VERBOSE_UDP  true
  #endif
 #endif

// #if DEBUG == 1
//   #define DEBUG_FAUXMO_VERBOSE_TCP  true
//   #define DEBUG_FAUXMO_VERBOSE_UDP  true
// #else
//   #define DEBUG_FAUXMO_VERBOSE_TCP  false
//   #define DEBUG_FAUXMO_VERBOSE_UDP  false
// #endif

#if DEBUG
  #define DBGPRINTLN(...) Serial.println(__VA_ARGS__)
  #define DBGPRINT(...)   Serial.print(__VA_ARGS__)
  #define DBG_F(...)      Serial.printf(__VA_ARGS__)
#else
  // In Release mode, these do absolutely nothing and use 0 bytes of RAM
  #define DBGPRINTLN(...) ((void)0)
  #define DBGPRINT(...)   ((void)0)
  #define DBG_F(...)      Serial.printf(__VA_ARGS__)
#endif


// #define DBG(...) DEBUG && Serial.printf(__VA_ARGS__)
// #define DBGPRINT(...) DEBUG && Serial.print(__VA_ARGS__)
// #define DBGPRINTLN(...) DEBUG && Serial.println(__VA_ARGS__)

#define DEBUG_FAUXMO  Serial
#define SERIAL_BAUDRATE 115200

#ifndef ALEXA_MODE
#define ALEXA_MODE  1
#endif
#ifndef CONFIG_MODE
#define CONFIG_MODE 4
#endif

#ifndef LEN
  #define LEN 16
#endif

#ifndef FIRST_OCTET
  #define   FIRST_OCTET    192
#endif
#ifndef SECOND_OCTET
  #define  SECOND_OCTET    168
#endif
#ifndef THIRD_OCTET
  #define   THIRD_OCTET     4
#endif
#ifndef FOURTH_OCTET
  #define   FOURTH_OCTET    1
#endif

// pins sutable for output only
#ifndef D3_GPIO0
  #define D3_GPIO0            0 //use in flash memory as esp8266 do not have EEPROM instead it uses flash memory as EEPROM
#endif
#ifndef D4_GPIO2
  #define D4_GPIO2            2
#endif
// pins sutable all purpose
#ifndef D5_GPIO14
  #define D5_GPIO14           14
#endif
#ifndef D6_GPIO12
  #define D6_GPIO12           12
#endif
#ifndef D7_GPIO13
  #define D7_GPIO13           13  // used for digital input push button
#endif
//////#define D8_GPIO15           15


#ifndef WIFI_MODE_DELAY
  #define WIFI_MODE_DELAY   500
#endif
#ifndef ALEXA_DELAY_COUNT
  #define ALEXA_DELAY_COUNT 12
#endif
#ifndef TMO
  #define TMO               44
#endif
#ifndef LOOP_MILLIS
  #define LOOP_MILLIS       15000 // Milli seconds  15s
#endif
#ifndef NONALEXA_COUNTER // it will count to zero from 255 to check internet connection and update clock.
  #define NONALEXA_COUNTER  255 // can be set 128
#endif

// 24H hour format
#ifndef ON_HOUR
  #define   ON_HOUR         19 // 7PM
#endif
#ifndef ON_MINUTE
  #define   ON_MINUTE       30 // 30 Minutes
#endif
#ifndef OFF_HOUR
  #define   OFF_HOUR       7 // 7AM
#endif
#ifndef OFF_MINUTE
  #define   OFF_MINUTE     30 // 30 Minutes
#endif

#define MON                 0x01 //1;   00000001
#define TUS                 0x02 //2;   00000010
#define WED                 0x04 //4;   00000100
#define THU                 0x08 //8;   00001000
#define FRI                 0x10 //16;  00010000
#define SAT                 0x20 //32;  00100000
#define SUN                 0x40 //64;  01000000

#ifndef ALLDAYS
  #define ALLDAYS           0x7F //01111111     //////0x80 //128; 10000000
#endif