#include "espdevices.h"

void espDevices::setup(){
  DBGPRINTLN("Setting up espDevice");
  mcumode = &devwifieeprom.mcumode;
  D_3.setup(devwifieeprom);
  D_4.setup(devwifieeprom);
  D_5.setup(devwifieeprom);
  D_6.setup(devwifieeprom);
  modes.setup(); // push button; when button pushed Pin D7_GPIO13 connected to GND; short press and long press
  statusled.setup(); // mcu alexa state; led off alexa is connected; blinking then paring mode
}
void espDevices::_D7GPIO13ButtonLoop(){
  // check button status
  if(modes.isPressed()){ // button pressed
    ++modes;
    //modes.inc_bpc();
    DBGPRINT("modes.get_bpc : ");
    DBGPRINTLN(modes.get_bpc());
    //DBGPRINT(".");
  }
  else{ // button released
    
    // print report
    #if DEBUG
    if(modes.get_bpc() > 0 && modes.get_bpc() < VERYSHORTPRESS){// push button very short press count

      Print();
      Print_status();
      print_DeviceStruct(devwifieeprom);
      // DBGPRINTLN(D_3.print());
      // DBGPRINTLN(D_4.print());
      // DBGPRINTLN(D_5.print());
      // DBGPRINTLN(D_6.print());
    }
    #endif

    if(modes.get_bpc() > VERYSHORTPRESS && modes.get_bpc() <= SHORTPRESS){// push button short press count for entring into Paring Mode for Alexa Device Discovery
      if(IsAlexaEnable() && (WiFi.getMode() == WIFI_STA) && (WiFi.status() == WL_CONNECTED)){
        pair = 50;
        DBGPRINT("Entring in Paring Mode : ");
        DBGPRINTLN(pair);
      }
    }
    
    if(modes.get_bpc() > SHORTPRESS){ // push button long press for factory reset or default config
      default_DeviceStruct(devwifieeprom);
      EnableConfig();
      if(rtc.isAT24C32()) // Get EEPROM AT24C32 data from DS1307 into our local copy RAM
        rtc.saveDeviceStruct(); // the EEPROM AT24C32 (32 = 32Kbits/4KBytes for 24C32)
      else // Get EEPROM(Emulated) data from Node MCU into our local copy RAM
        write_eeprom(devwifieeprom);
      #if DEBUG
      //read_eeprom(devwifieeprom);
      DBGPRINTLN(modes.get_bpc());
      print_DeviceStruct(devwifieeprom);
      DBGPRINTLN(D_3.print());
      DBGPRINTLN(D_4.print());
      DBGPRINTLN(D_5.print());
      DBGPRINTLN(D_6.print());
      DBGPRINT("Restarting in Manul Mode ESP modes : ");
      #endif
      delay(5000);
      ESP.restart();
    }
    // action taken in above lines based on counter D7restpin, now rest it to zero for next press
    modes.clr_bpc();
  }
}
void espDevices::Loop(){
  
  if ((millis() - lastD7millis) > LOOP_MILLIS/15){ // every second
    lastD7millis = millis();
    // is in paring mode
    if((pair > 0)){ // decrement pair counter every second; in paring mode
      DBGPRINT("Paring Mode ");
      DBGPRINTLN(pair);
      pair = (pair <= 0) ? 0 : --pair;
      statusled.parining(); // status led will toggle
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

    if(modes.isPressed())
      return;
    
    // Wifi reconnection
    if((devwifieeprom.mcumode == ALEXA_MODE) && (WiFi.getMode() == WIFI_STA)) {
      if(WiFi.status() != WL_CONNECTED){
        WiFi.begin(devwifieeprom.ssid,devwifieeprom.pass);
        statusled.AlexaDisConnect(); // status led will glow continuously if alexa is disconneted
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
      }else{// connected; sync clock 
          rtc.checkandupdateRTC();
          statusled.AlexaConnted(); // ststus led will switched off if alexa is connected
      }
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
void espDevices::OffAllDev(){
  D_3.Off();
  D_4.Off();
  D_5.Off();
  D_6.Off();
  }
void espDevices::OnAllDev(){
  D_3.On();
  D_4.On();
  D_5.On();
  D_6.On();
  }
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
    DBG_F("Name: %s;    enable: %i;    flip: %i    Pin logical memory veriable status(_active): %i;   Active-Logic: %s    Pin reading value: %i   Pin Voltage Level: %s\n",D_3.getName() ,D_3.IsEnable(), D_3.IsFlip() ,D_3.status(), (D_3.isActive_LOW_Logic()) ? "Active LOW" : "Active HIGH", D_3.isOn(), (D_3.isPinVoltageHigh()) ? "HIGH(3.3v)" : "LOW(GND)" );
    DBG_F("Name: %s;    enable: %i;    flip: %i    Pin logical memory veriable status(_active): %i;   Active-Logic: %s    Pin reading value: %i   Pin Voltage Level: %s\n",D_4.getName() ,D_4.IsEnable(), D_4.IsFlip() ,D_4.status(), (D_4.isActive_LOW_Logic()) ? "Active LOW" : "Active HIGH", D_4.isOn(), (D_4.isPinVoltageHigh()) ? "HIGH(3.3v)" : "LOW(GND)" );
    DBG_F("Name: %s;    enable: %i;    flip: %i    Pin logical memory veriable status(_active): %i;   Active-Logic: %s    Pin reading value: %i   Pin Voltage Level: %s\n",D_5.getName() ,D_5.IsEnable(), D_5.IsFlip() ,D_5.status(), (D_5.isActive_LOW_Logic()) ? "Active LOW" : "Active HIGH", D_5.isOn(), (D_5.isPinVoltageHigh()) ? "HIGH(3.3v)" : "LOW(GND)" );
    DBG_F("Name: %s;    enable: %i;    flip: %i    Pin logical memory veriable status(_active): %i;   Active-Logic: %s    Pin reading value: %i   Pin Voltage Level: %s\n",D_6.getName() ,D_6.IsEnable(), D_6.IsFlip() ,D_6.status(), (D_6.isActive_LOW_Logic()) ? "Active LOW" : "Active HIGH", D_6.isOn(), (D_6.isPinVoltageHigh()) ? "HIGH(3.3v)" : "LOW(GND)" );
    // DBG_F("Name: %s;    enable: %i;    flip: %i    Pin logical memory veriable status(_active): %i;   Pin reading value: %i\n",D_4.getName() ,D_4.IsEnable(), D_4.IsFlip() ,D_4.status(), D_4.isOn());
    // DBG_F("Name: %s;    enable: %i;    flip: %i    Pin logical memory veriable status(_active): %i;   Pin reading value: %i\n",D_5.getName() ,D_5.IsEnable(), D_5.IsFlip() ,D_5.status(), D_5.isOn());
    // DBG_F("Name: %s;    enable: %i;    flip: %i    Pin logical memory veriable status(_active): %i;   Pin reading value: %i\n",D_6.getName() ,D_6.IsEnable(), D_6.IsFlip() ,D_6.status(), D_6.isOn());
    // // DBG_F("Device Name: %s; Status: %i; enable: %i; flip: %i\n",D_3.getName(),D_3.status(),D_3.IsEnable());
    // // DBG_F("Device Name: %s; Status: %i; enable: %i; flip: %i\n",D_4.getName(),D_4.status(),D_4.IsEnable());
    // // DBG_F("Device Name: %s; Status: %i; enable: %i; flip: %i\n",D_5.getName(),D_5.status(),D_5.IsEnable());
    // // DBG_F("Device Name: %s; Status: %i; enable: %i; flip: %i\n",D_6.getName(),D_6.status(),D_6.IsEnable());
  #endif
}