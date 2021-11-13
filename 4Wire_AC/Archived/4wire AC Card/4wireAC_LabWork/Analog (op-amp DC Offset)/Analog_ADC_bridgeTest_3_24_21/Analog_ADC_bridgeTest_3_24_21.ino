

#include <Adafruit_NAU7802.h>

Adafruit_NAU7802 nau;

float twopi = 3.14159 * 2;
float fidel = 950;
float point;

void setup() {
  // put your setup code here, to run once:
  analogWriteResolution(12);

  Serial.begin(115200);
  while ( !Serial ) ;
  Serial.println("NAU7802");
  if (! nau.begin()) {
    Serial.println("Failed to find NAU7802");
  }
  Serial.println("Found NAU7802");

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

void loop() {
  // put your main code here, to run repeatedly:
  point = 0;
  for (int i = 0; i < fidel; i++)
  {
    float valWrite = sin(point * (twopi / fidel)) * 2000.0 + 2050.0;
    analogWrite(A14, (int)valWrite);
    point = point + 1;
    delay(1);
    int32_t valRead = nau.read();
    int t = millis();
    Serial.print(t); Serial.print(", ");
    Serial.print((valRead / 8388608.0) * 2.5, 6); Serial.println(", ");
  }
  while (! nau.available()) {
    delay(1);
  }

}
