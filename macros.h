#pragma once         // Tells the compiler: "Only include this file once"

//#define DEBUG 1 // Comment this line to turn off all debug prints

#ifndef EXSITING_PCB
  #define EXSITING_PCB 1
#endif

#if DEBUG
  #ifndef DEBUG_FAUXMO
    #define DEBUG_FAUXMO Serial
  #endif
  #ifndef DEBUG_FAUXMO_VERBOSE_TCP
    #define DEBUG_FAUXMO_VERBOSE_TCP  true
  #endif
  #ifndef DEBUG_FAUXMO_VERBOSE_UDP
    #define DEBUG_FAUXMO_VERBOSE_UDP  true
  #endif
#else
  #ifndef DEBUG_FAUXMO_VERBOSE_TCP
    #define DEBUG_FAUXMO_VERBOSE_TCP    false
  #endif
  #ifndef DEBUG_FAUXMO_VERBOSE_UDP
    #define DEBUG_FAUXMO_VERBOSE_UDP    false
  #endif
#endif
#include <Arduino.h>
#if defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESPAsyncTCP.h>
#elif defined(ESP32)
    #include <WiFi.h>
    #include <AsyncTCP.h>
#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
    #include <AsyncTCP_RP2040W.h>
#else
	#error Platform not supported
#endif

#ifndef EEPROM_SIZE_T
#define EEPROM_SIZE_T 512
#endif

#ifndef COMPANY
  #define COMPANY "DeziWebApp"
#endif

#ifndef DEVICENAME
  #define DEVICENAME "Light"
#endif

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
#ifndef D8_GPIO15
  #define D8_GPIO15           15
#endif

#define PrintPin(gpio_) \
  (gpio_ == 0) ? DBGPRINT("D3_GPIO0") \
        : (gpio_ == 2) ? DBGPRINT("D4_GPIO2") \
        : (gpio_ == 14) ? DBGPRINT("D5_GPIO14") \
        : (gpio_ == 12) ? DBGPRINT("D6_GPIO12") \
        : (gpio_ == 13) ? DBGPRINT("D7_GPIO13") \
        : (gpio_ == 15) ? DBGPRINT("D8_GPIO15") \
        : DBGPRINT("Un Known Pin")

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

// // #define UNMON               0xFE //254 bitwise &; 11111110
// // #define UNTUS               0xFD // 253 11111101
// // #define UNWED               0xFB // 11111011
// // #define UNTHU               0xF7 // 11110111
// // #define UNFRI               0xEF // 11101111
// // #define UNSAT               0xDF // 11011111
// // #define UNSUN               0x8F //10111111
///////////#define UNDAYS              0x7F //01111111

// Define strings once in Flash
