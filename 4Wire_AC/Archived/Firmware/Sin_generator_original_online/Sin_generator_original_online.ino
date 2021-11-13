//code modified for improvement from http://forum.arduino.cc/index.php?topic=8563.0
//connect pin 9 -> 10k Ohm + (series with)100nF ceramic cap -> GND, tap the sinewave signal from the point at between the resistor and cap.

void setup() {
  //Serial.begin(115200);//this is for debugging
}

void loop() {
  sinwave(0.75, 5, 23, 100, 0.75);
}

// function to map float number with integer scale - courtesy of other developers.
long mapf(float x, float in_min, float in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// function for generating a sin wave
void sinwave (float offset1, const int resolution1, const int pin1, float frequency, float amplitude) {
    float wav1[3];//0 frequency, 1 unscaled amplitude, 2 is final amplitude
    int average;
    const int Pin = pin1;
    float time;
    float percentage;
    float templitude;
    float offset = offset1; // default value 2.5 volt as operating range voltage is 0~5V
    float minOutputScale = 0.0;
    float maxOutputScale = 5.0;
    const int resolution = resolution1; //this determines the update speed. A lower number means a higher refresh rate.
    const float pi = 3.14159;

    wav1[0] = frequency; //frequency of the sine wave
    wav1[1] = amplitude; // 0V - 2.5V amplitude (Max amplitude + offset) value must not exceed the "maxOutputScale"
    TCCR1B = TCCR1B & 0b11111000 | 1;//set timer 1B (pin 9) to 31250khz

    time = micros() % 1000000;
    percentage = time / 1000000;
    templitude = sin(((percentage) * wav1[0]) * 2 * pi);
    wav1[2] = (templitude * wav1[1]) + offset; //shift the origin of sinewave with offset.
    average = mapf(wav1[2], minOutputScale, maxOutputScale, 0, 255);
    analogWrite(9, average);//set output "voltage"
    delayMicroseconds(resolution);//this is to give the micro time to set the "voltage"
}
