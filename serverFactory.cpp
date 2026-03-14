
#ifndef SERVERFACTORY_H
  #define SERVERFACTORY_H
  #include "serverFactory.h"
#endif

void serverFactory::deletelocalserver(){
  if(!Islocalserver()) // localserver is not running
    return;
  delete _localserver; // completely delete and remove local webserver from memory 
  _localserver = NULL;
  }
void serverFactory::deletefauxmo(){
  if(!Isfauxmo()) // fauxmo is not running
      return;
  _fauxmo->Stop();
  delete _fauxmo; 
  _fauxmo = NULL;
  }
// void serverFactory::setuplocalserver(){
//   if(Islocalserver()) // localserver is already running
//     return;
//   if(Isfauxmo())
//     deletefauxmo();
//   localserver = new AsyncWebServer(80);
//   // server
//   // Route for root / web page
//   localserver->on("/",HTTP_GET,defWebPage);
//   localserver->on("/wifi",HTTP_GET,wifiWebPage);
//   localserver->on("/wifiap",HTTP_GET,wifiAPWebPage); 
//   localserver->on("/devices",HTTP_GET,PageDeviesName);
//   // localserver->on("/week",HTTP_GET,weekpage);
//   // localserver->on("/time",HTTP_GET,timepage);
//   localserver->on("/commit",HTTP_GET,commitpage);
//   localserver->on("/onof",HTTP_GET,manualonof);
//   localserver->on("/alexa",HTTP_GET,alexa);
//   localserver->on("/local",HTTP_GET,local);
//   localserver->on("/manual",HTTP_GET,manual);
//   localserver->on("/about",HTTP_GET,aboutpage);
//   localserver->on("/restart",HTTP_GET,ReStart);
//   localserver->on("/factoryreset",HTTP_GET,factoryReset);
//   localserver->onNotFound(defWebPage);
//   // localserver->onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
//   //       if (fauxmo.process(request->client(), request->method() == HTTP_GET, request->url(), String((char *)data))) return;
//   //       // Handle any other body request here...
//   //   });
//   localserver->begin();
// }
// void serverFactory::setupfauxmo() {
//   if(Isfauxmo()){ // fauxmo is already running
//     DBGPRINTLN("fauxmo is already running");
//     return;
//   }
//   if(Islocalserver())
//     deletelocalserver();// completely delete and remove local webserver from memory
//   fauxmo = new fauxmoESP();
//   delay(50);
//   if(fauxmo == NULL){
//     DBGPRINTLN("could not allocate memory to fauxmo AsyncServer * _server");
//     return; // could not allocate memory to fauxmo AsyncServer * _server;
//   }
  
  
//   // By default, fauxmoESP creates it's own webserver on the defined port
//   // The TCP port must be 80 for gen3 devices (default is 1901)
//   // This has to be done before the call to enable()
//   fauxmo->createServer(true); // not needed, this is the default value
//   fauxmo->setPort(80); // This is required for gen3 devices

//   // You have to call enable(true) once you have a WiFi connection
//   // You can enable or disable the library at any moment
//   // Disabling it will prevent the devices from being discovered and switched
//   fauxmo->Start(); // fauxmo.enable(true);
  

//   // You can use different ways to invoke alexa to modify the devices state:
//   // "Alexa, turn yellow lamp on"
//   // "Alexa, turn on yellow lamp
//   // "Alexa, set yellow lamp to fifty" (50 means 50% of brightness, note, this example does not use this functionality)
  
//   // Add virtual devices. the order they are loaded is important as device_id are assined in sequence (0 1 2 3) and it should not change.
//   DBGPRINTLN("Number of Devices : %d", fauxmo->countDevices());
//   if(fauxmo->countDevices() <= 0){
//     devwifieeprom.D3device_id = fauxmo->addDevice(devwifieeprom.D3name);
//     devwifieeprom.D4device_id = fauxmo->addDevice(devwifieeprom.D4name);//nodemcu_esp.D_4.getName().c_str()
//     //devwifieeprom.D5device_id = fauxmo->addDevice(devwifieeprom.D5name);
//     //devwifieeprom.D6device_id = fauxmo->addDevice(devwifieeprom.D6name);

//     #if DEBUG == 1
//     fauxmo->printDevices();
//     char buf[LEN];
//     for(uint8_t i = 0; i < LEN; i++)
//       buf[i] = '\0';
//     fauxmo->getDeviceName(devwifieeprom.D3device_id, buf, LEN);
//     DBGPRINTLN("device_id : %d device_name : %s", fauxmo->getDeviceId(devwifieeprom.D3name), buf);
//     for(uint8_t i = 0; i < LEN; i++)
//       buf[i] = '\0';
//     fauxmo->getDeviceName(devwifieeprom.D4device_id, buf, LEN);
//     DBGPRINTLN("device_id : %d device_name : %s", fauxmo->getDeviceId(devwifieeprom.D4name), buf);
//     for(uint8_t i = 0; i < LEN; i++)
//       buf[i] = '\0';
//     fauxmo->getDeviceName(devwifieeprom.D5device_id, buf, LEN);
//     DBGPRINTLN("device_id : %d device_name : %s", fauxmo->getDeviceId(devwifieeprom.D5name), buf);
//     for(uint8_t i = 0; i < LEN; i++)
//       buf[i] = '\0';
//     fauxmo->getDeviceName(devwifieeprom.D6device_id, buf, LEN);
//     DBGPRINTLN("device_id : %d device_name : %s", fauxmo->getDeviceId(devwifieeprom.D6name), buf);

//     #endif
//   }
//   DBGPRINTLN("Number of Devices : %d", fauxmo->countDevices());
  
  
//   // DBGPRINTLN(fauxmo->addDevice(devwifieeprom.D3name));
//   // DBGPRINTLN(fauxmo->addDevice(devwifieeprom.D4name));//nodemcu_esp.D_4.getName().c_str()
//   // DBGPRINTLN(fauxmo->addDevice(devwifieeprom.D5name));
//   // DBGPRINTLN(fauxmo->addDevice(devwifieeprom.D6name));
  
  
//   // DBGPRINTLN(fauxmo->getDeviceId(devwifieeprom.D3name));
//   // DBGPRINTLN(fauxmo->getDeviceId(devwifieeprom.D4name));
//   // DBGPRINTLN(fauxmo->getDeviceId(devwifieeprom.D5name));
//   // DBGPRINTLN(fauxmo->getDeviceId(devwifieeprom.D6name));
  
  

//   fauxmo->onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
      
//       // Callback when a command from Alexa is received. 
//       // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
//       // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
//       // Just remember not to delay too much here, this is a callback, exit as soon as possible.
//       // If you have to do something more involved here set a flag and process it in your main loop.
//       DBGPRINTLN("[MAIN] Device #%d (%s) state: %s value: %d", device_id, device_name, state ? "ON" : "OFF", value);

//       // Checking for device_id is simpler if you are certain about the order they are loaded and it does not change.
//       // Otherwise comparing the device_name is safer.
//       if(devwifieeprom.D3device_id == device_id)
//         nodemcu_esp.D_3.newonof = state;
//       if(devwifieeprom.D4device_id == device_id)
//         nodemcu_esp.D_4.newonof = state;
//       if(devwifieeprom.D5device_id == device_id)
//         nodemcu_esp.D_5.newonof = state;
//       if(devwifieeprom.D6device_id == device_id)
//         nodemcu_esp.D_6.newonof = state;
//   });
//   }