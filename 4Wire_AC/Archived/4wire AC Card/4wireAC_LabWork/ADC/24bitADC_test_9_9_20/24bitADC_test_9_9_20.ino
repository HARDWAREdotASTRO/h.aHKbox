/*
   Testing code for ADC measuring resistance bridge (across middle resistors, 3 1K resistors in series) with AC modulation from AD2

   9/9/20
   SShaw

*/

#include <Wire.h>

#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_NAU8702

NAU7802 myScale; //Create instance of the NAU7802 class

const int acpin = 23;

void setup() {
  pinMode(acpin, OUTPUT);

  //analogWrite(23, 78); //2V rail @155, 3V rail @233, 1V @ 77
  Serial.begin(9600);
  while ( !Serial ) ;
  Serial.println("Qwiic Scale Example");

  Wire.begin();

  if (myScale.begin() == false)
  {
    Serial.println("Scale not detected. Please check wiring. Freezing...");
    while (1);
  }
  Serial.println("Scale detected!");

  myScale.setGain(NAU7802_GAIN_1); //Gain can be set to 1, 2, 4, 8, 16, 32, 64, or 128.

  myScale.setSampleRate(NAU7802_SPS_320); //Sample rate can be set to 10, 20, 40, 80, or 320Hz

  myScale.calibrateAFE(); //Does an internal calibration. Recommended after power up, gain changes, sample rate changes, or channel changes.


}

void loop()
{
  sinwave (1.67, 1, acpin, 1, 1.67, 1);
  if (myScale.available() == true)
  {
    long currentReading = myScale.getReading();
    Serial.print("Reading: ");
    Serial.println(currentReading);
    Serial.print("Voltage: ");
    Serial.println(((((currentReading) / 8388608.0) * 2.5)), 8);
  }
}


// function for generating a sin wave
void sinwave (float offset, const int resolution, const int pin, float frequency, float amplitude, int dur) {
  /*
     offset: default value 2.5 volt as operating range voltage is 0~5V
     resolution: this determines the update speed. A lower number means a higher refresh rate.
     pin: outputs sin wave to this pin
     dur: duration of sin wave in milliseconds
  */
  float wav1[3];//0 frequency, 1 unscaled amplitude, 2 is final amplitude
  int average;
  float time;
  float percentage;
  float templitude;
  float minOutputScale = 0.0;
  float maxOutputScale = 3.3;
  const float pi = 3.14159;
  wav1[0] = frequency; //frequency of the sine wave
  wav1[1] = amplitude; // 0V - 2.5V amplitude (Max amplitude + offset) value must not exceed the "maxOutputScale"

  int t0 = millis(); //set initial time
  int tend = t0 + dur; //calc when to end, add initial time to duration
  int tnow; //initialize time now variable
  while (tnow < tend) { // continue doing sin wave for duration specificed
    //TCCR1B = TCCR1B & 0b11111000 | 1;//set timer 1B (pin 9) to 31250khz
    time = micros() % 1000000;
    percentage = time / 1000000;
    templitude = sin(((percentage) * wav1[0]) * 2 * pi);
    wav1[2] = (templitude * wav1[1]) + offset; //shift the origin of sinewave with offset.
    average = mapf(wav1[2], minOutputScale, maxOutputScale, 0, 255);
    analogWrite(pin, average);//set output "voltage"
    delayMicroseconds(resolution);//this is to give the micro time to set the "voltage"
    tnow = millis(); //set time now for comparing to end time
  }
}


// function to map float number with integer scale - courtesy of other developers. Used with sinwave funct.
long mapf(float x, float in_min, float in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
