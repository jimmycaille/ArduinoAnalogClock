/* Descr.  : Analog clock using 3x 5v voltmeter gauges
 * Author  : Caille Jimmy
 * Usage   : Enter hh:mm:ss in console to update RTC module
 * Library : https://github.com/NorthernWidget/DS3231 available in lib manager
 * Wiring  :
 *
 * Arduino DS3231 Gauges
 * GND     GND
 * 5V      VCC
 * A4      SDA
 * A5      SCL
 * D9             Hours
 * D10            Minutes
 * D11            Seconds
 *
 * Infos about PWM pins on Arduino Uno, Nano, Mini
 * PINs      : 3, 5, 6, 9, 10, 11
 * Frequency : 490 Hz (pins 5 and 6: 980 Hz)
 * 
 * The PWM outputs generated on pins 5 and 6 will have higher-than-expected duty cycles.
 * This is because of interactions with the millis() and delay() functions,
 * which share the same internal timer used to generate those PWM outputs.
 * This will be noticed mostly on low duty-cycle settings (e.g. 0 - 10)
 * and may result in a value of 0 not fully turning off the output on pins 5 and 6.
 */

#define PIN_H 9
#define PIN_M 10
#define PIN_S 11
#define RTC_UPD 600000//in ms
#define CLK_UPD 1000  //in ms
#define DEBUG false

#include <Wire.h>
#include "DS3231.h"

DS3231 RTC;

byte h,m,s;
unsigned long lastTimeRTC = 0;
unsigned long lastTimeUPD = 0;
bool h12 = false;
bool PM;

void setup() {
  Serial.begin(9600);
  Serial.println(F("START"));
  Wire.begin();

  RTC.setClockMode(h12);
  h = RTC.getHour(h12,PM);
  m = RTC.getMinute();
  s = RTC.getSecond();

  pinMode(PIN_H, OUTPUT);
  pinMode(PIN_M, OUTPUT);
  pinMode(PIN_S, OUTPUT);
}

void loop() {
  if(Serial.available()){
    String input = Serial.readStringUntil("\n");
    h = getValue(input,':',0).toInt();
    m = getValue(input,':',1).toInt();
    s = getValue(input,':',2).toInt();
    RTC.setHour(h);
    RTC.setMinute(m);
    RTC.setSecond(s);
    if(DEBUG) Serial.print(F("TIME UPDATED "));
    if(DEBUG) printTime();
  }
  if(millis() - lastTimeRTC > RTC_UPD){
    lastTimeRTC = millis();
    h = RTC.getHour(h12,PM);
    m = RTC.getMinute();
    s = RTC.getSecond();
  }
  if(millis() - lastTimeUPD > CLK_UPD){
    lastTimeUPD = millis();
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
    if(DEBUG) printTime();
    updateCluster(h,m,s);
  }
}
void printTime(){
  Serial.print(F("Time is : "));
  Serial.print(h);
  Serial.print(":");
  Serial.print(m);
  Serial.print(":");
  Serial.println(s);
}
void updateCluster(int h, int m, int s){
  analogWrite(PIN_H,map(h,0,24,0,255));
  analogWrite(PIN_M,map(m,0,60,0,255));
  analogWrite(PIN_S,map(s,0,60,0,255));
}
//https://arduino.stackexchange.com/questions/1013/how-do-i-split-an-incoming-string
String getValue(String data, char separator, int index){
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
