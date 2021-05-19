/*
     file for testing SPI communication between cards. Meant as Slave Code and will be added to the 4wire Firmware primary file
      SShaw
      7/23/2020
      v 1.0
*/

#include <SPI.h>

volatile boolean received;
volatile byte Slavereceived, Slavesend;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCK, INPUT);
  pinMode(SS, INPUT);
  SPCR |= _BV(SPE); // turn on SPI in slave mode?e
  received = false;
  SPI.attachInterrupt(); // turn on interrupt, or SPCR |= _BV(SPIE);
}

ISR (SPI_STC_vect) { //interrupt routine
  Slavereceived = SPDR;
  received = true;
}

void loop() {
  // put your main code here, to run repeatedly:
}
