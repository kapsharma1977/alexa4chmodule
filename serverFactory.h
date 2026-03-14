
#ifndef H_MACROS
  #define H_MACROS
  #include "macros.h"
#endif

#ifndef H_ASYNCWEBSERVER
  #define H_ASYNCWEBSERVER
  #include <ESPAsyncWebServer.h>
#endif
#ifndef FAUXMOESP_H
  #define FAUXMOESP_H
  #include "fauxmoESP.h"
#endif

#ifndef H_PAGE
  #define H_PAGE
  #include "page.h"
#endif

class serverFactory{
  fauxmoESP *_fauxmo;
  AsyncWebServer *_localserver;
  public:
    static AsyncWebServer* createlocalserver(AsyncWebServer* _server, uint16_t port = 80){ // Returns a pointer to a new, configured server
      
      if(_server != nullptr){ // check fauxmo server already exsit/created/running
        DBGPRINTLN("AsyncWebServer localserver server for Congiuration is Running");
        return _server;
      }

      // 1. Pre-allocation Safety Check
      if (ESP.getFreeHeap() < 15000) {
          DBGPRINTLN("FATAL: Not enough RAM to create WebServer!");
          return nullptr;
      }
      // 2. Memory Allocation
      AsyncWebServer* server = new AsyncWebServer(port);
      
      // 3. Null Pointer Check (Exception 28 protection)
      if (server == nullptr) {
          DBGPRINTLN("FATAL: Server allocation failed (nullptr)");
          return nullptr;
      }
      // server
      // Route for root / web page
      server->on("/",HTTP_GET,defWebPage);
      server->on("/wifi",HTTP_GET,wifiWebPage);
      server->on("/wifiap",HTTP_GET,wifiAPWebPage); 
      server->on("/devices",HTTP_GET,PageDeviesName);
      // server->on("/week",HTTP_GET,weekpage);
      // server->on("/time",HTTP_GET,timepage);
      server->on("/commit",HTTP_GET,commitpage);
      server->on("/onof",HTTP_GET,manualonof);
      server->on("/alexa",HTTP_GET,alexa);
      server->on("/local",HTTP_GET,local);
      server->on("/manual",HTTP_GET,manual);
      server->on("/about",HTTP_GET,aboutpage);
      server->on("/restart",HTTP_GET,ReStart);
      server->on("/factoryreset",HTTP_GET,factoryReset);
      server->onNotFound(defWebPage);
      // server->onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      //       if (_fauxmo.process(request->client(), request->method() == HTTP_GET, request->url(), String((char *)data))) return;
      //       // Handle any other body request here...
      //   });
      server->begin();
      delay(1000);
      #if DEBUG
        DBG_F("Server created at address: %p on port %u\n", (void*)server, port);
      #endif
      return server;
    }
    static fauxmoESP* createfauxmo(fauxmoESP* faux){
      
      if(faux != nullptr){ // check fauxmo server already exsit/created/running
        DBGPRINTLN("fauxmo server for Alexa is Running");
        return faux;
      }
      
      // 1. Pre-allocation Safety Check
      if (ESP.getFreeHeap() < 15000) {
          DBGPRINTLN("FATAL: Not enough RAM to create fauxmo server! for Alexa");
          return nullptr;
      }

      // 2. Memory Allocation
      fauxmoESP* _faux = new fauxmoESP(); // new fauxmo server
      delay(300);
      #ifdef DEBUG
      (_faux != nullptr) ? DBGPRINTLN("Cretated new fauxmo server for Alexa") : DBGPRINTLN("can not create new fauxmo server for Alexa");
      #endif
      
      
      // 3. Null Pointer Check (Exception 28 protection)
      if (_faux == nullptr) {
          DBGPRINTLN("FATAL: _fauxmo Server allocation failed (nullptr)");
          return nullptr;
      }
      // By default, fauxmoESP creates it's own webserver on the defined port
      // The TCP port must be 80 for gen3 devices (default is 1901)
      // This has to be done before the call to enable()
      _faux->createServer(true); // not needed, this is the default value
      delay(100);
      _faux->setPort(80); // This is required for gen3 devices

      // You have to call enable(true) once you have a WiFi connection
      // You can enable or disable the library at any moment
      // Disabling it will prevent the devices from being discovered and switched
      _faux->Start(); // _fauxmo.enable(true);
      delay(100);
      

      // You can use different ways to invoke alexa to modify the devices state:
      // "Alexa, turn yellow lamp on"
      // "Alexa, turn on yellow lamp
      // "Alexa, set yellow lamp to fifty" (50 means 50% of brightness, note, this example does not use this functionality)
      
      // Add virtual devices. the order they are loaded is important as device_id are assined in sequence (0 1 2 3) and it should not change.
      //DBGPRINT("Number of Devices : ");
      //DBGPRINTLN(_faux->countDevices());
      devwifieeprom.D3device_id = _faux->addDevice(devwifieeprom.D3name);
      devwifieeprom.D4device_id = _faux->addDevice(devwifieeprom.D4name);
      devwifieeprom.D5device_id = _faux->addDevice(devwifieeprom.D5name);
      devwifieeprom.D6device_id = _faux->addDevice(devwifieeprom.D6name);
      //_fauxmo.addDevice(devwifieeprom.D5name);
      DBGPRINT("Number of Devices : ");
      DBGPRINTLN(_faux->countDevices());
      _faux->onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    
        #if DEBUG
        // Callback when a command from Alexa is received. 
        // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
        // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
        // Just remember not to delay too much here, this is a callback, exit as soon as possible.
        // If you have to do something more involved here set a flag and process it in your main loop.
        DBG_F("[MAIN] Device = #%d devwifieeprom.D6device_id = %d (%s) state: %s value: %d", device_id, devwifieeprom.D6device_id, device_name, state ? "ON" : "OFF", value);

        if(devwifieeprom.D3device_id == device_id){
          nodemcu_esp.D_3.newonof = state;
          DBG_F("[D3] Device #%d (%s) state: %s value: %d", device_id, device_name, state ? "ON" : "OFF", value);
        }
        if(devwifieeprom.D4device_id == device_id){
          nodemcu_esp.D_4.newonof = state;
          DBG_F("[D4] Device #%d (%s) state: %s value: %d", device_id, device_name, state ? "ON" : "OFF", value);
        }
        if(devwifieeprom.D5device_id == device_id){
          nodemcu_esp.D_5.newonof = state;
          DBG_F("[D5] Device #%d (%s) state: %s value: %d", device_id, device_name, state ? "ON" : "OFF", value);
        }
        if(devwifieeprom.D6device_id == device_id){
          nodemcu_esp.D_6.newonof = state;
          DBG_F("[D6] Device #%d (%s) state: %s value: %d", device_id, device_name, state ? "ON" : "OFF", value);
        }
        #else
        // Checking for device_id is simpler if you are certain about the order they are loaded and it does not change.
        // Otherwise comparing the device_name is safer.
        if(devwifieeprom.D3device_id == device_id)
          nodemcu_esp.D_3.newonof = state;
        if(devwifieeprom.D4device_id == device_id)
          nodemcu_esp.D_4.newonof = state;
        if(devwifieeprom.D5device_id == device_id)
          nodemcu_esp.D_5.newonof = state;
        if(devwifieeprom.D6device_id == device_id)
          nodemcu_esp.D_6.newonof = state;
        #endif
      });
      return _faux;
    }
    
    serverFactory(){ _fauxmo = nullptr; _localserver = nullptr; }
    ~serverFactory(){ (_fauxmo != nullptr) ? _fauxmo->Stop() : (void)0; delete _fauxmo; _fauxmo == nullptr; (_localserver != nullptr) ? _localserver->end() : (void)0; delete _localserver; _localserver =nullptr;}
    void setlocalserver(AsyncWebServer* p){ _localserver = ((_localserver == nullptr) && (p != nullptr) ) ? p : _localserver; }
    void setfauxmo(fauxmoESP* p){ _fauxmo = ((_fauxmo == nullptr) && (p != nullptr) ) ? p : _fauxmo; }
    
    bool Isfauxmo(){return (_fauxmo != nullptr) ? true : false;}
    bool Isfauxmorunning(){return _fauxmo->Is_server();}
    bool Islocalserver(){return (_localserver != nullptr) ? true : false;}
    void deletelocalserver();
    void deletefauxmo();
    //void setup_localserver();
    //void setup_fauxmo();
    void fauxmoHandel(){if(_fauxmo != nullptr) _fauxmo->handle();}
    AsyncWebServer* getLocalServer(){return (_localserver != nullptr) ? _localserver : nullptr;}
    fauxmoESP* getFauxmo(){ return (_fauxmo != nullptr) ? _fauxmo : nullptr;}
};