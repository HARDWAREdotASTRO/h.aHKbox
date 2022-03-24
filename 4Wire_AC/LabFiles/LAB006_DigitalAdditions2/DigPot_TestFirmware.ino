/*
   Digital Potentiometer Test Firmware
   WAG _ Project H.aHK Box _ 4-wire AC Daugter Card
   Version 1.0
   S. Shaw
   3/23/2022

   Sets resistance of Digital Potentiometer (ADS5241)
   Adapted from AD5241 example code AD524X_write_AD5241 from 
   library by Rob Tillart, URL: https://github.com/RobTillaart/AD524X
   
   
*/

#include "AD524X.h"

AD5241 AD(0x2C);
byte val = 0; // 100 Meg-Ohm Pot, from 0 to 255 value input

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i; i < 256; i++) {
    val = i;
    AD.write(0, val);
    if (val == 200)
    {
      AD.write(1, val, HIGH, LOW);
    }
    if (val == 0)
    {
      AD.write(0, val, LOW, LOW);
    }
    Serial.println(val);
    delay(20);
  }

  //Wire.endTransmission();     // stop
}
