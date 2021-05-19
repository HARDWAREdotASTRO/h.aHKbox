/*
   Device Definition
   dev1 : 24-bit ADC
   dev2 : Potentiometer 1 (1M)
   dev3 : Potentiometer 2 (1M)
*/

#include <NAU7802.h>
#include <Wire.h>
//#include <i2c_t3.h> Need a teensy to be hooked up to include this library, although the teensyduino software is installed so should import automatically when teensy is hooked up

NAU7802 adc = NAU7802();

int dev1_address = 0101010; //0101010 or 42
int dev2_address = 0101100; //0101100 or 44, first 5 bits set, last two defined as AD0 adn AD1 values
int dev3_address = 0101101; //0101101 or 45, first 5 bits set, last two defined as AD0 adn AD1 values

int pin_startup[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; //array of pins used for initializing
int AD0_1 = 0; //define digital write pins for I2C interfacing with potentiometers
int AD1_1 = 1;
int AD0_2 = 2;
int AD1_2 = 3;

int R_ref = 1; //value of R_ref [0,256] corresponds to about [1M, 60] ohms

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 30; i++) { //initalized pins used
    pinMode(pin_startup[i], OUTPUT);
  }
  digitalWrite(AD0_1, LOW);
  digitalWrite(AD1_1, LOW);
  digitalWrite(AD0_2, LOW);
  digitalWrite(AD1_2, HIGH);

  Wire.begin();//startup I2C line
  //Wire1.begin(I2C_SLAVE, 0x00, I2C_PINS_26_31,);
  adc.begin(); //startup adc
  //Serial.begin(9600);  // Serial comm output for troubleshooting
}

void loop() {
  // put your main code here, to run repeatedly:
  dev_setR(R_ref, dev2_address); //set R_ref on Potent. 1
  dev_setR(R_ref, dev3_address); //set R_ref on Potent. 2

  adc.rate010sps(); // Decides ADC sample rate
  adc.pga4x();  // Decides ADC gain
  adc.selectCh1();
  adc.readmV();

}

// Function for writing to an I2C address
void dev_setR(int val, int dev_address) {
  Wire.beginTransmission(dev_address);
  Wire.write(0); // Indicates to the RDAC register that the master device is in write mode
  Wire.write(00000000); // Fram 2, instruction byte (all low) - refer to data sheet for more info
  Wire.write(val); // Writes the corresponding integer for the desired resistance (frame 3)
  Wire.endTransmission();
}
