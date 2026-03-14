#ifndef H_PAGE
#define H_PAGE
  #include "page.h"
#endif

// #define COL_CARD(no, dev) \
//   "<div class=\"column\"><div class=\"card\"><H3>" + String(no) + ". " + dev.getName() \
//     + (nodemcu_esp.IsAlexaEnable() ? (dev.onof ? "<button class=\"pillbutton success\">ON</button>" : "<button class=\"pillbutton warning\">OFF</button>") : "") \
//     + (nodemcu_esp.IsConfigEnable() ? (dev.onof ? "<button class=\"pillbutton default\">Disabled</button><p><a href=\"/onof?nf=0&gpio=" + String(dev.getgpio()) + "\" class=\"pillbutton success\" target=\"_self\">ON-Click to Off</a></p>" : "<button class=\"pillbutton default\">Disabled</button><p><a href=\"/onof?nf=1&gpio=" + String(dev.getgpio()) + "\" class=\"pillbutton warning\" target=\"_self\">OFF-Click to ON</a></p>") : "") \
//     + "</H3>" + (dev.isday(MON) ? "<p>Monday</p>": "<p><del>Monday</del></p>") + (dev.isday(TUS) ? "<p>Tusday</p>": "<p><del>Tusday</del></p>") + (dev.isday(WED) ? "<p>Wednesday</p>": "<p><del>Wednesday</del></p>") + (dev.isday(THU) ? "<p>Thursday</p>": "<p><del>Thursday</del></p>") + (dev.isday(FRI) ? "<p>Friday</p>": "<p><del>Friday</del></p>") + (dev.isday(SAT) ? "<p>Saturday</p>": "<p><del>Saturday</del></p>") + (dev.isday(SUN) ? "<p>Sunday</p>": "<p><del>Sunday</del></p>") \
//     + "<p>Off Timing: " + ds1307rtc::hhmmss(dev.getOffHour(), dev.getOffMinute(), 0) + "</p>" + " <p>On Timing: " + ds1307rtc::hhmmss(dev.getOnHour(),dev.getOnMinute(), 0) + "</p>" + (dev.IsFlip() ? "<p>Fliped</p>" : "<p><del>Fliped</del></p>") + "</div></div>"

#define COL_CARD(no, dev) \
  "<div class=\"column\"><div class=\"card\"><H3>" + String(no) + ". " + dev.getName() \
    + (nodemcu_esp.IsAlexaEnable() ? (dev.onof ? "<button class=\"pillbutton success\">ON</button>" : "<button class=\"pillbutton warning\">OFF</button>") : "") \
    + (nodemcu_esp.IsConfigEnable() ? (dev.onof ? "<button class=\"pillbutton default\">Disabled</button><p><a href=\"/onof?nf=0&gpio=" + String(dev.getgpio()) + "\" class=\"pillbutton success\" target=\"_self\">ON-Click to Off</a></p>" : "<button class=\"pillbutton default\">Disabled</button><p><a href=\"/onof?nf=1&gpio=" + String(dev.getgpio()) + "\" class=\"pillbutton warning\" target=\"_self\">OFF-Click to ON</a></p>") : "") \
    + "</div></div>"


void defWebPage(AsyncWebServerRequest *request){
  String _nav = "<H1>DeziWebApp Systems Pvt. Ltd.</H1><H3>IoT Server Status</H3><div class=\"pill-nav\"><a class=\"active\" href=\"/\">Home</a><a href=\"/wifi\">Wifi</a><a href=\"/wifiap\">WifiAP</a><a href=\"/devices\">Devices</a><a href=\"/week\">Week Days</a><a href=\"/time\">On/Off Timings</a><a href=\"/commit\">Save All Changes</a><a href=\"/about\">About</a></div>";
    String _page = String("<H3>") + "Wifi SSID :- " + "<button class=\"pillbutton success\">" + String(devwifieeprom.ssid) + "</button>" + (WiFi.status() == WL_CONNECTED ? " is <button class=\"pillbutton success\">Connected</button>" : " is <button class=\"pillbutton warning\">Not Connected</button>")
    + (nodemcu_esp.IsAlexaEnable() ? "<button class=\"pillbutton warning\">Alexa enabled</button>" : "<a href=\"/alexa?enable=1\" class=\"pillbutton info\" target=\"_self\">Alexa Disabled--Click to Enable</a>" )
    + (nodemcu_esp.IsConfigEnable() ? "<button class=\"pillbutton warning\">Manual Mode</button>" : "<a href=\"/manual?enable=1\" class=\"pillbutton info\" target=\"_self\">Manual Mode Disabled--Click to Enable</a>")
    //+ (nodemcu_esp.IsSchedulerMode() ? "<button class=\"pillbutton warning\">Schedular Mode</button>" : "<a href=\"/local?enable=1\" class=\"pillbutton info\" target=\"_self\">Schedular Mode Disabled--Click to Enable</a>")
    + "</H3><div class=\"row\">"
    + COL_CARD(1, nodemcu_esp.D_3)
    + COL_CARD(2, nodemcu_esp.D_4)
    + COL_CARD(3, nodemcu_esp.D_5)
    + COL_CARD(4, nodemcu_esp.D_6)
    + "</div>";
  request->send(200, "text/html", String(header_html_defpage) + "</style></head><body>" + _nav
  + _page
  + "</div></body></html>" );
}
void wifiWebPage(AsyncWebServerRequest *request){
  String _nav = "<H1>DeziWebApp Systems Pvt. Ltd.</H1><H3>IoT Server Status</H3><div class=\"pill-nav\"><a href=\"/\">Home</a><a class=\"active\" href=\"/wifi\">Wifi</a><a href=\"/wifiap\">WifiAP</a><a href=\"/devices\">Devices</a><a href=\"/week\">Week Days</a><a href=\"/time\">On/Off Timings</a><a href=\"/commit\">Save All Changes</a><a href=\"/about\">About</a></div>";
  if(request->args() == 0)
    request->send(200, "text/html", String(header_html) + "</style></head><body>" + _nav + String(wifi_body_html));
  else
  {
    if(request->hasParam(OLDPASS)){
      String op = request->getParam(OLDPASS)->value();
      String po(devwifieeprom.pass);
      DBGPRINT(F("Entered pass = "));
      DBGPRINTLN(op.c_str());
      DBGPRINT(F("devwifieeprom.pass "));
      DBGPRINTLN(po.c_str());
      if(op != po)
        request->send(200, "text/html", String(header_html) + "</style></head><body><H2>Pervious Wifi Password Mismatch</H2>" + _nav + String(wifi_body_html));
      else{
        if(request->hasParam(SID) && request->getParam(SID)->value().length() > 0){
          String newsid(request->getParam(SID)->value());
          changeDeviceStruct_CarrayLEN(devwifieeprom.ssid,newsid);
        }
        if(request->hasParam(PASWD) && request->getParam(PASWD)->value().length() > 0){
          String newpass(request->getParam(PASWD)->value());
          changeDeviceStruct_CarrayLEN(devwifieeprom.pass,newpass);
        }
        DBGPRINT("New SSID is ");
        DBGPRINTLN(devwifieeprom.ssid);
        DBGPRINT("New PASWD is ");
        DBGPRINTLN(devwifieeprom.pass);
        delay(100);
        ESP.getFreeHeap();
        defWebPage(request);
      }
    }
    else{
      ESP.getFreeHeap();
      request->send(200, "text/html", String(header_html) + "</style></head><body>" + _nav + String(wifi_body_html));
    }
  }
}
void wifiAPWebPage(AsyncWebServerRequest *request){
  String _nav = "<H1>DeziWebApp Systems Pvt. Ltd.</H1><H3>IoT Server Status</H3><div class=\"pill-nav\"><a href=\"/\">Home</a><a href=\"/wifi\">Wifi</a><a class=\"active\" href=\"/wifiap\">WifiAP</a><a href=\"/devices\">Devices</a><a href=\"/week\">Week Days</a><a href=\"/time\">On/Off Timings</a><a href=\"/commit\">Save All Changes</a><a href=\"/about\">About</a></div>";
  if(request->args() == 0)
    request->send(200, "text/html", String(header_html) + "</style></head><body>" + _nav + String(wifi_ap_body_html));
  else
  {
    if(request->hasParam(OLDPASS)){
      String op = request->getParam(OLDPASS)->value();
      String po(devwifieeprom.pass);
      DBGPRINT(F("Entered pass = "));
      DBGPRINTLN(op.c_str());
      DBGPRINT(F("devwifieeprom.pass "));
      DBGPRINTLN(po.c_str());
      if(op != po)
        request->send(200, "text/html", String(header_html) + "</style></head><body><H2>Pervious Wifi Password Mismatch</H2>" + _nav + String(wifi_ap_body_html));
      else{
        if(request->hasParam(APSSID) && request->getParam(APSSID)->value().length() > 0){
          String newsid(request->getParam(APSSID)->value());
          changeDeviceStruct_CarrayLEN(devwifieeprom.APssid,newsid);
        }
        if(request->hasParam(APPASWD) && request->getParam(APPASWD)->value().length() > 0){
          String newpass(request->getParam(APPASWD)->value());
          changeDeviceStruct_CarrayLEN(devwifieeprom.APpass,newpass);
        }
        if(request->hasParam(IP) && request->getParam(IP)->value().length() > 0){
          String ip(request->getParam(IP)->value());
          char buf[4]; buf[3] = '\0'; uint8_t j = 0;
          uint8_t octat_no = 0; uint8_t n_ip[4];
          for(uint8_t i = 0; i <= ip.length() && octat_no < 4; i++){
            if(ip.charAt(i) == '.' || i == ip.length()){
              n_ip[octat_no] = String(buf).toInt();
              octat_no++;
              j=0;buf[0] = '\0';buf[1] = '\0';buf[2] = '\0';
            }
            else{
              if(j < 3){
                buf[j] = ip.charAt(i);
                j++;
              }
            }
          }
          devwifieeprom.fo1 = n_ip[0];
          devwifieeprom.so2 = n_ip[1];
          devwifieeprom.to3 = n_ip[2];
          devwifieeprom.fo4 = n_ip[3];
          #if DEBUG
          DBG_F("ip 1 = %d 2 = %d 3 = %d 4 = %d",n_ip[0],n_ip[1],n_ip[2],n_ip[3]);
          #endif
        }
        delay(100);
        ESP.getFreeHeap();
        defWebPage(request);
      }
    }
    else
      request->send(200, "text/html", String(header_html) + "</style></head><body>" + _nav + String(wifi_ap_body_html));
  }
}
void PageDeviesName(AsyncWebServerRequest *request){
  String _nav = "<H1>DeziWebApp Systems Pvt. Ltd.</H1><H3>IoT Server Status</H3><div class=\"pill-nav\"><a href=\"/\">Home</a><a href=\"/wifi\">Wifi</a><a href=\"/wifiap\">WifiAP</a><a class=\"active\" href=\"/devices\">Devices</a><a href=\"/week\">Week Days</a><a href=\"/time\">On/Off Timings</a><a href=\"/commit\">Save All Changes</a><a href=\"/about\">About</a></div>";
  if(request->args() == 0)
    request->send(200, "text/html", String(header_html) + "</style></head><body>" + _nav + String(devices_body_html));
  else{ 
    if(request->hasParam(OLDPASS)){
      String op(request->getParam(OLDPASS)->value());
      String po(devwifieeprom.pass);
      DBGPRINT("\nEntered pass = ");
      DBGPRINTLN(op);
      DBGPRINTLN(po);
      if(op != po){
        request->send(200, "text/html", String(header_html) + "</style></head><body><H2>Pervious Wifi Password Mismatch</H2>" + _nav +String(devices_body_html));
        return;
      }
    }   
    if(request->hasParam("div1") && request->getParam("div1")->value().length() > 0){
      String newdev1(request->getParam("div1")->value());
      DBGPRINTLN(newdev1);
      nodemcu_esp.D_3.setName(newdev1);
      DBGPRINTLN(nodemcu_esp.D_3.getName());
    }
    if(request->hasParam("div2") && request->getParam("div2")->value().length() > 0){
      String newdev2(request->getParam("div2")->value());
      DBGPRINTLN(newdev2);
      nodemcu_esp.D_4.setName(newdev2);
      DBGPRINTLN(nodemcu_esp.D_4.getName());
    }
    if(request->hasParam("div3") && request->getParam("div3")->value().length() > 0){
      //DBGPRINTLN("request->getParam("div3")->value() = %s;\n", request->getParam("div3")->value());
      String newdev3(request->getParam("div3")->value());
      DBGPRINTLN(newdev3);
      nodemcu_esp.D_5.setName(newdev3);
      DBGPRINTLN(nodemcu_esp.D_5.getName());
    }
    if(request->hasParam("div4") && request->getParam("div4")->value().length() > 0){
      String newdev4(request->getParam("div4")->value());
      DBGPRINTLN(newdev4);
      nodemcu_esp.D_6.setName(newdev4);
      DBGPRINTLN(nodemcu_esp.D_6.getName());
    }
    defWebPage(request);
  }
  ESP.getFreeHeap();
}
// void weekpage(AsyncWebServerRequest *request){
//   String _nav = "<H1>DeziWebApp Systems Pvt. Ltd.</H1><H3>IoT Server Status</H3><div class=\"pill-nav\"><a href=\"/\">Home</a><a href=\"/wifi\">Wifi</a><a href=\"/wifiap\">WifiAP</a><a href=\"/devices\">Devices</a><a class=\"active\" href=\"/week\">Week Days</a><a href=\"/time\">On/Off Timings</a><a href=\"/commit\">Save All Changes</a><a href=\"/about\">About</a></div>";
//   if(request->args() == 0)
//     request->send(200, "text/html", String(header_html) + "</style></head><body>" + _nav + String(week_body_html));
//   else{ 
//         uint8_t week = 0x00;
//         if(request->hasParam(OLDPASS)){
//           String op(request->getParam(OLDPASS)->value());
//           String po(devwifieeprom.pass);
//           DBGPRINTLN("Entered pass = %s",op);
//           DBGPRINTLN(po);
//           if(op != po){
//             request->send(200, "text/html", String(header_html) + "</style></head><body><H2>Pervious Wifi Password Mismatch</H2>" + _nav + String(week_body_html));
//             return;
//           }
//         }
//         if(request->hasParam("0")){String MoN(request->getParam("0")->value());if(MoN == "Mon"){week = week | MON;}}
//         if(request->hasParam("1")){String TuE(request->getParam("1")->value());if(TuE == "Tue"){week = week | TUS;}}
//         if(request->hasParam("2")){String WeD(request->getParam("2")->value());if(WeD == "Wed"){week = week | WED;}}
//         if(request->hasParam("3")){String ThU(request->getParam("3")->value());if(ThU == "Thu"){week = week | THU;}}
//         if(request->hasParam("4")){String FrI(request->getParam("4")->value());if(FrI == "Fri"){week = week | FRI;}}
//         if(request->hasParam("5")){String SaT(request->getParam("5")->value());if(SaT == "Sat"){week = week | SAT;}}
//         if(request->hasParam("6")){String SuN(request->getParam("6")->value());if(SuN == "Sun"){week = week | SUN;}}
//         // // for (uint8_t i = 2; i <= request->args(); i++){
//         // //   if(request->argName(i) == "0"){String MoN(request->arg(i));if(MoN == "Mon"){week = week | MON;}}
//         // //   if(request->argName(i) == "1"){String TuE(request->arg(i));if(TuE == "Tue"){week = week | TUS;}}
//         // //   if(request->argName(i) == "2"){String WeD(request->arg(i));if(WeD == "Wed"){week = week | WED;}}
//         // //   if(request->argName(i) == "3"){String ThU(request->arg(i));if(ThU == "Thu"){week = week | THU;}}
//         // //   if(request->argName(i) == "4"){String FrI(request->arg(i));if(FrI == "Fri"){week = week | FRI;}}
//         // //   if(request->argName(i) == "5"){String SaT(request->arg(i));if(SaT == "Sat"){week = week | SAT;}}
//         // //   if(request->argName(i) == "6"){String SuN(request->arg(i));if(SuN == "Sun"){week = week | SUN;}}
//         // // }
//         ESP.getFreeHeap();        
//         if(request->hasParam("dn")){
//           String dev(request->getParam("dn")->value());
//           if(dev == "1"){nodemcu_esp.D_3.setWeek(week);}
//           if(dev == "2"){nodemcu_esp.D_4.setWeek(week);}
//           if(dev == "3"){nodemcu_esp.D_5.setWeek(week);}
//           if(dev == "4"){nodemcu_esp.D_6.setWeek(week);}
//           defWebPage(request);
//         }
//       }
// }
// void timepage(AsyncWebServerRequest *request){
//   String _nav = "<H1>DeziWebApp Systems Pvt. Ltd.</H1><H3>IoT Server Status</H3><div class=\"pill-nav\"><a href=\"/\">Home</a><a href=\"/wifi\">Wifi</a><a href=\"/wifiap\">WifiAP</a><a href=\"/devices\">Devices</a><a href=\"/week\">Week Days</a><a class=\"active\" href=\"/time\">On/Off Timings</a><a href=\"/commit\">Save All Changes</a><a href=\"/about\">About</a></div>";
//   if(request->args() == 0)
//     request->send(200, "text/html", String(header_html) + "</style></head><body>" + _nav +  String(time_body_html));
//   else{ 
//         if(request->hasParam(OLDPASS)){
//           String op(request->getParam(OLDPASS)->value());
//           String po(devwifieeprom.pass);
//           DBGPRINTLN("Entered pass = %s",op);
//           DBGPRINTLN(po);
//           if(op != po){
//             request->send(200, "text/html", String(header_html) + "</style></head><body><H2>Pervious Wifi Password Mismatch</H2>" + _nav + String(time_body_html));
//             return;
//           }
//         }
        
//         uint8_t onh,onm,ofh,ofm;bool enable_ = 1;bool flip_ = 0;
//         if(request->hasParam("onh")){
//           String oh(request->getParam("onh")->value());
//           onh = (uint8_t)oh.toInt();
//         }
//         if(request->hasParam("onm")){
//           String om(request->getParam("onm")->value());
//           onm = (uint8_t)om.toInt();
//         }
//         if(request->hasParam("offh")){
//           String oh(request->getParam("offh")->value());
//           //DBGPRINTLN(oh);
//           ofh = (uint8_t)oh.toInt();
//           //DBGPRINTLN(ofh);
//         }
//         if(request->hasParam("offm")){
//           String om(request->getParam("offm")->value());
//           //DBGPRINTLN(om);
//           ofm = (uint8_t)om.toInt();
//           //DBGPRINTLN(ofm);
//         }
        
//         if(request->hasParam("status")){
//           String en(request->getParam("status")->value());
//           if(en == "off")
//             enable_ = false;
//         }
//         if(request->hasParam("flip")){
//           String fl_ip(request->getParam("flip")->value());
//           if(fl_ip == "on")
//             flip_ = true;
//         }
        
//         if(request->hasParam("dn")){
//           String dev(request->getParam("dn")->value());
//           if(dev == "1"){nodemcu_esp.D_3.setOnHour(onh); nodemcu_esp.D_3.setOnMinute(onm); nodemcu_esp.D_3.setOffHour(ofh); nodemcu_esp.D_3.setOffMinute(ofm); nodemcu_esp.D_3.setEnable(enable_);nodemcu_esp.D_3.setFlip(flip_);}
//           if(dev == "2"){nodemcu_esp.D_4.setOnHour(onh); nodemcu_esp.D_4.setOnMinute(onm); nodemcu_esp.D_4.setOffHour(ofh); nodemcu_esp.D_4.setOffMinute(ofm); nodemcu_esp.D_4.setEnable(enable_);nodemcu_esp.D_4.setFlip(flip_);}
//           if(dev == "3"){nodemcu_esp.D_5.setOnHour(onh); nodemcu_esp.D_5.setOnMinute(onm); nodemcu_esp.D_5.setOffHour(ofh); nodemcu_esp.D_5.setOffMinute(ofm); nodemcu_esp.D_5.setEnable(enable_);nodemcu_esp.D_5.setFlip(flip_);}
//           if(dev == "4"){nodemcu_esp.D_6.setOnHour(onh); nodemcu_esp.D_6.setOnMinute(onm); nodemcu_esp.D_6.setOffHour(ofh); nodemcu_esp.D_6.setOffMinute(ofm); nodemcu_esp.D_6.setEnable(enable_);nodemcu_esp.D_6.setFlip(flip_);}
//         }
//         //DBGPRINTLN("enable_ %d\n",enable_);
//         defWebPage(request);
//       }
// }
void commitpage(AsyncWebServerRequest *request){
  String _nav = "<H1>DeziWebApp Systems Pvt. Ltd.</H1><H3>IoT Server Status</H3><div class=\"pill-nav\"><a href=\"/\">Home</a><a href=\"/wifi\">Wifi</a><a href=\"/wifiap\">WifiAP</a><a href=\"/devices\">Devices</a><a href=\"/week\">Week Days</a><a href=\"/time\">On/Off Timings</a><a class=\"active\" href=\"/commit\">Save All Changes</a><a href=\"/about\">About</a></div>";
  if(request->method() == HTTP_GET){
    if(request->args() == 0)
      request->send(200, "text/html", String(header_html) + "</style></head><body>" + _nav + "<h2>IoT Web Server : Save Changes?</h2><h3><a href=\"/commit?save=1\" class=\"btn regbtn danger\" target=\"_self\">Click to Save Changes</a></h3> \
      <h2>Factory Reset?</h2><h3><a href=\"/factoryreset?reset=1\" class=\"btn regbtn danger\" target=\"_self\">Click to Fectory Reset</a></h3> \
      <h2>ReStart?</h2><h3><a href=\"/restart?reset=1\" class=\"btn regbtn danger\" target=\"_self\">Click to ReStart</a></h3>\
      </body></html>");
    else{
      if(request->hasParam("save") && request->getParam("save")->value().equals("1")){    
        

        if(rtc.isAT24C32()){ // Get EEPROM AT24C32 data from DS1307 into our local copy RAM
          // the EEPROM AT24C32 (32 = 32Kbits/4KBytes for 24C32)
          rtc.saveDeviceStruct();
          rtc.loadDeviceStruct();
        }
        else{ // Get EEPROM(Emulated) data from Node MCU into our local copy RAM
          write_eeprom(devwifieeprom);
          read_eeprom(devwifieeprom);
        }
        
        // if(rtc.isRTC_DS1307)
        //   rtc.saveDeviceStruct();
        // else{ // read flash memory as esp8266 do not have EEPROM instead it uses flash memory as EEPROM(Emulated)
          // write_eeprom(devwifieeprom);

        //   // Get EEPROM data into our local copy 
        //   read_eeprom(devwifieeprom);
        // }
        
        defWebPage(request);
      }
    }
  }
}
void aboutpage(AsyncWebServerRequest *request){
  String _nav = "<H1>DeziWebApp Systems Pvt. Ltd.</H1><H3>IoT Server Status</H3><div class=\"pill-nav\"><a href=\"/\">Home</a><a href=\"/wifi\">Wifi</a><a href=\"/wifiap\">WifiAP</a><a href=\"/devices\">Devices</a><a href=\"/week\">Week Days</a><a href=\"/time\">On/Off Timings</a><a href=\"/commit\">Save All Changes</a><a class=\"active\" href=\"/about\">About</a></div>";
  request->send(200, "text/html", String(header_html) + "</style></head><body>" + _nav + "DeziWebApp Systems Pvt. Ltd." + "</body></html>");
}
void manualonof(AsyncWebServerRequest *request){ ///e.g. manulonof?nf=1&gpio=2
  //String _nav = "<H1>DeziWebApp Systems Pvt. Ltd.</H1><H3>IoT Server Status</H3><div class=\"pill-nav\"><a class=\"active\" href=\"/\">Home</a><a href=\"/wifi\">Wifi</a><a href=\"/devices\">Devices</a><a href=\"/week\">Week Days</a><a href=\"/time\">On/Off Timings</a><a href=\"/commit\">Save All Changes</a><a href=\"/about\">About</a></div>";
  if(request->args() == 0)
    defWebPage(request);
  else{
    if(request->hasParam("nf") && request->getParam("nf")->value().toInt() == O_N){
      // on
      if(request->hasParam("gpio") && request->getParam("gpio")->value().toInt() == D3_GPIO0)
        nodemcu_esp.D_3.newonof = nodemcu_esp.D_3.On();
      if(request->hasParam("gpio") && request->getParam("gpio")->value().toInt() == D4_GPIO2)
        nodemcu_esp.D_4.newonof = nodemcu_esp.D_4.On();
      if(request->hasParam("gpio") && request->getParam("gpio")->value().toInt() == D5_GPIO14)
        nodemcu_esp.D_5.newonof = nodemcu_esp.D_5.On();
      if(request->hasParam("gpio") && request->getParam("gpio")->value().toInt() == D6_GPIO12)
        nodemcu_esp.D_6.newonof = nodemcu_esp.D_6.On();
    }
    if(request->hasParam("nf") && request->getParam("nf")->value().toInt() == O_FF){
      // off
      if(request->hasParam("gpio") && request->getParam("gpio")->value().toInt() == D3_GPIO0)
        nodemcu_esp.D_3.newonof = nodemcu_esp.D_3.Off();
      if(request->hasParam("gpio") && request->getParam("gpio")->value().toInt() == D4_GPIO2)
        nodemcu_esp.D_4.newonof = nodemcu_esp.D_4.Off();
      if(request->hasParam("gpio") && request->getParam("gpio")->value().toInt() == D5_GPIO14)
        nodemcu_esp.D_5.newonof = nodemcu_esp.D_5.Off();
      if(request->hasParam("gpio") && request->getParam("gpio")->value().toInt() == D6_GPIO12)
        nodemcu_esp.D_6.newonof = nodemcu_esp.D_6.Off();
    }
    defWebPage(request);
  }
}
//void enabel
void alexa(AsyncWebServerRequest *request){
  if(request->args() == 0)
    defWebPage(request);
  else{
    if(request->hasParam("enable") && request->getParam("enable")->value().equals("1")){
      //request->send(200, "text/html", "<!doctype html><html><body><H1>Alexa Mode Enabled</H1></body></html>" );
      nodemcu_esp.EnableAlexa();
      DBGPRINTLN("Alexa Mode Enabled");
    }
    // if(request->hasParam("enable") && request->getParam("enable")->value().equals("0")){
    //   disabelalexa();
    //   DBGPRINTLN("Alexa Mode Disable");
    // }
    defWebPage(request);
  }
}
//void enabel local scheduler
void local(AsyncWebServerRequest *request){
  if(request->args() == 0)
    defWebPage(request);
  else{
    if(request->hasParam("enable") && request->getParam("enable")->value().equals("1")){
      nodemcu_esp.IsConfigEnable();
      DBGPRINTLN("local schedule Mode Enabled");
    }
    defWebPage(request);
  }
}
//void enabel manual
void manual(AsyncWebServerRequest *request){
  if(request->args() == 0)
    defWebPage(request);
  else{
    if(request->hasParam("enable") && request->getParam("enable")->value().equals("1")){
      nodemcu_esp.IsConfigEnable();
      DBGPRINTLN("Manual Mode Enabled");
    }
    defWebPage(request);
  }
}
void ReStart(AsyncWebServerRequest *request){
  if(request->args() == 0)
    defWebPage(request);
  else{
    if(request->hasParam("reset") && request->getParam("reset")->value().equals("1")){
      DBGPRINTLN("Restart ESP");
      ESP.reset();
    }
    defWebPage(request);
  }
}
void factoryReset(AsyncWebServerRequest *request){
  if(request->args() == 0)
    defWebPage(request);
  else{
    if(request->hasParam("reset") && request->getParam("reset")->value().equals("1")){
      nodemcu_esp.FactoryRest();
      DBGPRINTLN("Factory Rest");
    }
    defWebPage(request);
  }
}
/////////////////////// helper functions
