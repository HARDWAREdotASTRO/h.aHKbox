#include <Adafruit_ADS1015.h> // Library for using the ADC
#include <Wire.h> // Library for using simple I2C commands for the digi potentiometer

Adafruit_ADS1115 ads; // define sensor for NAU7802 library
byte dev2_address = 44; //0101100, first 5 bits set, last two defined as AD0 adn AD1 values

float V_reading = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  // Serial comm output for troubleshooting
  Wire.begin();//startup I2C channel
  ads.begin(); //start I2C comm with ADC
  dev_setR(0, dev2_address); //set R_ref on Potent. 1

}

void loop() {
  ads.setGain(GAIN_ONE);
  float multiplier = 0.1875F; /* ADS1115  @ +/- 6.144V gain (16-bit results) */
  Serial.println("test");
  byte val = 0;
  int16_t reading;
  for (int i = 0; i < 256; i++) {
    /*
            Read in Voltage Values
    */
    dev_setR(val, dev2_address);
    reading = ads.readADC_Differential_0_1();
    float current = 3.3 / 2000000.0;
    float voltage = reading * multiplier * 0.001;
    float resistance = (voltage / current) - 6000;
    Serial.print(val);
    Serial.print("    ");
    Serial.print(reading);
    Serial.print("    ");
    Serial.print(voltage);
    Serial.print("    ");
    Serial.println(resistance);
    val = val + 10;
    delay(4000);
  }
}


// Function for writing to an I2C address
void dev_setR(byte val, byte dev_address) {
  Wire.beginTransmission(dev_address);
  Wire.write(byte(0x00)); // Fram 2, instruction byte (all low) - refer to data sheet for more info
  Wire.write(val); // Writes the corresponding integer for the desired resistance (frame 3)
  Wire.endTransmission();
}
