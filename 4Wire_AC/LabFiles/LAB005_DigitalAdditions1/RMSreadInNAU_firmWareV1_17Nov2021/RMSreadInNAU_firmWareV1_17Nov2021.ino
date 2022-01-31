/*
   RMS Read in test code
   WAG _ Project H.aHK Box _ 4-wire AC Daugter Card
   Version 1.0
   S. Shaw
   1/30/2022
*/

#include <Adafruit_NAU7802.h>

Adafruit_NAU7802 nau;

float twopi = 3.14159 * 2;
float freq_m = 1; // Frequency of Modulation (Hz), [1, 160] range allowed given Nyquist sampling rate of 2 samples per period, 320 SPS
float rms_sum, tim;
int i;
int samps = (320 / freq_m); //Number of samples per frame of period
float voltRead [319]; // Array of voltage values read

void setup() {
  // put your setup code here, to run once:
  analogWriteResolution(12);

  Serial.begin(115200);
  while ( !Serial ) ;
  Serial.println("NAU7802");
  if (! nau.begin()) {
    Serial.println("Failed to find NAU7802");
  }
  // Serial.println("Found NAU7802");

  nau.setLDO(NAU7802_EXTERNAL);
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

  rms_sum = 0;
  i = 0;
}


void loop() {
  // put your main code here, to run repeatedly:
  tim = millis() / 1000.0; //Calculate Microseconds
  Sin(freq_m, tim); //Continue Sin wave output

  if (nau.available() == 1) //If 1/320 seconds have passed, enter loop (new reading available)
  {
    voltRead[i % (samps)] = ((nau.read() / 8388608.0) * 2.5); //Read ADC
    if (i == 320) //If enought readings (full period, 320 samples), then calculate and send RMS. Reset variables
    {
      for (int k = 0; k < samps; k++) { //Calcuate RMS sum pre-sqrt
        rms_sum = voltRead[k] * voltRead[k] + rms_sum;
      }
      i = 0;
      float rms_volt = sqrt(rms_sum/320.0);
      Serial.println("RMS_Voltage");
      Serial.println("");
      Serial.println(rms_volt, 6);
      Serial.println("");
      rms_sum = 0;
    }
    i = i + 1;
  }
}

void Sin(float freq_m, float tim) {
  //Full resolution is 4095, over 3.3 V -> 0.00080586 V per bit
  // Currently set to 500mV amplitude, 2V offset
  float valWrite = sin(tim * freq_m * twopi) * 620.0 + 2481.0;
  analogWrite(A14, (int)valWrite);
}
