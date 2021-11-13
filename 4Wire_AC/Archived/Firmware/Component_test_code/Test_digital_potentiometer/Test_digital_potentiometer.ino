#include <Wire.h> // Library for using simple I2C commands for the digi potentiometer

byte val = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Wire.begin();
}


void loop() {
  for (int i = 0; i < 255; i++) {
    int readingPos [10];
    int readingNeg [10];
    Wire.beginTransmission(44); // transmit to device #44 (0x2c)
    Wire.write(0);            // sends instruction byte
    Wire.write(val);             // sends potentiometer value byte
    //Wire.endTransmission();     // stop
    for (int i = 0; i < 10; i++) {
      readingPos [i] = analogRead(A0);
      readingNeg [i] = analogRead(A1);
    }
    float averageReadPos = average(readingPos, 10);
    float averageReadNeg = average(readingNeg, 10);
    float voltsPos  = averageReadPos * (3.3 / 1023.0);
    float voltsNeg  = averageReadNeg * (3.3 / 1023.0);
    float current = 3.3 / 2000000.0;
    float resistance  = (voltsPos - voltsNeg) / current;
    Serial.print (voltsPos, 8);
    Serial.print("     ");
    Serial.print (voltsNeg, 8);
    Serial.print("     ");
    Serial.print (voltsPos - voltsNeg, 8);
    Serial.print("     ");
    Serial.print(resistance);
    Serial.print("     ");
    Serial.println(val);
    delay(1000);
    val = val + 10;
  }
}

float average (int array[], int num_reads) {
  int sum = 0;
  int average;
  for (int i = 0; i < num_reads; i++) {
    sum = array[i] + sum;
  }
  average = sum / num_reads;
  return average;
}
