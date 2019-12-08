//install library for 3231 : https://github.com/NorthernWidget/DS3231 available in lib manager

#define PIN_H
#define PIN_M
#define PIN_S
#define RTC_UPD 60000//in ms
#define CLK_UPD 1000//in ms

#include <Wire.h>
#include "DS3231.h"

DS3231 RTC;

byte h,m,s;
unsigned long lastTimeRTC = 0;
unsigned long lastTimeUPD = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
}
/*
 * Uno, Nano, Mini

3, 5, 6, 9, 10, 11

490 Hz (pins 5 and 6: 980 Hz)

Notes and Warnings
The PWM outputs generated on pins 5 and 6 will have higher-than-expected duty cycles. This is because of interactions with the millis() and delay() functions, which share the same internal timer used to generate those PWM outputs. This will be noticed mostly on low duty-cycle settings (e.g. 0 - 10) and may result in a value of 0 not fully turning off the output on pins 5 and 6.
*/
int i=0;
void loop() {
  if(Serial.available()){
    
    RTC.setHour(h);
    RTC.setMinute(m);
    RTC.setSecond(s);
  }
  if(millis() - lastTimeRTC > RTC_UPD){
    lastTimeRTC = millis();
    h = RTC.getHour();
    m = RTC.getMinute();
    s = RTC.getSecond();
    Serial.println(F("rtc updated"));
  }
  if(millis() - lastTimeUPD > CLK_UPD){
    s++;
    if(s==60){
      s=0;
      m++;
    }
    if(m==60){
      m=0;
      h++;
    }
    if(h==24){
      h=0;
    }
  }
  // put your main code here, to run repeatedly:
  analogWrite(3,i);
  i=i+10%255;
  delay(200);
}
