#include <Adafruit_ADS1015.h> // Library for using the ADC
#include <Wire.h> // Library for using simple I2C commands for the digi potentiometer
Adafruit_ADS1115 ads; // define sensor for NAU7802 library
byte dev2_address = 44; //0101100, first 5 bits set, last two defined as AD0 adn AD1 values
byte dev3_address = 45; //0101101, first 5 bits set, last two defined as AD0 adn AD1 values
float multiplier = 0.1875F; /* ADS1115  @ +/- 6.144V gain (16-bit results) */
float voltage, current;
int16_t reading;
long resistance, correction;

void setup() {
  pinMode(9, OUTPUT);
  analogWrite(9, 255);
  pinMode(6, OUTPUT);
  analogWrite(6, 190);
  Serial.begin(9600);  // Serial comm output for troubleshooting
  Wire.begin();//startup I2C channel
  ads.begin(); //start I2C comm with ADC
  dev_setR(0, dev2_address); //set R_ref on Potent. 1
  dev_setR(0, dev3_address); //set R_ref on Potent. 1
  //ads.setGain(GAIN_ONE);
}

void loop() {
  // put your main code here, to run repeatedly:
  correction = resistance;
  reading = ads.readADC_Differential_0_1();
  current = 3.2 / (2000060.0 + correction);
  voltage = (reading * multiplier * 0.001);
  resistance = (voltage / current);
  Serial.print(reading);
  Serial.print("    ");
  Serial.print(voltage);
  Serial.print("    ");
  Serial.print(current, 7);
  Serial.print("    ");
  Serial.println(resistance);
  delay(1000);
  if (voltage > 2) {
    resistance = 0;
  }
}

// Function for writing to an I2C address
void dev_setR(byte val, byte dev_address) {
  Wire.beginTransmission(dev_address);
  Wire.write(byte(0x00)); // Fram 2, instruction byte (all low) - refer to data sheet for more info
  Wire.write(val); // Writes the corresponding integer for the desired resistance (frame 3)
  Wire.endTransmission();
  delay(1);
}
