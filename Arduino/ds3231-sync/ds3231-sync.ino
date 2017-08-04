// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setalarm1sec() {
   Wire.beginTransmission(0x68);
   Wire.write(0x0e);
   Wire.endTransmission();
   Wire.requestFrom(0x68, 1);
   uint8_t register0E = Wire.read();

   //register0E |= (1<<6);      // BBSQW - enable square wave out

   // clear bits 3 and 4 for 1Hz
   register0E &= ~(1 << 3);

   register0E |= (1 << 3);

   
   register0E &= ~(1 << 4);


   register0E &= ~(1 << 2); // Disable INTCN so square wave can come out

   // put the value of the register back
   Wire.beginTransmission(0x68);
   Wire.write(0x0e);
   Wire.write(register0E);
   Wire.endTransmission();
}

void set1Hz() {
   // Frequency is stored in register 0x0e in bit 3 and 4
   Wire.beginTransmission(0x68);
   Wire.write(0x0e);
   Wire.endTransmission();
   Wire.requestFrom(0x68, 1);
   uint8_t register0E = Wire.read();

   register0E |= (1<<6);      // BBSQW - enable square wave out

   // clear bits 3 and 4 for 1Hz
   register0E &= ~(1 << 3);
   
   register0E &= ~(1 << 4);


   register0E &= ~(1 << 2); // Disable INTCN so square wave can come out

   // put the value of the register back
   Wire.beginTransmission(0x68);
   Wire.write(0x0e);
   Wire.write(register0E);
   Wire.endTransmission();
}

void setEverySecInt() {

  // Alarm bits A1M4 


   uint8_t register07 = 0b10001111;    // Enable (A1M1), Set alarm 1 match to every second

   // put the value of the register back
   Wire.beginTransmission(0x68);
   Wire.write(0x07);
   Wire.write(register07);
   Wire.endTransmission();


   uint8_t register0E = 0b01000000;    // SQ enabled on VBAT, Square wave enabled with 1hrz

   // put the value of the register back
   Wire.beginTransmission(0x68);
   Wire.write(0x07);
   Wire.write(register07);
   Wire.endTransmission();

  
   // Frequency is stored in register 0x0e in bit 3 and 4
   Wire.beginTransmission(0x68);
   Wire.write(0x0e);
   Wire.endTransmission();
   Wire.requestFrom(0x68, 1);
}



void kill32khz() {
   // Frequency is stored in register 0x0e in bit 3 and 4
   Wire.beginTransmission(0x68);
   Wire.write(0x0f);
   Wire.endTransmission();
   Wire.requestFrom(0x68, 1);
   uint8_t register0f = Wire.read();

   // clear bits 3 and 4 for 1Hz
   register0f &= ~(1 << 3);     // EN32Khz

   // put the value of the register back
   Wire.beginTransmission(0x68);
   Wire.write(0x0f);
   Wire.write(register0f);
   Wire.endTransmission();
 }


void setup () {

      

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);

Serial.println("start");
  

  //delay(3000); // wait for console opening
Serial.println("begin");

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
Serial.println("begined");


  kill32khz();

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  

  set1Hz();

//  setCR();
  if (0 && rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void loop () {

Serial.println("now");
  
    DateTime now = rtc.now();

Serial.println("nowed");
    
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
    
    // calculate a date which is 7 days and 30 seconds into the future
    DateTime future (now + TimeSpan(7,12,30,6));
    
    Serial.print(" now + 7d + 30s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();
    
    Serial.println();
    delay(3000);
}
