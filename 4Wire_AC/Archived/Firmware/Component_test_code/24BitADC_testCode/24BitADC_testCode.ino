#include <Adafruit_NAU7802.h>
#include <Wire.h>

Adafruit_NAU7802 nau;
uint8_t addr = 42;

void setup() {
  pinMode(23, OUTPUT);
  analogWrite(23, 155); //2V rail @155, 1V rail @
  Serial.begin(9600);
  while ( !Serial ) ;
  Wire.begin();
  //nau.begin();
  //nau.setGain(NAU7802_GAIN_4);
  //nau.setRate(NAU7802_RATE_20SPS);
  //nau.calibrate(NAU7802_CALMOD_INTERNAL);
  //nau.calibrate(NAU7802_CALMOD_OFFSET);
  delay(100);
  /*writeDev(addr, 0x00, 00000001);
  writeDev(addr, 0x00, 00000010);
  Serial.println();
  Wire.beginTransmission(addr);
  Wire.write(0x00);
  Wire.endTransmission();
  byte checkin = 0;
  Wire.requestFrom(addr, 1);
  while (Wire.available()) {
    checkin = Wire.read();
  }
  for (int y = 7; y > -1; y--) {
    Serial.print(bitRead(checkin, y));
  }
  Serial.println();
  writeDev(addr, 0x00, 0xAE);
  writeDev(addr, 0x15, 0x30);
  */
}

void loop() {
  Wire.beginTransmission(addr);
  Wire.write(byte(0x12));
  Wire.endTransmission();
  Wire.requestFrom(addr, 3);
  int i = 0;
  byte data_in[3] = {0, 0, 0};
  while (Wire.available()) {
    data_in[i] = Wire.read();
    i++;
  }
  for (int k = 0; k < 3; k++) {
    Serial.println(data_in[k]);
  }
  Serial.println();
  unsigned long value = data_in[0];
  value = value * 256 + data_in[1];
  value = value * 256 + data_in[2];
  float Volt = (value / 16777216.0) * 2;
  Serial.println(value, 8);
  Serial.println();
  Serial.println(Volt, 8);
  Serial.println();
  //Serial.println(nau.read());
  //Serial.println(nau.getGain());
  delay(1000);
}

void writeDev(uint8_t i2cAddr, uint8_t ctlByte, uint8_t data)
{
  Wire.beginTransmission(i2cAddr);
  Wire.write(ctlByte);
  Wire.write(data);
  Wire.endTransmission();
}
