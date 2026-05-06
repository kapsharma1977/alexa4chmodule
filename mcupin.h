#pragma once
#include "macros.h"
class mcuPin{
      // On the ESP8266 (NodeMCU/Wemos D1 Mini), the chip does not have internal pull-down resistors. It only supports internal pull-ups.
          //If you want a Pull-Down configuration on D3 ... D7 D8 , you must use an external resistor 10k. Pin(D7) ----- 10K ----- GND
      
      //Pull-up Strength: The internal pull-up is weak (roughly 10k? to 20k?). If you are driving a power-heavy component like a relay or a long data line, this internal resistor is usually not strong enough; 
      //you should use a physical 4.7k? external resistor to 3.3V instead.
      //Recommendation
          //If your goal is to ensure the pin stays HIGH during the boot process or before your code takes control, 
          //you must use a physical external pull-up resistor to 3.3V. Software commands like pinMode only take effect after your code starts running.
  protected:
    uint8_t _gpio;
    bool _extresistor; // external resistor
    uint8_t _IOmode; // OUTPUT OR INPUT
    bool _pull; // Pin is pulled-up(_pull=true; Active-LOW Logic: HIGH(3.3v) is Off, LOW(GND) is On) or pulld-down(_pull=false; Active-HIGH Logic: HIGH(3.3v) is On, LOW(GND) is Off) //If you want a Pull-Down configuration on D7 (GPIO13), you must use an external resistor 10k. Pin(D7) ----- 10K ----- GND
    volatile bool _active;
    
  public:
    mcuPin(uint8_t gpio, bool extresistor, uint8_t IOmode, uint8_t pull){_gpio = gpio;_extresistor = extresistor; _IOmode = IOmode;_pull = pull;_active=false;}// default pulled up
    void setup(){
      if(_extresistor){ // (4.7k? to 10k?)
        if(_pull){ // pulled up // Active-LOW Logic // (4.7k? to 10k?): Connect one end to the 3.3V pin (VCC) and the other end to the D3 to D7 pin
          if(_IOmode == OUTPUT)
            pinMode(_gpio, OUTPUT);
                // pin configured as an OUTPUT, you are essentially creating an Open-Drain style circuit. 
                    //This is useful for protecting components during boot or for shifting signal levels (e.g., from 3.3V to 5V) with level shifter IC
                    //Load/Target: Connect the device you want to control (like a Transistor base or another IC input) directly to the D3 to D7 pin
                    // How it Functions (Inverted Logic); it operates with inverted logic: 
                    // digitalWrite(D7, LOW): The ESP8266 internally connects the pin to GND. This "overpowers" the resistor, and your output signal becomes 0V.
                    // digitalWrite(D7, HIGH): The ESP8266 drives the pin to 3.3V. Since the resistor is also connected to 3.3V, no current flows through it, and your output signal stays 3.3V
          else
            pinMode(_gpio, INPUT);
                // High-Impedance (Input)
                // Resistor (10krecommended): Connect one end to the 3.3V pin and the other end to the D7 pin.
                // Button/Switch: Connect one terminal of the switch to the D7 pin and the other terminal to the GND (Ground) pin
                // read the state of the pin if(digitalRead(D7) == LOW) Then button clicked
                // Proactive Follow-up: If you find that the button is still giving "ghost" presses, 
                // you might need to add a small 0.1�F capacitor between D7 and GND to hardware debounce the signal
          
          // Explicitly start HIGH to match the pull-up state
          digitalWrite(_gpio, HIGH);
        }
        else{ // pulled down // Active-HIGH Logic // (4.7k? to 10k?): Connect one end to the GND and the other end to the D3 to D8 pin
          if(_IOmode == OUTPUT){
            pinMode(_gpio, OUTPUT);
            //To use an external pull-down resistor on D7 (GPIO13) as an OUTPUT, 
            // you are typically ensuring the connected device (like a Transistor, MOSFET, or LED) stays strictly OFF until the NodeMCU explicitly turns it on
            // In this setup, the resistor ensures the signal stays at 0V (GND) when the pin is not being driven high or is in a high-impedance state (like during reboot).
            // Resistor (4.7k? to 10k?): Connect one end to the D7 pin and the other end to the GND pin.
            // Load/Target: Connect the input of your device (e.g., a Relay module "IN" pin or a Transistor "Base") directly to the D7 pin.
            // Stability: If you are driving a Power MOSFET or an NPN Transistor, 
                // a pull-down is essential to prevent them from "ghost triggering" due to static electricity or during the half-second it takes for the NodeMCU to start your code
            // How it Functions (Active-HIGH Logic)
            // During Boot: Before your code runs, the ESP8266 pins can "float." The 10k? resistor pulls any stray voltage to Ground, keeping your connected device OFF.
            // digitalWrite(D7, HIGH): The ESP8266 drives the pin to 3.3V. This overcomes the weak pull-down resistor, and your output signal becomes 3.3V (ON).
            // digitalWrite(D7, LOW): The ESP8266 internally connects the pin to GND. Both the chip and the resistor are now pulling to 0V, ensuring a solid OFF state.
          }
          else
            pinMode(_gpio, INPUT);
            // To set up D7 (GPIO13) with an external pull-down resistor for input, you create a circuit that holds the pin at 0V (GND) by default. The pin only goes HIGH (3.3V) when you physically press a button or a sensor triggers. 
            // 1. Circuit Connections
            // Resistor (10k?): Connect one end to the D7 pin and the other end to the GND pin.
            // Button/Switch: Connect one terminal to the 3.3V pin and the other terminal to the D7 pin.
            // How it Works (Active-HIGH Logic)
            // Button Open: The 10k? resistor "drains" any stray voltage to Ground. Your code reads digitalRead(D7) as LOW (0).
            // Button Closed: The switch creates a direct path to 3.3V. This path has less resistance than the 10k? path to ground, so the voltage at D7 jumps to 3.3V. Your code reads this as HIGH (1)
            // Simple software debounce delay(200) between two reads
            // If you are getting "flickering" or multiple detections for a single press, use code snippet for advanced software debouncing
          
          // Explicitly start LOW to match the pull-down state
          digitalWrite(_gpio, LOW);
        }
      }else // Internal Pull-up active
        if(_pull){  // Active-LOW Logic
              // If you need a pin to be an Output but also want the Internal Pull-up active
                  // you must set them sequentially
                      // 1. Enable the internal pull-up resistor first
                      // 2. Then switch it to Output mode
            // 1. Enable the internal pull-up resistor first
          pinMode(_gpio, INPUT_PULLUP);
          // 2. Then switch it to Output mode 
          if(_IOmode == OUTPUT) pinMode(_gpio, OUTPUT);
        }else // Pull-Down configuration; no internal pull-down resistor and no external pull-down resistor; floatting Pins, do use in this config 
          (_IOmode == OUTPUT) ?  pinMode(_gpio, OUTPUT) : pinMode(_gpio, INPUT);
      _active=true;
      Off();
    }
    
    void On(){
      if(!_active){
        // _pull is true then Active-LOW Logic: by setting pin to LOW(GND) will be On i.e. at application level logic switch is on; application level logic true
        (_pull) ? digitalWrite(_gpio, LOW) : digitalWrite(_gpio, HIGH);
        // _pull is false then Active-HIGH Logic: by setting pin to HIGH(3.3V) will be On i.e. at application level logic switch is on; application level logic true 
        _active = true;
      }
    }
    void Off(){
      if(_active){
        // _pull is true then Active-LOW Logic: by setting pin to HIGH(3.3v) will be Off i.e. at application level logic switch is off; application level logic false
        (_pull) ? digitalWrite(_gpio, HIGH) : digitalWrite(_gpio, LOW);
        // _pull is false then Active-HIGH Logic: by setting pin to LOW(GND) will be Off i.e. at application level logic switch is off; application level logic false
          _active = false;
      }
    }

    // void activate(){
    //   if(!_active){
    //     (_pull) ? digitalWrite(_gpio, LOW) :digitalWrite(_gpio, HIGH); 
    //     _active = true;
    //   }
    // }
    // void deactivate(){
    //   if(_active){
    //     (_pull) ? digitalWrite(_gpio, HIGH) : digitalWrite(_gpio, LOW); 
    //       _active = false;
    //   }
    // }
    //void toggle(){(_active) ? deactivate() : activate();}
    void toggle(){(_active) ? Off() : On();}
    // Yes, you can read from a NodeMCU (ESP8266) pin even if it is set as an OUTPUT.
    // Voltage State:     digitalRead() on an output pin measures the actual voltage at that moment. If the pin is set to HIGH but is being heavily loaded or shorted to ground, the reading might return LOW because the physical voltage has dropped.
    //Safety Warning:     Be cautious when reading an output pin that is connected to external power or other signals. If an external source forces a different voltage than what the ESP8266 is trying to output, it can cause a short circuit and damage the chip.
    //Logic Verification: Developers often use this technique to verify if an output command (like turning on a relay) was successfully processed by checking the pin's state before performing the next action.
    bool isOn() const { return ( (_pull && (digitalRead(_gpio) == LOW)) || (!_pull && (digitalRead(_gpio) == HIGH)) ) ? true : false;}
    bool isOff() const { return ( (!_pull && (digitalRead(_gpio) == LOW)) || (_pull && (digitalRead(_gpio) == HIGH)) ) ? true : false;}
    //bool isactive() const { return ( (_pull && (digitalRead(_gpio) == LOW)) || (!_pull && (digitalRead(_gpio) == HIGH)) ) ? true : false;}
    //bool isdeactivated() const { return ( (!_pull && (digitalRead(_gpio) == LOW)) || (_pull && (digitalRead(_gpio) == HIGH)) ) ? true : false;}
    bool isPinVoltageHigh(){return (digitalRead(_gpio) == HIGH) ? true : false;}
    bool isPinVoltageLow(){return (digitalRead(_gpio) == LOW) ? true : false;}
    bool status(){return _active;}
    bool isActive_HIGH_Logic(){return (_pull == false) ? true : false;}  // pulld-down(_pull=false; Active-HIGH Logic: HIGH(3.3v) is On, LOW(GND) is Off)
    bool isActive_LOW_Logic(){return (_pull == true) ? true : false;} // Pin is pulled-up(_pull=true; Active-LOW Logic: HIGH(3.3v) is Off, LOW(GND) is On)
    void print(){
      #if DEBUG
      DBGPRINT("MCU Pin : ");
      PrintPin(_gpio);
      (_IOmode == INPUT) ? DBGPRINT(" is receiving digital Input ") : DBGPRINT(" is emitting digital oupput ");
      //DBGPRINT("\t");
      (_pull) ? DBGPRINT(" in pull-up config") : DBGPRINT(" in pull-down config");
      (_extresistor) ? DBGPRINT(" with external resistor ") : DBGPRINT(" with no external resistor ");
      DBGPRINTLN();
      DBGPRINTLN("Check PCB circuit for Pin config (external pull resistor)");
      if(!_extresistor && !_pull){ 
        DBGPRINT("MCU Pin ");
        PrintPin(_gpio);
        DBGPRINTLN(" - Wrong Pin config - Can not be pull-down with out external resistor");
      }
      #endif
    }
};

class PushButton : public mcuPin{
  // button pressed counter
  volatile uint8_t _bpc;
  //volatile unsigned long lastmillis;
  public:
    PushButton(uint8_t gpio, bool extresistor = false,uint8_t IOmode = INPUT, uint8_t pull = true) : mcuPin(gpio,extresistor,INPUT,pull){_bpc=0;}
    // void setup(){
    //   mcuPin::setup();
    //   #if DEBUG
    //     mcuPin::print();
    //   #endif
    // }
    // bool isPressed(){ return ( (_pull && (digitalRead(_gpio) == LOW)) || (!_pull && (digitalRead(_gpio) == HIGH)) ) ? true : false;}
    //bool isPressed(){ return isactive();}
    bool isPressed(){ return isOn();}
    // button pressed counter; zero if button is released
    void operator++(){ _bpc = (_bpc >= 255) ? 0 : ++_bpc; } //prefix ++(obj)
    //void inc_bpc(){_bpc = (_bpc >= 255) ? 0 : ++_bpc;}
    uint8_t get_bpc() const {return _bpc;}
    void clr_bpc(){_bpc=0;}
};

// MCU Status aka D8 pin behaviour 
class mcustatus : public mcuPin{
  // function : Led attached to pin will show status of system(MCU)
  //Purpose: mcu alexa state; led off alexa is connected; blinking then paring mode
  public:
    // config: D8_GPIO15,external resistor 10K true, IOmode = OUTPUT, false(pull-down))
    //mcustatus(uint8_t gpio = D8_GPIO15, bool extresistor = true,uint8_t IOmode = OUTPUT, uint8_t pull = false): mcuPin(gpio,extresistor,OUTPUT,pull){}
    mcustatus(uint8_t gpio, bool extresistor,uint8_t IOmode, uint8_t pull): mcuPin(gpio,extresistor,OUTPUT,pull){}
    //void setup(){mcuPin::setup();}
    void parining(){toggle();} // status led will toggle (blink)
    void AlexaDisConnect(){On();} // status led will glow continuously if alexa is disconneted
    void AlexaConnted(){Off();} // ststus led will switched off if alexa is connected

    // Red LED: 2V 15mA
    // Green LED: 2.1V 20mA
    // Blue LED: 3.2V 25mA
    // While LED: 3.2V 25mA
    // curcuit connection : 5v --- 220ohm --- LED Red(2v,12.5mA) --- GND; and pull-down : D8 ---- 10kohm ---- GND
};
// class D8{
//   // GPIO15 (D8) MUST be LOW; If your external circuit pulls D8 HIGH during power-up, the ESP8266 will enter "SDIO Mode" or "UART Download Mode," and your code will never run
//     // 1. Use it for Active-High Loads: Connect your device (like an LED or a Relay module) so that it only draws power when the pin goes HIGH
//     // 2. Add a Pull-Down Resistor: If your external device has an internal pull-up or is "floating," add a 10kΩ resistor between D8 and GND. This ensures the pin stays LOW during the critical boot sequence
//     // 3. Avoid Certain Modules: Do not connect things that default to "High" or "Pull-up" states on power-on (like some specific sensor data lines or NPN transistor bases without base resistors) directly to D8
//   //Example Wiring for a Relay/LED
//     // Connect: D8 ----- Resistor(10kΩ)/Device ----- GND.
//         //Why: This configuration naturally keeps the pin pulled toward Ground at startup, satisfying the boot requirement
//   private:
//     static const uint8_t _gpio = D8_GPIO15;
//     static const uint8_t _IOmode = OUTPUT; // OUTPUT OR INPUT
//     static const bool _pull = false; // Pin D8 pulld-down(false) ----- Resistor(10kΩ)/Device ----- GND.
//     static const bool _extresistor = true; // external resistor
//     static inline bool _onoff = false;
//     D8(){} //// Private constructor prevents instantiation
//   public:
//     static void setup(){pinMode(_gpio, OUTPUT);digitalWrite(_gpio, LOW);}
//     static void On(){if(!_onoff) {digitalWrite(_gpio, HIGH);_onoff=true;}}// _onoff = true;
//     static void Off(){if(_onoff) {digitalWrite(_gpio, LOW);_onoff=false;}}// _onoff = false;
//     static void toggle(){ (_onoff) ? Off() : On();}
// };