void setup() {
  // put your setup code here, to run once:

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
  // put your main code here, to run repeatedly:
  analogWrite(3,i);
  i=i+10%255;
  delay(200);
}
