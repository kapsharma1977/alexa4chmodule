#ifndef H_RTCDS1307
  #define H_RTCDS1307
  #include "rtcds1307.h"
#endif

void ds1307rtc::initRTC(){
  // Initialize I2C on ESP8266 default pins: D2 (SDA) and D1 (SCL)
  if(_IsI2C()){
    Wire.begin(D2,D1);// SDA=D2 SCL=D1
    Wire.setClock(100000); // 100kHz slower is more reliable
    Wire.setTimeout(500); // Prevents hangs
    delay(100);
    RTC.begin();
    delay(100);
    
    if (!RTC.isrunning()) {
      #if DEBUG
        DBGPRINTLN("RTC is NOT running!");
      #endif
      // following line sets the RTC to the date & time this sketch was compiled
      RTC.adjust(DateTime(__DATE__, __TIME__));
      uday = RTC_NO_DAY; // No RTC
    }
    else{
      printRTC();
      uday = RTC.now().day();
      #if DEBUG
        if(!isAT24C32()) DBGPRINTLN("AT24C32 eeprom is NOT woring on Ds1307!");
      #endif
    }
  }
}
bool ds1307rtc::_IsI2C(){
  //The "Ghost" Device: If every single address (1-127) shows a device found, your SDA/SCL lines are likely shorted or missing pull-up resistors.
  //Missing RTC (0x68): If your scanner finds 0x57 but not 0x68, your RTC module is powered, but the clock chip itself might have a dead battery or loose connection (0x57 is the EEPROM on the module).
  //Timing: Some I2C devices need a moment to "wake up" after power-on. If it fails, try adding delay(100); before Wire.begin().
  Wire.begin(D2,D1);// SDA=D2 SCL=D1
  delay(10);
  //Wire.setClock(100000); // 100kHz slower is more reliable
  Wire.setClock(300000); // 100kHz slower is more reliable
  Wire.setTimeout(500); // Prevents hangs
  
  DBGPRINTLN("\n--- I2C Boot Check ---");
  
  byte error, address;
  int devices = 0;

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission(false); // false = donot release bus
    #if DEBUG
    if (error == 0) {
      DBGPRINT("Found device at: 0x");
      if (address < 16) DBGPRINT("0");
      DBGPRINTLN(address, HEX);
      if(address == DS1307_I2C_ADDRESS) hexadd_clock = address;
      if(address == AT24C32_I2C_ADDRESS || address == 0x57) at24c32_hexadd = address;
      devices++;
    }else if (error==4) {
      DBGPRINTLN("Unknow error at address 0x");
      if (address<16) DBGPRINT("0");
      DBGPRINTLN(address,HEX);
    }
    #else
    if (error == 0) {
      if(address == DS1307_I2C_ADDRESS) hexadd_clock = address;
      if(address == AT24C32_I2C_ADDRESS || address == 0x57) at24c32_hexadd = address;
      devices++;
    }
    #endif
  }
  Wire.endTransmission(true);
  delay(10);
  if (devices == 0) {
    DBGPRINTLN("CRITICAL: No I2C devices found!");
    isRTC_DS1307 = false;
    return false;
    // Optional: while(1); // Stop here if hardware is required
  } else {
    #if DEBUG
    DBG_F("Scan complete. %d device(s) active.\n", devices);
    #endif
    if(isDS1307()){
      isRTC_DS1307 = true;
      return true;
    }else
      return false;
  }
}
bool ds1307rtc::ntpadjustRTC(uint8_t attempts){
  if (! RTC.isrunning()) // update RTC if it is runing
    return false;
  WiFiUDP ntpUDP;
  NTPClient ntpClient = NTPClient(ntpUDP, "in.pool.ntp.org");//"pool.ntp.org"
  ntpClient.setTimeOffset(3600*5+1800);
  ntpClient.begin();
  
  bool flag = ntpClient.update(); // flag = ntpClient.forceUpdate();
  ////uint8_t attempts = 5; //10;
  while(attempts > 0 && !flag){
    flag = ntpClient.update();
    attempts--;
  }
  if(flag){// DBGPRINTLN("ntp fail");return !flag;
    
    time_t epochTime = ntpClient.getEpochTime();
    //DBGPRINT("Epoch Time: ");
    //DBGPRINTLN(epochTime);
    
    String formattedTime = ntpClient.getFormattedTime();
    // DBGPRINT("Formatted Time: ");
    // DBGPRINTLN(formattedTime);  

    
    int currentHour = ntpClient.getHours();
    // DBGPRINT("Hour: ");
    // DBGPRINTLN(currentHour);  

    int currentMinute = ntpClient.getMinutes();
    // DBGPRINT("Minutes: ");
    // DBGPRINTLN(currentMinute); 
    
    int currentSecond = ntpClient.getSeconds();
    // DBGPRINT("Seconds: ");
    // DBGPRINTLN(currentSecond);  

    // String weekDay = weekDays[ntpClient.getDay()];
    // DBGPRINT("Week Day: ");
    // DBGPRINTLN(weekDay);    
    //delay(50);
    //Get a time structure
    struct tm *ptm = gmtime((time_t *)&epochTime);
    
    // int monthDay = ptm->tm_mday;
    // DBGPRINT("Month day: ");
    // DBGPRINTLN(monthDay);

    // int currentMonth = ptm->tm_mon+1;
    // DBGPRINT("Month: ");
    // DBGPRINTLN(currentMonth);

    // String currentMonthName = months[currentMonth-1];
    // DBGPRINT("Month name: ");
    // DBGPRINTLN(currentMonthName);

    //int currentYear = ptm->tm_year+1900;
    // DBGPRINT("Year: ");
    // DBGPRINTLN(currentYear);
    // DBGPRINT("tm_year: ");
    // DBGPRINTLN(ptm->tm_year);

    //Print complete date:
    // String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
    // DBGPRINT("Current date: ");
    // DBGPRINTLN(currentDate);

    DateTime dt = DateTime(ptm->tm_year+1900,ptm->tm_mon,ptm->tm_mday,currentHour,currentMinute,currentSecond); //(year, month, day, hour, min, sec);
    RTC.adjust(dt);
  }
  #if DEBUG
  if (!RTC.isrunning())
    DBGPRINTLN("ntp fail!");
  #endif
  ntpClient.end();
  return flag;
}
void ds1307rtc::checkandupdateRTC(bool connected, uint8_t attempts){ // once in a day
  if (IsUpdateRequired() && connected){
      if(ntpadjustRTC(attempts)){
        adjustntp = false;
        uday = RTC.now().day();
      }
  }
}

String ds1307rtc::ddmmyyyy(uint8_t dd, uint8_t mm,uint16_t yyyy,char seaprator){
  char d[11];
  d[10] = '\0';
  if(dd <= 9)
    sprintf(&d[0],"0%d%c",dd,seaprator, DEC);
  else
    sprintf(&d[0],"%2d%c",dd,seaprator, DEC);
  if(mm <= 9)
    sprintf(&d[3],"0%d%c",mm,seaprator, DEC);
  else
    sprintf(&d[3],"%2d%c",mm,seaprator, DEC);
  sprintf(&d[6],"%d",yyyy,DEC);
  return String(d);
}
String ds1307rtc::hhmmss(uint8_t hh,uint8_t mm,uint8_t ss,char seaprator){//hh:mm:ss
  char t[9];
  t[8] = '\0';
  if(hh <= 9)
    sprintf(&t[0],"0%d%c",hh,seaprator, DEC);
  else
    sprintf(&t[0],"%2d%c",hh,seaprator, DEC);
  if(mm <= 9)
    sprintf(&t[3],"0%d%c",mm,seaprator, DEC);
  else
    sprintf(&t[3],"%2d%c",mm,seaprator, DEC);
  if(ss <= 9)
    sprintf(&t[6],"0%d",ss, DEC);
  else
    sprintf(&t[6],"%2d",ss, DEC);
  return String(t);
}
String ds1307rtc::date(char seaprator){//dd:mm:yyyy
  DateTime now = RTC.now();
  return ddmmyyyy(now.day(), now.month(), now.year());
}
String ds1307rtc::time(char seaprator){//hh:mm:ss
  DateTime now = RTC.now();
  return hhmmss(now.hour(), now.minute(), now.second());
}
void ds1307rtc::printRTC(){
  #if DEBUG
  if (!RTC.isrunning())
    DBGPRINTLN("RTC is NOT running!");
  #endif
  DateTime now = RTC.now();
  
  DBGPRINT(now.year(), DEC);
  DBGPRINT('/');
  DBGPRINT(now.month(), DEC);
  DBGPRINT('/');
  DBGPRINT(now.day(), DEC);
  DBGPRINT(' ');
  DBGPRINT(now.hour(), DEC);
  DBGPRINT(':');
  DBGPRINT(now.minute(), DEC);
  DBGPRINT(':');
  DBGPRINT(now.second(), DEC);
  DBGPRINTLN();
  
  DBGPRINT(" since midnight 1/1/1970 = ");
  DBGPRINT(now.unixtime());
  DBGPRINT("s = ");
  DBGPRINT(now.unixtime() / 86400L);
  DBGPRINTLN("d");
  

  // calculate a date which is 7 days and 30 seconds into the future
  DateTime future (now.unixtime() + 7 * 86400L + 30);
  DBGPRINT(" now + 7d + 30s: ");
  DBGPRINT(future.year(), DEC);
  DBGPRINT('/');
  DBGPRINT(future.month(), DEC);
  DBGPRINT('/');
  DBGPRINT(future.day(), DEC);
  DBGPRINT(' ');
  DBGPRINT(future.hour(), DEC);
  DBGPRINT(':');
  DBGPRINT(future.minute(), DEC);
  DBGPRINT(':');
  DBGPRINT(future.second(), DEC);
  DBGPRINTLN();
  DBGPRINTLN();
}