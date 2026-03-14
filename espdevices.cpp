
#ifndef H_ESPDEVICES
  #define H_ESPDEVICES
  #include "espdevices.h"
#endif

// void espDevices::setup(){
//   D_3.setup();
//   D_4.setup();
//   D_5.setup();
//   D_6.setup();
// }
void espDevices::setup(){
  DBGPRINTLN("espDevices::setup()");
  mcumode = &devwifieeprom.mcumode;
  D_3.setup(devwifieeprom);
  D_4.setup(devwifieeprom);
  D_5.setup(devwifieeprom);
  D_6.setup(devwifieeprom);
  pinMode(D7_GPIO13,INPUT_PULLUP); // push button; when button pushed Pin D7_GPIO13 connected to GND; short press and long press
}
// void espDevices::init(){
//   mcumode = &devwifieeprom.mcumode;
//   D_3.init(devwifieeprom);
//   D_4.init(devwifieeprom);
//   D_5.init(devwifieeprom);
//   D_6.init(devwifieeprom);
// }
void espDevices::_D7GPIO13ButtonLoop(){
  // check button status
  if(D7.isPressed()){ // button pressed
    D7.inc_bpc();
    DBGPRINT("D7.get_bpc : ");
    DBGPRINTLN(D7.get_bpc());
    //DBGPRINT(".");
  }
  else{ // button released
    
    if(D7.get_bpc() > 0 && D7.get_bpc() < 3){// push button very short press 
      // print report
      #if DEBUG
      Print();
      Print_status();
      print_DeviceStruct(devwifieeprom);
      // DBGPRINTLN(D_3.print());
      // DBGPRINTLN(D_4.print());
      // DBGPRINTLN(D_5.print());
      // DBGPRINTLN(D_6.print());
      #endif
    }

    if(D7.get_bpc() > 3 && D7.get_bpc() <= 9){// push button short press 
      if(IsAlexaEnable() && (WiFi.getMode() == WIFI_STA) && (WiFi.status() == WL_CONNECTED))
        pair = 50;
      DBGPRINT("Paring Mode ");
      DBGPRINTLN(pair);
    }
    
    if(D7.get_bpc() > 9){ // push button long press 
      default_DeviceStruct(devwifieeprom);
      EnableConfig();
      if(rtc.isAT24C32()) // Get EEPROM AT24C32 data from DS1307 into our local copy RAM
        rtc.saveDeviceStruct(); // the EEPROM AT24C32 (32 = 32Kbits/4KBytes for 24C32)
      else // Get EEPROM(Emulated) data from Node MCU into our local copy RAM
        write_eeprom(devwifieeprom);
      #if DEBUG
      //read_eeprom(devwifieeprom);
      DBGPRINTLN(D7.get_bpc());
      print_DeviceStruct(devwifieeprom);
      DBGPRINTLN(D_3.print());
      DBGPRINTLN(D_4.print());
      DBGPRINTLN(D_5.print());
      DBGPRINTLN(D_6.print());
      DBGPRINT("Restarting in Manul Mode ESP D7 : ");
      #endif
      delay(5000);
      ESP.restart();
    }
    // action taken in above lines based on counter D7restpin, now rest it to zero for next press
    D7.clr_bpc();
    //D7resetpin = 0;
  }



  // // check button status
  // if(digitalRead(D7_GPIO13) == 0){ // button pressed
  //   if(D7resetpin == 255)
  //     D7resetpin = 0;
  //   D7resetpin++;
  //   DBGPRINT(".");
  // }
  // else{ // button released
    
  //   if(D7resetpin > 1 && D7resetpin <= 7){// push button short press 
  //     if(IsAlexaEnable() && (WiFi.getMode() == WIFI_STA) && (WiFi.status() == WL_CONNECTED))
  //       pair = 50;
  //     DBGPRINT("Paring Mode ");
  //     DBGPRINTLN(pair);
  //   }
    
  //   if(D7resetpin > 7){ // push button long press 
  //     default_DeviceStruct(devwifieeprom);
  //     EnableConfig();
  //     if(rtc.isRTC_DS1307) // Get EEPROM AT24C32 data from DS1307 into our local copy RAM
  //       rtc.saveDeviceStruct(); // the EEPROM AT24C32 (32 = 32Kbits/4KBytes for 24C32)
  //     else // Get EEPROM(Emulated) data from Node MCU into our local copy RAM
  //       write_eeprom(devwifieeprom);
  //     #if DEBUG
  //       DBGPRINT("Restarting in Manul Mode ESP D7 : ");
  //       DBGPRINTLN(D7resetpin);
  //       print_DeviceStruct(devwifieeprom);
  //       D_3.print();
  //       D_4.print();
  //       D_5.print();
  //       D_6.print();
  //     #endif
  //     delay(5000);
  //     ESP.restart();
  //   }
  //   // action taken in above lines based on counter D7restpin, now rest it to zero for next press
  //   D7resetpin = 0;
  // }
}

void espDevices::Loop(){
  
  if ((millis() - lastD7millis) > LOOP_MILLIS/15){ // every second
    lastD7millis = millis();
    // is in paring mode
    if((pair > 0)){ // decrement pair counter every second; in paring mode
      DBGPRINT("Paring Mode ");
      DBGPRINTLN(pair);
      pair = (pair <= 0) ? 0 : --pair;
      return;
    }else{ 
      _D7GPIO13ButtonLoop(); // check push button every second; not in pair mode
      // Device Pins update
      D_3.loop();
      D_4.loop();
      D_5.loop();
      D_6.loop();
    }
  }
  
  // In paring mode keep counting pair counter in(// every second) condition and do not proceed further in loop()
  if((pair > 0))
      return;
  
  if ((millis() - clocksyncmillis) > LOOP_MILLIS*2) {// 15s*2 = 30s 
    clocksyncmillis = millis();

    if(D7.isPressed())
      return;
    
    // Wifi reconnection
    if((devwifieeprom.mcumode == ALEXA_MODE) && (WiFi.getMode() == WIFI_STA)) {
      if(WiFi.status() != WL_CONNECTED){
        WiFi.begin(devwifieeprom.ssid,devwifieeprom.pass);
        if(rtc.isRTC_DS1307 && rtc.RTC.isrunning()){
          if(D_3.isdaytime(rtc.RTC.now().hour(), rtc.RTC.now().minute()))// check for flip flag; take in next version
            D_3.newonof = false;
          else
            D_3.newonof = true;
          if(D_4.isdaytime(rtc.RTC.now().hour(), rtc.RTC.now().minute()))
            D_4.newonof = false;
          else
            D_4.newonof = true;
          if(D_5.isdaytime(rtc.RTC.now().hour(), rtc.RTC.now().minute()))
            D_5.newonof = false;
          else
            D_5.newonof = true;
          if(D_6.isdaytime(rtc.RTC.now().hour(), rtc.RTC.now().minute()))
            D_6.newonof = false;
          else
            D_6.newonof = true;
        }
      }else// sync clock
          rtc.checkandupdateRTC();
    }

    #if DEBUG
      DBGPRINT("[MAIN] Free heap bytes : ");
      DBGPRINT(ESP.getFreeHeap());
      DBGPRINT("\t");
      //rtc.printIsUpdateRequired();
      //DBGPRINT("\t");
      if(devwifieeprom.mcumode == ALEXA_MODE) 
        DBGPRINT("Alexa");
      if(devwifieeprom.mcumode == CONFIG_MODE) 
        DBGPRINT("CONFIG");
      DBGPRINT("\t");
      if(WiFi.status() == WL_CONNECTED)
        DBGPRINT("CONNECTED");
      else
        DBGPRINT("Not CONNECTED");
      DBGPRINT("\t");
      rtc.PrintLine();
      Print_status();
    #endif
  } // end 15 second loop
}
// void espDevices::Loop(ds1307rtc & rtc, DeviceStruct& conf,bool IslocalServer, bool Isfauxmo){
//   D_3.loop(rtc);
//   D_4.loop(rtc);
//   D_5.loop(rtc);
//   D_6.loop(rtc);
//   if ((millis() - lastD7millis) > 1000){
//     lastD7millis = millis();
//     if(digitalRead(D7_GPIO13) == 0){
//       if(D7resetpin == 255)
//         D7resetpin = 0;
//       D7resetpin++;
//       DBG(".");
//     }
//     else{
//       if(D7resetpin > 1 && D7resetpin <= 7){// push button short press 
//         if(IsAlexaEnable() && !IslocalServer)
//           pair = 90;
//         DBG("\nParing Mode %d ",pair);
//         // EnableAlexa();
//         // write_eeprom(devwifieeprom);
//         // DBG("\n\nRestarting in Alexa Mode ESP D7 : %d\n\n",D7resetpin);
//         // delay(2000);
//         // ESP.restart();
//       }
//       if(D7resetpin > 5){ // push button long press 
//         EnableManulMode();
//         write_eeprom(devwifieeprom);
//         DBG("\n\nRestarting in Manul Mode ESP D7 : %d\n\n",D7resetpin);
//         delay(2000);
//         ESP.restart();
//       }
//       D7resetpin = 0;
//     }
//   }
//   // In manul mode; do nothing 
//   if(IslocalServer || Isfauxmo){
//     if ((millis() - clocksyncmillis) > LOOP_MILLIS) {// 900 * 1000ms = 900 seconds = 15minutes
//       clocksyncmillis = millis();
//       //ESP.getFreeHeap()
//       DBG("\n[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
//     }
//     return;
//   }
//   // try to sync clock; in scheduler mode
//   if ((millis() - clocksyncmillis) > LOOP_MILLIS) {// 900 * 1000ms = 900 seconds = 15minutes
//     clocksyncmillis = millis();
//     if(reconnectSTA(conf)) // then check Wifi STA mode and try to connect 
//       esp_clocksync(conf, rtc);
//     ESP.getFreeHeap();
//     DBGPRINTLN("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
//     DBG("\nMode = %s \n",(*mcumode == 1) ? "ALEXA" : (*mcumode == 2) ? "MANUAL" : (*mcumode == 3) ? "SCHEDULER" : "ERROR");
//     DBG("\nIsUpdateRequired() = %i DS1307 RTC is = %s ",rtc.IsUpdateRequired(),(rtc.RTC.isrunning() == 1) ? "Running" : "Not Running");
//     DBG("\nWiFi.getMode() = %s WiFi.status() = %s nonalexa_counter = %i",(WiFi.getMode() == 1) ? "WIFI_STA;" : "WIFI_AP;",(WiFi.status() == WL_CONNECTED) ? "Conneted;" : "Disconnected;",nonalexa_counter);
//     DBG("\n[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
//     Print_status();
//     rtc.printRTC();
//     DBGPRINTLN("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
//   }
// }

// void espDevices::Loop(ds1307rtc & rtc, DeviceStruct& conf,bool IslocalServer, bool Isfauxmo){
//   D_3.loop(rtc);
//   D_4.loop(rtc);
//   D_5.loop(rtc);
//   D_6.loop(rtc);
//   if ((millis() - lastD7millis) > 1000){
//     lastD7millis = millis();
//     if(digitalRead(D7_GPIO13) == 0){
//       if(D7resetpin == 255)
//         D7resetpin = 0;
//       D7resetpin++;
//       DBG(".");
//     }
//     else{
//       if(D7resetpin > 1 && D7resetpin <= 7){// push button short press 
//         if(IsAlexaEnable() && !IslocalServer)
//           pair = 90;
//         DBG("\nParing Mode %d ",pair);
//         // EnableAlexa();
//         // write_eeprom(devwifieeprom);
//         // DBG("\n\nRestarting in Alexa Mode ESP D7 : %d\n\n",D7resetpin);
//         // delay(2000);
//         // ESP.restart();
//       }
//       if(D7resetpin > 5){ // push button long press 
//         EnableManulMode();
//         write_eeprom(devwifieeprom);
//         DBG("\n\nRestarting in Manul Mode ESP D7 : %d\n\n",D7resetpin);
//         delay(2000);
//         ESP.restart();
//       }
//       D7resetpin = 0;
//     }
//   }
//   // In manul mode; do nothing 
//   if(IslocalServer || Isfauxmo){
//     if ((millis() - clocksyncmillis) > LOOP_MILLIS) {// 900 * 1000ms = 900 seconds = 15minutes
//       clocksyncmillis = millis();
//       //ESP.getFreeHeap()
//       DBG("\n[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
//     }
//     return;
//   }
//   // try to sync clock; in scheduler mode
//   if ((millis() - clocksyncmillis) > LOOP_MILLIS) {// 900 * 1000ms = 900 seconds = 15minutes
//     clocksyncmillis = millis();
//     if(reconnectSTA(conf)) // then check Wifi STA mode and try to connect 
//       esp_clocksync(conf, rtc);
//     ESP.getFreeHeap();
//     DBGPRINTLN("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
//     DBG("\nMode = %s \n",(*mcumode == 1) ? "ALEXA" : (*mcumode == 2) ? "MANUAL" : (*mcumode == 3) ? "SCHEDULER" : "ERROR");
//     DBG("\nIsUpdateRequired() = %i DS1307 RTC is = %s ",rtc.IsUpdateRequired(),(rtc.RTC.isrunning() == 1) ? "Running" : "Not Running");
//     DBG("\nWiFi.getMode() = %s WiFi.status() = %s nonalexa_counter = %i",(WiFi.getMode() == 1) ? "WIFI_STA;" : "WIFI_AP;",(WiFi.status() == WL_CONNECTED) ? "Conneted;" : "Disconnected;",nonalexa_counter);
//     DBG("\n[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
//     Print_status();
//     rtc.printRTC();
//     DBGPRINTLN("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
//   }
// }
// void espDevices::Reset(){
//   D_3.reset();
//   D_4.reset();
//   D_5.reset();
//   D_6.reset();
// }
// void espDevices::Enable(){
//   D_3.setEnable(true);
//   D_4.setEnable(true);
//   D_5.setEnable(true);
//   D_6.setEnable(true);
// }
void espDevices::Disable(){
  D_3.setEnable(false);
  D_4.setEnable(false);
  D_5.setEnable(false);
  D_6.setEnable(false);
  DBGPRINTLN("All devices are disabled");
  DBGPRINTLN(D_3.print());
  DBGPRINTLN(D_4.print());
  DBGPRINTLN(D_5.print());
  DBGPRINTLN(D_6.print());
}
// void espDevices::EnableAlexa(){
//   if(*mcumode == ALEXA_MODE)
//     return;
//   setmcumode(ALEXA_MODE);
//   Disable();
// }
// void espDevices::EnableManulMode(){
//   if(*mcumode == ONOF_MODE)
//     return;
//   setmcumode(ONOF_MODE);
//   Disable();
// }
// void espDevices::EnableSchedulerMode(){
//   if(*mcumode == SCHEDULER_MODE)
//     return;
//   setmcumode(SCHEDULER_MODE);
//   Enable();
// }
bool espDevices::reconnectSTA(uint8_t re_tmo){ // call to this function will change WiFi Mode
  if(WiFi.getMode() != WIFI_STA){
    WiFi.mode(WIFI_STA);
    DBGPRINT("WiFi.mode(WIFI_STA) retmo = ");
    DBGPRINTLN(re_tmo);
    delay(WIFI_MODE_DELAY);
  }
  else
    if(WiFi.status() == WL_CONNECTED)
      return true;
  
  // start STA
  WiFi.begin(devwifieeprom.ssid, devwifieeprom.pass);
  // Wait
  while (WiFi.status() != WL_CONNECTED) {
    DBGPRINT(".");
    delay(350); //150
    re_tmo -= 1;
    if(re_tmo == 0)
      break;
  }
  if(re_tmo != 0){
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    #if DEBUG
      DBG_F("Connected re_tmo = %i\n",re_tmo);
      DBG_F("reconnect STA [WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
      DBG_F("First Octact %i Second Octat %i\n",WiFi.localIP()[0],WiFi.localIP()[1]);
    #endif
    return true;
  }
  else{
    #if DEBUG
      DBG_F("\nCould Not connect to SSID : %s ; Passwoed : %s \n",devwifieeprom.ssid,devwifieeprom.pass);
    #endif
    return false;
  }
}
bool espDevices::StationAP(){  // // call to this function will change WiFi Mode
  if(WiFi.getMode() != WIFI_AP)
    WiFi.mode(WIFI_AP);
  else
    return false; //// call this function one time only
  delay(WIFI_MODE_DELAY);
  if(!WiFi.softAPConfig(IPAddress(devwifieeprom.fo1,devwifieeprom.so2,devwifieeprom.to3,devwifieeprom.fo4), IPAddress(devwifieeprom.fo1,devwifieeprom.so2,devwifieeprom.to3,devwifieeprom.fo4), IPAddress(255,255,255,0)))   // subnet FF FF FF 00
    return false;
  ////WiFi.softAP("DeziWebApp","DeziWebApp",1,0,1);
  WiFi.softAP(devwifieeprom.APssid,devwifieeprom.APpass,1,0,4);
  
  IPAddress myIP = WiFi.softAPIP();        //IP Address of our Esp8266 accesspoint(where we can host webpages, and see data)
  DBGPRINT("Access Point IP address : ");
  DBGPRINTLN(myIP);
  return true;
}
// void espDevices::clockSync(ds1307rtc& d1307rtc, uint8_t attempts){
//   //bool connected;
//   if(!d1307rtc.IsUpdateRequired()){
//     DBG("Clock do not require update : IsUpdateRequired() = %i \n",d1307rtc.IsUpdateRequired());
//     return;
//   }
//   DBG("Clock require update : IsUpdateRequired() = %i \n",d1307rtc.IsUpdateRequired());
//   // check WiFi mode
//   if(WiFi.getMode() == WIFI_STA)
//     if(WiFi.status() == WL_CONNECTED){
//       DBGPRINTLN("WiFi.getMode() == WIFI_STA WiFi.status() == WL_CONNECTED : checkandupdateRTC(true)");
//       while(attempts && d1307rtc.IsUpdateRequired()){
//         d1307rtc.checkandupdateRTC();
//         attempts--;
//       }
//     }
//   }
// void espDevices::esp_clocksync(DeviceStruct& conf,ds1307rtc& d1307rtc){
//   if(d1307rtc.IsUpdateRequired()){
//     uint8_t attempts = 10;
//     while(attempts && d1307rtc.IsUpdateRequired()){
//       d1307rtc.checkandupdateRTC();
//       attempts--;
//     }
//     if(IsManulMode())
//       StationAP();
//   }
// }
void espDevices::Print(){
  #if DEBUG
    DBGPRINTLN("###################################################");
    DBG_F("Mode = %s",(*mcumode == ALEXA_MODE) ? "ALEXA" : (*mcumode == 2) ? "MANUAL" : (*mcumode == 3) ? "SCHEDULER" : (*mcumode == CONFIG_MODE) ? "CONFIG" : "ERROR");// CONFIG_MODE
    DBGPRINTLN();
    DBG_F("WiFi.getMode() = %s; WiFi.status() = %s",(WiFi.getMode() == 1) ? "WIFI_STA" : "WIFI_AP",(WiFi.status() == WL_CONNECTED) ? "Conneted" : "Disconnected");
    DBGPRINTLN();
    DBGPRINTLN(D_3.print());
    DBGPRINTLN(D_4.print());
    DBGPRINTLN(D_5.print());
    DBGPRINTLN(D_6.print());
    DBGPRINTLN("###################################################");
  #endif
  }
void espDevices::Print_status(){
  #if DEBUG
    DBG_F("Name: %s; onof: %i; enable: %i; flip: %i\n",D_3.getName(),D_3.onof,D_3.IsEnable(),D_3.IsFlip());
    DBG_F("Name: %s; onof: %i; enable: %i; flip: %i\n",D_4.getName(),D_4.onof,D_4.IsEnable(),D_4.IsFlip());
    DBG_F("Name: %s; onof: %i; enable: %i; flip: %i\n",D_5.getName(),D_5.onof,D_5.IsEnable(),D_5.IsFlip());
    DBG_F("Name: %s; onof: %i; enable: %i; flip: %i\n",D_6.getName(),D_6.onof,D_6.IsEnable(),D_6.IsFlip());
    // DBG_F("Device Name: %s; Status(onof): %i; enable: %i; flip: %i\n",D_3.getName(),D_3.onof,D_3.IsEnable());
    // DBG_F("Device Name: %s; Status(onof): %i; enable: %i; flip: %i\n",D_4.getName(),D_4.onof,D_4.IsEnable());
    // DBG_F("Device Name: %s; Status(onof): %i; enable: %i; flip: %i\n",D_5.getName(),D_5.onof,D_5.IsEnable());
    // DBG_F("Device Name: %s; Status(onof): %i; enable: %i; flip: %i\n",D_6.getName(),D_6.onof,D_6.IsEnable());
  #endif
  }