#include <Wire.h> // Library for using simple I2C commands for the digi potentiometer
#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_NAU8702

NAU7802 myScale; //Create instance of the NAU7802 class

byte dev2_address = 44; //0101100, first 5 bits set, last two defined as AD0 adn AD1 values
byte dev3_address = 45; //0101101, first 5 bits set, last two defined as AD0 adn AD1 values

byte ADC_ready_1_pin = A0; // define byte data channel for ADC 1 to say when a new reading is available
float V_reading = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  // Serial comm output for troubleshooting
  pinMode(ADC_ready_1_pin, INPUT);
  Wire.begin();//startup I2C channel
  dev_setR(0, dev2_address); //set R_ref on Potent. 1
  dev_setR(0, dev3_address); //set R_ref on Potent. 2
  if (myScale.begin() == false)
  {
    Serial.println("Scale not detected. Please check wiring. Freezing...");
    while (1);
  }
  Serial.println("Scale detected!");

  myScale.setGain(NAU7802_GAIN_1); //Gain can be set to 1, 2, 4, 8, 16, 32, 64, or 128.

  myScale.setSampleRate(NAU7802_SPS_10); //Sample rate can be set to 10, 20, 40, 80, or 320Hz

  myScale.calibrateAFE(); //Does an internal calibration. Recommended after power up, gain changes, sample rate changes, or channel changes.
}

void loop() {
  Serial.print("test");
  if (myScale.available() == true)
  {
    long currentReading = myScale.getReading();
    Serial.print("Reading: ");
    Serial.println(currentReading);
    Serial.print("Voltage: ");
    V_reading = ((((currentReading) / 8388608.0) * 3.3) - 0.084);
    // put your main code here, to run repeatedly:
    correction = resistance;
    current = 3.2 / (2000060.0 + correction);
    resistance = (V_reading / current);
    Serial.println(V_reading, 8);
    Serial.print("current");
    Serial.println();
    Serial.print("resistance");
    Serial.println();
    Serial.println();
    delay(1000);
  }
}


// Function for writing to an I2C address
void dev_setR(byte val, byte dev_address) {
  Wire.beginTransmission(dev_address);
  Wire.write(byte(0x00)); // Fram 2, instruction byte (all low) - refer to data sheet for more info
  Wire.write(val); // Writes the corresponding integer for the desired resistance (frame 3)
  Wire.endTransmission();
}


// function for generating a sin wave
void sinwave (float offset, const int resolution, const int pin, float frequency, float amplitude) {
  /*
      offset: default value 2.5 volt as operating range voltage is 0~5V
     resolution: this determines the update speed. A lower number means a higher refresh rate.
      pin: outputs sin wave to this pin (currently written for pin 9)
  */
  for (int i = 0; i < 100; i++) {
    float wav1[3];//0 frequency, 1 unscaled amplitude, 2 is final amplitude
    int average;
    float time;
    float percentage;
    float templitude;
    float minOutputScale = 0.0;
    float maxOutputScale = 5.0;
    const float pi = 3.14159;
    wav1[0] = frequency; //frequency of the sine wave
    wav1[1] = amplitude; // 0V - 2.5V amplitude (Max amplitude + offset) value must not exceed the "maxOutputScale"
    //TCCR1B = TCCR1B & 0b11111000 | 1;//set timer 1B (pin 9) to 31250khz
    time = micros() % 1000000;
    percentage = time / 1000000;
    templitude = sin(((percentage) * wav1[0]) * 2 * pi);
    wav1[2] = (templitude * wav1[1]) + offset; //shift the origin of sinewave with offset.
    average = mapf(wav1[2], minOutputScale, maxOutputScale, 0, 255);
    analogWrite(pin, average);//set output "voltage"
    delayMicroseconds(resolution);//this is to give the micro time to set the "voltage"
  }
}


// function to map float number with integer scale - courtesy of other developers. Used with sinwave funct.
long mapf(float x, float in_min, float in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
