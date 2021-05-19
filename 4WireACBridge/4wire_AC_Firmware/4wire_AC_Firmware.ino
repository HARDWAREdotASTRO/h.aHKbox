/*
    This code is for a 4-wire AC voltage divider circuit card in taking temperature measurements with Lakeshore millikelvin thermometers.
    Intended for a Teensy 3.2
    Code written by Stephen Shaw 10 July 2020
    Winona Astrophysics Group 2020, H.aHK Box project
    Version: 1.0
*/

/*
   Device Definition
   dev1 : 24-bit ADC
   dev2 : Potentiometer 1 (1M) on channel 1
   dev3 : Potentiometer 2 (1M) on channel 2
   dev4 : Potentiometer 3 (1M) on channel 3
   dev5 : potentiometer 4 (1M) on channel 4
   dev6 : 24-bit ADC 2 (I2C Wire1)
*/

#include <Adafruit_NAU7802.h> // Library for using the ADC
#include <Wire.h> // Library for using simple I2C commands for the digi potentiometer
#include <SPI.h>

String card_id = "4-wire AC 1"; // specify card identity for returning to RPI in startup
#define card_address 2 // address for I2C comm with RPI
Adafruit_NAU7802 nau; // define sensor for NAU7802 library

byte dev1_address = 42; //0101010
byte dev2_address = 44; //0101100, first 5 bits set, last two defined as AD0 adn AD1 values
byte dev3_address = 45; //0101101, first 5 bits set, last two defined as AD0 adn AD1 values
byte dev4_address = 46; //0101110, first 5 bits set, last two defined as AD0 adn AD1 values
byte dev5_address = 47; //0101111, first 5 bits set, last two defined as AD0 adn AD1 values
byte dev6_address = 42; //0101010

byte AD0_1_pin = 0; //define digital write pins for I2C interfacing with potentiometers, programable addresses
byte AD1_1_pin = 1;
byte AD0_2_pin = 2;
byte AD1_2_pin = 3;
byte AD0_3_pin = 7;
byte AD1_3_pin = 8;
byte AD0_4_pin = 9;
byte AD1_4_pin = 11;
byte ADC_ready_1_pin = 4; // define byte data channel for ADC 1 to say when a new reading is available
byte ADC_ready_2_pin = 5; // define byte data channel for ADC 2 to say when a new reading is available
byte reset_pin = 6; //connect channel to reset analog in pin, flipped HIGH if card is shut down or reset from RPI
byte output_pin = 9;

bool card_switch = 1; // Variable for turning on and off card
bool  channel_0 = 1;//Define channel on/off bits for all 4 channels/channels
bool  channel_1 = 1;
bool  channel_2 = 1;
bool  channel_3 = 1;
byte channel_count = 4; // define channel count for determining offset/amp voltage on AC excite
byte temp_range[4] = {3, 3, 3, 3}; // set temperature range to default value (highest temp range)
byte sps_rate = 10; // set sample rate for ADC
int R_level[4]; //value of R_ref [0,256] corresponds to about [1M, 60] ohms
float volt_channelout; // Determines amplitude and offset of AC excitation voltage, changes with different number of channels in operation
long ac_freq = 100; //set ac excitation frequency
long R_assumed[4]; // An assumed value of the sensor resistance at different temp ranges
long R_ref[4]; // Actual value of the reference resistors at different temp ramges (inputed already multiplied by 2)
float V_reading[4]; // Variable for the voltage readings from the ADC's
float R_reading[4]; // Variable for the resistance calculated using the voltage readings from the ADC's (formatted as megaohms so that it fits in the confines of a float)
float current[4]; // Variable for storing current for each channel during calculations

//===================================================

union u_tag { //Function for splitting up a float into 4 bytes
  byte b[4];
  float fval;
} u;

void setup() {
  /*
       Card startup, independent of comm with RPI
  */
  Serial.begin(9600);  // Serial comm output for troubleshooting
  pinMode(ADC_ready_1_pin, INPUT);
  pinMode(output_pin, OUTPUT);
  digitalWrite(AD0_1_pin, LOW); //Define pins for ADC bit I2C addressing
  digitalWrite(AD1_1_pin, LOW);
  digitalWrite(AD0_2_pin, LOW);
  digitalWrite(AD1_2_pin, HIGH);
  digitalWrite(AD0_3_pin, HIGH);
  digitalWrite(AD1_3_pin, LOW);
  digitalWrite(AD0_4_pin, HIGH);
  digitalWrite(AD1_4_pin, HIGH);
  Wire.begin();//startup I2C channel
  nau.begin(); //start I2C comm with ADC
  // Intiialize SPI comm
  //digitalWrite(SS, HIGH);
  //SPI.begin();
  //SPI.setBitOrder(MSBFIRST); // MSB first bit order (Most significant bit)
  //SPI.setClockDivider(SPI_CLOCK_DIV8);
  /*
      Initialize I2C comm with RPI, wait for instructions
  */
  while (card_switch == 0) {
    delay (500); // While card is set to off, continue to idle, interupt from I2C will update card_switch and take this out of loop
  }
  /*
         Initialize I2C on card devices and setup
  */
  dev_setR(R_level[0], dev2_address); //set R_ref on Potent. 1
  dev_setR(R_level[1], dev3_address); //set R_ref on Potent. 2
  dev_setR(R_level[2], dev4_address); //set R_ref on Potent. 3
  dev_setR(R_level[3], dev5_address); //set R_ref on Potent. 4
  adc.rate010sps(); // Decides ADC sample rate
  adc.pga4x();  // Decides ADC gain
}

//function for resetting card
void(* resetFunc)(void) = 0; //declard reset function @ address 0


//====================================================

void loop() {
  /*
          Voltage output value AC excite depndent on channel count operation
  */
  byte channel_count_last;;
  if (channel_count != channel_count_last) {
    channel_count = 0;
    if (channel_0 == 1) {
      channel_count = channel_count + 1;
    }
    if (channel_1 == 1) {
      channel_count ++;
    }
    if (channel_2 == 1) {
      channel_count ++;
    }
    if (channel_3 == 1) {
      channel_count ++;
    }
    channel_count_last = channel_count;
    volt_channelout = (1.67 * channel_count) / 4;
  }
  /*
        Determine Reference resistance based on temp range, comm with chips and set resistance
  */
  for (int i = 0; i < 4; i++) {
    switch (temp_range[i]) {
      case 0 :
        R_ref[i] = 2000000;
        R_level[i] = 255;
        R_assumed[i] = 29075;
        break;
      case 1 :
        R_ref[i] = 200000;
        R_level[i] = 26;
        R_assumed[i] = 8698;
        break;
      case 2 :
        R_ref[i] = 100000;
        R_level[i] = 13;
        R_assumed[i] = 2224;
        break;
      case 3 :
        R_ref[i] = 39062;
        R_level[i] = 5;
        R_assumed[i] = 1209;
        break;
      default :
        R_ref[i] = 2000000;
        R_level[i] = 255;
        R_assumed[i] = 29075;
    }
  }
  dev_setR(R_level[0], dev2_address); //set R_ref on Potent. 1
  dev_setR(R_level[1], dev3_address); //set R_ref on Potent. 2
  dev_setR(R_level[2], dev4_address); //set R_ref on Potent. 3
  dev_setR(R_level[3], dev5_address); //set R_ref on Potent. 4
  /*
        Determine readout rate of SPS, set the readout rate for each ADC chip
  */
  switch (sps_rate) {
    case 10 :
      for (int i = 0; i < 2; i++) {
        // select ADC 1 then second itteration selects ADC 2
        adc.rate010sps();
      }
      break;
    case 20 :
      adc.rate020sps();
      break;
    case 40 :
      adc.rate040sps();
      break;
    case 80 :
      adc.rate080sps();
      break;
    case 320 :
      adc.rate320sps();
      break;
    default :
      adc.rate010sps();
  }
  /*
          AC excitation
  */
  bool wait_test = 0;
  while (wait_test == 0 ) {
    sinwave(volt_channelout, 5, output_pin, ac_freq, volt_channelout);
    if (analogRead(ADC_ready_1_pin) > 1 && analogRead(ADC_ready_2_pin) > 1) {
        wait_test = 1;
    }
  }
  /*
          Read in Voltage Values
  */
  //adc.selectCh1(); //select channel 1
  V_reading[0] = adc.readmV();
  //adc.selectCh2(); //select channel 2
  V_reading[1] = adc.readmV();
  /*
          Convert voltage readings to resistance values of sensors
  */
  for (int i = 0; i < 4; i++) {
    current[i] = 19.8 / (R_ref[i] + R_assumed[i]);
    R_reading[i] = (V_reading[i]) / (current[i] * 1000000.0 * 4.0); // Format as megaohms for use as a float
    SPItransferPID(R_reading[i]);
  }
  /*
          Set the Temp Range depending on resistance values
  */
  for (int i = 0; i < 4; i++) {
    setTempRange(R_reading[i], i);
  }
  /*
        Card switch turned off, reset card
  */
  if (card_switch == 0) { // check if new comm. from RPI comes in with off swtich bit, if so reset card, code will restart and card will idle until bit is flipped to HIGH again in startup code
    resetFunc();//call reset
  }
}

//====================================================

//Function for sending float reading over SPI to PID controller card
void SPItransferPID(float R_reading) {
  u.fval = R_reading;
  for (int j = 0; j < 4; j++) {
    SPI.transfer(u.b[j]);
    delayMicroseconds(20);
  }
}

// Function for writing to an I2C address
void dev_setR(byte val, byte dev_address) {
  Wire.beginTransmission(dev_address);
  Wire.write(byte(0x00)); // Fram 2, instruction byte (all low) - refer to data sheet for more info
  Wire.write(val); // Writes the corresponding integer for the desired resistance (frame 3)
  Wire.endTransmission();
}

// function to map float number with integer scale - courtesy of other developers. Used with sinwave funct.
long mapf(float x, float in_min, float in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// function for generating a sin wave
void sinwave (float offset, const int resolution, const int pin, float frequency, float amplitude) {
  /*
      offset: default value 2.5 volt as operating range voltage is 0~5V
     resolution: this determines the update speed. A lower number means a higher refresh rate.
      pin: outputs sin wave to this pin (currently written for pin 9)
  */
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

//function for setting temprange
void setTempRange(float resistance, int i) {
  if (resistance > 0.00137) {
    temp_range[i] = 2;
    if (resistance > 0.005615) {
      temp_range[i] = 1;
      if (resistance > 0.0194) {
        temp_range[i] = 0;
      }
    }
  }
  else if (resistance < 0.00137) {
    temp_range[i] = 3;
  }
}

//function for sending data over SPI as the master
void sendOverSPI (float data) {
  digitalWrite(SS, LOW); // select device
  SPI.transfer(data);
  digitalWrite(SS, HIGH); //deselect device
}
