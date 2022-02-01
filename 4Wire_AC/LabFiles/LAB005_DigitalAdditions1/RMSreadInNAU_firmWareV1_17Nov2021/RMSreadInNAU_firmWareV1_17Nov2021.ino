/*
   RMS Read in test code
   WAG _ Project H.aHK Box _ 4-wire AC Daugter Card
   Version 1.0
   S. Shaw
   1/30/2022
*/

#include <Adafruit_NAU7802.h>

Adafruit_NAU7802 nau;

float twopi = 3.14159 * 2; //For sin wave generation
float freq_m = 1; // Frequency of Modulation (Hz), [1, 160] range allowed given Nyquist sampling rate of 2 samples per period, 320 max SPS
float rms_sum, tim; //rms sum for RMS calculations, tim for outputting sinwave based on time
int i; //integer used to keep track of data set per read cycle (1 sine wave cycle)
int samps = int (320 / freq_m); //Number of samples per read cycle ((320 samples/sec) / (1/sec)) = samples
float voltRead [320]; // Array of voltage values read per read cycle (set to max readings if frequency is 1 Hz)

void setup() {
  // put your setup code here, to run once:
  analogWriteResolution(12); //Define DAC output pin for sin wave generation

  Serial.begin(115200); //Begin serial comm for debug
  while ( !Serial ) ; //Wait for serial comm to initialize
  // Error handling if ADC not found. Might need to run nauStartupCode or check wiring
  if (! nau.begin()) {
    Serial.println("Failed to find NAU7802");
  }

  // Define ADC settings
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

  // Take 100 readings to flush out ADC
  for (uint8_t i = 0; i < 100; i++) {
    while (! nau.available()) delay(1);
    nau.read();
  }

  // Do system calibrations
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

  //Define variables at starting values for RMS and read cycle looping
  rms_sum = 0;
  i = 0;
}


void loop() {
  tim = millis() / 1000.0; //Calculate Seconds
  Sin(freq_m, tim); // Run next sinwave output voltage level based on frequency and time

  if (nau.available() == 1) //If new data available(1/320 seconds have passed), enter loop
  {
    voltRead[i] = ((nau.read() / 8388608.0) * 2.5); //Read ADC, equation set by hardware. See data sheet for NAU7802
    i = i + 1;
    if (i == samps) //If enought readings (full period, 320 samples), then calculate and send RMS. Reset variables
    {
      for (int k = 0; k < samps; k++) { //Calcuate RMS sum pre-sqrt
        rms_sum = voltRead[k] * voltRead[k] + rms_sum;
        //Serial.println(k);
        //Serial.println("");
        //Serial.println(voltRead[k], 4); Serial.print(",");
        //Serial.println("");
      }
      float rms_volt = sqrt(rms_sum / float(samps));
      Serial.println("RMS_Voltage");
      Serial.println("");
      Serial.println(rms_volt, 6);
      Serial.println("");
      Serial.print(tim, 7);
      Serial.println("");
      rms_sum = 0;
      i = 0;
    }
  }
}

void Sin(float freq_m, float tim) {
  //Full resolution is 4095, over 3.3 V -> 0.00080586 V per bit
  // Currently set to 500mV amplitude, 2V offset
  float valWrite = sin(tim * freq_m * twopi) * 620.0 + 2481.0;
  analogWrite(A14, (int)valWrite);
}
