#include <Adafruit_NAU7802.h>

Adafruit_NAU7802 nau;

const int acpin = 23;
int freq = 1;

void setup() {
  pinMode(acpin, OUTPUT);

  Serial.begin(115200);
  while ( !Serial ) ;
  Serial.println("NAU7802");
  if (! nau.begin()) {
    Serial.println("Failed to find NAU7802");
  }
  Serial.println("Found NAU7802");

  nau.setLDO(NAU7802_EXTERNAL);
  while ( !Serial ) ;
  Serial.print("LDO voltage set to ");
  switch (nau.getLDO()) {
    case NAU7802_4V5:  Serial.println("4.5V"); break;
    case NAU7802_4V2:  Serial.println("4.2V"); break;
    case NAU7802_3V9:  Serial.println("3.9V"); break;
    case NAU7802_3V6:  Serial.println("3.6V"); break;
    case NAU7802_3V3:  Serial.println("3.3V"); break;
    case NAU7802_3V0:  Serial.println("3.0V"); break;
    case NAU7802_2V7:  Serial.println("2.7V"); break;
    case NAU7802_2V4:  Serial.println("2.4V"); break;
    case NAU7802_EXTERNAL:  Serial.println("External"); break;
  }

  nau.setGain(NAU7802_GAIN_1);
  Serial.print("Gain set to ");
  switch (nau.getGain()) {
    case NAU7802_GAIN_1:  Serial.println("1x"); break;
    case NAU7802_GAIN_2:  Serial.println("2x"); break;
    case NAU7802_GAIN_4:  Serial.println("4x"); break;
    case NAU7802_GAIN_8:  Serial.println("8x"); break;
    case NAU7802_GAIN_16:  Serial.println("16x"); break;
    case NAU7802_GAIN_32:  Serial.println("32x"); break;
    case NAU7802_GAIN_64:  Serial.println("64x"); break;
    case NAU7802_GAIN_128:  Serial.println("128x"); break;
  }

  nau.setRate(NAU7802_RATE_320SPS);
  Serial.print("Conversion rate set to ");
  switch (nau.getRate()) {
    case NAU7802_RATE_10SPS:  Serial.println("10 SPS"); break;
    case NAU7802_RATE_20SPS:  Serial.println("20 SPS"); break;
    case NAU7802_RATE_40SPS:  Serial.println("40 SPS"); break;
    case NAU7802_RATE_80SPS:  Serial.println("80 SPS"); break;
    case NAU7802_RATE_320SPS:  Serial.println("320 SPS"); break;
  }

  // Take 10 readings to flush out readings
  for (uint8_t i = 0; i < 10; i++) {
    while (! nau.available()) delay(1);
    nau.read();
  }

  while (! nau.calibrate(NAU7802_CALMOD_INTERNAL)) {
    Serial.println("Failed to calibrate internal offset, retrying!");
    delay(1000);
  }
  Serial.println("Calibrated internal offset");

  while (! nau.calibrate(NAU7802_CALMOD_OFFSET)) {
    Serial.println("Failed to calibrate system offset, retrying!");
    delay(1000);
  }
  Serial.println("Calibrated system offset");
  nau.setChannel(NAU7802_CH1);
}

float maxRead;
float minRead;
float VoltResReading;
float I = 0.98 / 973000;
float R;

void loop() {
 /* while (! nau.available()) {
    delay(1);
  }
  int32_t val = nau.read();
  int t = millis();
  Serial.print(t); Serial.print(", ");
  Serial.print((val / 8388608.0) * 2.5, 6); Serial.println(", ");*/

  maxRead = 0.0;
  minRead = 0.0;
  int tend2 = (1.0 / freq) * 1000; //calc period of frequency dependent wave in millis
  int tnow2; //initialize time now variable
  float currentReading;
  float voltReading;
  int t02 = millis(); //set initial time
  while (tnow2 < tend2) {
    sinwave (1.67, 1, acpin, freq, 1.67, 1);
    while (! nau.available()) {
      delay(1);
    }
    currentReading = nau.read();
    voltReading = ((currentReading) / 8388608.0) * 2.5;
    if (voltReading > maxRead) {
      maxRead = voltReading;
    }
    if (voltReading < minRead) {
      minRead = voltReading;
    }
    /*Serial.print("Reading: ");
      Serial.println(currentReading);
      Serial.print("Voltage: ");
      Serial.println(voltReading, 8);*/
    tnow2 = millis() - t02; //set time now for comparing to end time
  }
  Serial.println(); Serial.println("MaxRead: "); Serial.println(maxRead, 8);
  Serial.println("MinRead: "); Serial.println(minRead, 8);
  VoltResReading = (abs(minRead) + maxRead)/2;
  Serial.println(VoltResReading, 8); Serial.println();
  R = VoltResReading / I;
  Serial.println(R);
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
