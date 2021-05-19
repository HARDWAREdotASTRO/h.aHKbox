// Simple DAC sine wave test on Teensy 3.2

float twopi = 3.14159 * 2;
float fidel = 950;
float point;

void setup() {
  analogWriteResolution(12);
}

void loop() {
  point = 0;
  for (int i = 0; i < fidel; i++)
  {
    float val = sin(point * (twopi / fidel)) * 2000.0 + 2050.0;
    analogWrite(A14, (int)val);
    point = point + 1;
    delay(1);
  }
}
