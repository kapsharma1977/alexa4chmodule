#ifndef H_MACROS
  #define H_MACROS
  #include "macros.h"
#endif

#ifndef H_DEVICE
#define H_DEVICE
#include "Device.h"
#endif
#ifndef H_ESPDEVICES
  #define H_ESPDEVICES
  #include "espdevices.h"
#endif
#ifndef H_RTCDS1307
#define H_RTCDS1307
  #include "rtcds1307.h"
#endif
#ifndef H_DEVICESTUCT
  #define H_DEVICESTUCT
  #include "devicestruct.h"
#endif

#ifndef H_ASYNCWEBSERVER
  #define H_ASYNCWEBSERVER
  #include <ESPAsyncWebServer.h>
#endif


//// wifiWebPage UI variables
#define OLDPASS "oldpass"
#define SID     "ssid"
#define PASWD  "paswd"
#define APSSID  "apssid"
#define APPASWD "appaswd"
#define IP      "ip"
#define O_N     1
#define O_FF    0

extern struct DeviceStruct devwifieeprom;

extern espDevices nodemcu_esp;
extern ds1307rtc rtc;
extern uint8_t nonalexa_counter; // it will count to zero from 255 to check internet connection and update clock.

extern bool StationAP(bool mode);
extern bool reconnectSTA(uint8_t re_tmo);


const char header_html_defpage[] PROGMEM = R"rawliteral(
  <!DOCTYPE html><html><head><meta name="viewport" content="width=device-width,initial-scale=1"><base href="http://192.168.4.1/" target="_self">
  <style> body{font-family:Arial,Helvetica,sans-serif,"Lato";} *{box-sizing:border-box;}
  .pill-nav a{display:inline-block;color:black;text-align:center;padding:14px;text-decoration:none;font-size: 17px;border-radius:5px;} .pill-nav a:hover{background-color:#ddd;color:black;} .pill-nav a.active{background-color:dodgerblue;color: white;}
  .card{box-shadow:0 4px 8px 0 rgba(0, 0, 0, 0.2);padding:16px;text-align:center;background-color:#f1f1f1;} .column{float:left;width:25%;padding:0 10px;} .row {margin: 0 -5px;} .row:after{content:"";display:table;clear:both;}
  .btn{border:none;color:white;padding:14px 28px;font-size:16px;cursor:pointer;} .pillbutton{border:none;color:black;padding:10px 20px;text-align:center;text-decoration:none;display:inline-block;margin:4px 2px;cursor:pointer;border-radius:16px;} .success{background-color:#04AA6D;} .success:hover{background-color:#46a049;} .warning{background-color:#ff9800;} .warning:hover{background:#e68a00;} .default{background-color:#e7e7e7;color:black;} .default:hover{background: #ddd;}
  h1,h2,h3{text-align:center}
  @media screen and (max-height: 600px){.sidenav {padding-top:15px;} .sidenav a {font-size:18px;} .column{width:100%;display:block;margin-bottom:20px;}}
  
)rawliteral";

const char header_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE html><html><head><meta name="viewport" content="width=device-width,initial-scale=1"><base href="http://192.168.4.1/" target="_self">
  <style> body{font-family:Arial,Helvetica,sans-serif,"Lato";} *{box-sizing:border-box;}
  .pill-nav a{display:inline-block;color:black;text-align:center;padding:14px;text-decoration:none;font-size: 17px;border-radius:5px;} .pill-nav a:hover{background-color:#ddd;color:black;} .pill-nav a.active{background-color:dodgerblue;color: white;}
  form{border:3px solid #111111;} .container{padding:16px;background-color:white;}
  input[type=text],input[type=password],input[type=number]{width:100%;padding:15px;margin:5px 0 22px 0;display:inline-block;border:none;background: #f1f1f1;} input[type=text]:focus,input[type=password]:focus,input[type=number]:focus{background-color:#ddd;outline:none;}
  .btn{border:none;color:white;padding:14px 28px;font-size:16px;cursor:pointer;} .pillbutton{border:none;color:black;padding:10px 20px;text-align:center;text-decoration:none;display:inline-block;margin:4px 2px;cursor:pointer;border-radius:16px;} .regbtn{width:100%;opacity:0.9;}
  .success{background-color:#04AA6D;} .success:hover{background-color:#46a049;} .info{background-color:#2196F3;} .info:hover{background:#0b7dda;} .warning{background-color:#ff9800;} .warning:hover{background:#e68a00;} .danger{background-color:#f44336;} .danger:hover{background:#da190b;} .default{background-color:#e7e7e7;color:black;} .default:hover{background: #ddd;}
  h1,h2,h3{text-align:center}
  @media screen and (max-height: 450px){.sidenav {padding-top:15px;} .sidenav a {font-size:18px;}}
  
)rawliteral"; // <body><H1>DeziWebApp Systems Pvt. Ltd.</H1><H3>IoT Server Status</H3><div class="pill-nav"><a class="active" href="/">Home</a><a href="/wifi">Wifi</a><a href="/devices">Devices</a><a href="/week">Week Days</a><a href="/time">On/Off Timings</a><a href="#about">About</a></div>

// second version sidebar
// const char header_html[] PROGMEM = R"rawliteral(
//   <!DOCTYPE html><html><head><meta name="viewport" content="width=device-width,initial-scale=1"><base href="http://192.168.4.1/" target="_blank">
//   <style> body{font-family:Arial,Helvetica,sans-serif,"Lato";} *{box-sizing:border-box;}
//   .sidenav{height: 100%;width: 160px;position: fixed;z-index: 1;top: 0;left: 0;background-color: #111;overflow-x: hidden;padding-top: 20px;} .sidenav a {padding: 6px 8px 6px 16px;text-decoration: none;font-size: 25px;color: #818181;display: block;} .sidenav a:hover {color: #f1f1f1;} .main {margin-left: 160px;font-size:28px;padding:0px 10px;}
//   form{border:3px solid #111111;} .container{padding:16px;background-color:white;} input[type=text],input[type=password]input[type=number]{width:100%;padding:15px;margin:5px 0 22px 0;display:inline-block;border:none;background: #f1f1f1;} input[type=text]:focus,input[type=password]:focus,input[type=number]:focus{background-color:#ddd;outline:none;}
//   .btn{border:none;color:white;padding:14px 28px;font-size:16px;cursor:pointer;} .pillbutton{border:none;color:black;padding:10px 20px;text-align:center;text-decoration:none;display:inline-block;margin:4px 2px;cursor:pointer;border-radius:16px;} .regbtn{width:100%;opacity:0.9;}
//   .success{background-color:#04AA6D;} .success:hover{background-color:#46a049;} .info{background-color:#2196F3;} .info:hover{background:#0b7dda;} .warning{background-color:#ff9800;} .warning:hover{background:#e68a00;} .danger{background-color:#f44336;} .danger:hover{background:#da190b;} .default{background-color:#e7e7e7;color:black;} .default:hover{background: #ddd;}
//   @media screen and (max-height: 450px){.sidenav {padding-top:15px;} .sidenav a {font-size:18px;}}
//   </style></head>
// )rawliteral";
// first version
// const char style[] PROGMEM = R"rawliteral(
//   <style> body{font-family:Arial,Helvetica,sans-serif,"Lato";} *{box-sizing:border-box;}
//   .sidenav{height: 100%;width: 160px;position: fixed;z-index: 1;top: 0;left: 0;background-color: #111;overflow-x: hidden;padding-top: 20px;} .sidenav a {padding: 6px 8px 6px 16px;text-decoration: none;font-size: 25px;color: #818181;display: block;} .sidenav a:hover {color: #f1f1f1;} .main {margin-left: 160px;font-size:28px;padding:0px 10px;}
//   form{border:3px solid #111111;} .container{padding:16px;background-color:white;} input[type=text],input[type=password]input[type=number]{width:100%;padding:15px;margin:5px 0 22px 0;display:inline-block;border:none;background: #f1f1f1;} input[type=text]:focus,input[type=password]:focus,input[type=number]:focus{background-color:#ddd;outline:none;}
//   .btn{border:none;color:white;padding:14px 28px;font-size:16px;cursor:pointer;} .pillbutton{border:none;color:black;padding:10px 20px;text-align:center;text-decoration:none;display:inline-block;margin:4px 2px;cursor:pointer;border-radius:16px;} .regbtn{width:100%;opacity:0.9;}
//   .success{background-color:#04AA6D;} .success:hover{background-color:#46a049;} .info{background-color:#2196F3;} .info:hover{background:#0b7dda;} .warning{background-color:#ff9800;} .warning:hover{background:#e68a00;} .danger{background-color:#f44336;} .danger:hover{background:#da190b;} .default{background-color:#e7e7e7;color:black;} .default:hover{background: #ddd;}
//   @media screen and (max-height: 450px){.sidenav {padding-top:15px;} .sidenav a {font-size:18px;}}
//   </style>
//   )rawliteral";

// <body>
// <div class="sidenav">
//   <a href="#about">About</a>
//   <a href="#services">Services</a>
//   <a href="#clients">Clients</a>
//   <a href="#contact">Contact</a>
// </div>

// <div class="main">
//   <h2>Sidebar</h2>
// </div>
// </body>
// <button class="btn success">Success</button>
// <button class="btn info">Info</button>
// <button class="btn warning">Warning</button>
// <button class="btn danger">Danger</button>
// <button class="btn default">Default</button>

const char wifi_body_html[] PROGMEM = R"rawliteral(
  <h2>IoT Web Server </h2><form action="/wifi" method="get" target="_self"><fieldset><div class="container"><label for="oldpass">Wifi Password:</label><input type="password" id="oldpass" name="oldpass" required><br><label for="ssid">New Wifi SSID(Leave Blank for no change):</label><input type="text" id="ssid" name="ssid"><br><label for="paswd">New Wifi Password(Leave Blank for no change):</label><input type="password" id="paswd" name="paswd"><br></div></fieldset><button type="submit" class="regbtn success container">Set New SSID and Password</button></form></body></html>
  )rawliteral";
const char wifi_ap_body_html[] PROGMEM = R"rawliteral(
  <h2>IoT Web Server </h2><form action="/wifi" method="get" target="_self"><fieldset><div class="container"><label for="oldpass">Wifi Password:</label><input type="password" id="oldpass" name="oldpass" required><br><label for="apssid">New SSID for Access Point(Leave Blank for no change):</label><input type="text" id="apssid" name="apssid"><br><label for="appaswd">New Password for Access Point(Leave Blank for no change):</label><input type="password" id="appaswd" name="appaswd"><br><label for="ip">New I.P. for Access Point(Leave Blank for no change)(192.168.4.1):</label><input type="text" id="ip" name="ip"></div></fieldset><button type="submit" class="regbtn success container">Set New Access Point SSID and Access Point Password</button></form></body></html>
  )rawliteral";
const char devices_body_html[] PROGMEM = R"rawliteral(
  <h2>IoT Web Server </h2><form action="/devices" method="get" target="_self"><div class="container"><fieldset><div class="container"><label for="oldpass">Old Wifi Password:</label><input type="password" id="oldpass" name="oldpass" required><br><label for="div1">Device 1 Name(Leave empty for no change) :</label><input type="text" id="div1" name="div1"><br><label for="div2">Device 2 Name:</label><input type="text" id="div2" name="div2"><br><label for="div3">Device 3 Name:</label><input type="text" id="div3" name="div3"><br><label for="div4">Device 4 Name:</label><input type="text" id="div4" name="div4"></div></fieldset><button type="submit" class="regbtn success container">Set Devices Name</button></div></form></body></html>
  )rawliteral";
const char week_body_html[] PROGMEM = R"rawliteral(
  <h2>IoT Web Server : Change Week Days On/Off</h2><form action="/week" method="get" target="_self"><fieldset><div class="container"><label for="oldpass">Old Wifi Password:</label><input type="password" id="oldpass" name="oldpass" required><br><label for="dn">Device Number(1|2|3|4) :</label><input type="number" id="dn" name="dn" min="1" max="4" value="1" step="1" required><br><input type="checkbox" name="0" value="Mon">Mon<br><input type="checkbox" name="1" value="Tue">Tue<br><input type="checkbox" name="2" value="Wed">Wed<br><input type="checkbox" name="3" value="Thu">Thur<br><input type="checkbox" name="4" value="Fri">Fri<br> <input type="checkbox" name="5" value="Sat">Sat<br><input type="checkbox" name="6" value="Sun">Sun<br></div></fieldset><button type="submit" class="regbtn success container">Submit</button></form></body></html>
  )rawliteral";
const char time_body_html[] PROGMEM = R"rawliteral(
  <h2>IoT Web Server : Change Timings of Days On/Off</h2><form action="/time" method="get" target="_self"><fieldset><div class="container"><label for="oldpass">Old Wifi Password:</label><input type="password" id="oldpass" name="oldpass" required><br><label for="dn">Device Number(1|2|3|4) :</label><input type="number" id="dn" name="dn" min="1" max="4" value="1" step="1" required><br><label for="onh">On Hour :</label><input type="number" id="onh" name="onh" min="0" max="23" value="19" step="1" required><label for="onm">On Minute :</label><input type="number" id="onm" name="onm" min="0" max="59" value="20" step="1" required><br><label for="offh">Off Hour :</label><input type="number" id="offh" name="offh"  min="0" max="23" value="7" step="1"  required><br><label for="offm">Off Minute :</label><input type="number" id="offm" name="offm" min="0" max="59" value="30" step="1" required><br><input type="checkbox" name="status" id="status" value="off"><label for="status"> Disable this Device</label><br><input type="checkbox" name="flip" id="flip" value="on"><label for="flip"> Flip this Device</label></div></fieldset><button type="submit" class="regbtn success container">Submit</button></form></body></html>
  )rawliteral";

////////// pages 
void defWebPage(AsyncWebServerRequest *request);
void wifiWebPage(AsyncWebServerRequest *request);
void wifiAPWebPage(AsyncWebServerRequest *request);
void PageDeviesName(AsyncWebServerRequest *request);
void weekpage(AsyncWebServerRequest *request);
void timepage(AsyncWebServerRequest *request);
void commitpage(AsyncWebServerRequest *request);
void aboutpage(AsyncWebServerRequest *request);
void manualonof(AsyncWebServerRequest *request);
void alexa(AsyncWebServerRequest *request);
void local(AsyncWebServerRequest *request);
void manual(AsyncWebServerRequest *request);
void ReStart(AsyncWebServerRequest *request);
void factoryReset(AsyncWebServerRequest *request);
///////////////////////////// helper functions
