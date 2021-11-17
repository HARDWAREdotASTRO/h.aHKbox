const int acpin = 23;


void setup() {
  // put your setup code here, to run once:
  pinMode(acpin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  sinwave (1.67, 1, acpin, 1, 1.67, 1000);
  //sinwave (2.5, 5, acpin, 1000, 2.5, 1000);
}


// function to map float number with integer scale - courtesy of other developers.
long mapf(float x, float in_min, float in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// function for generating a sin wave
void sinwave (float offset, const int resolution, const int pin, float frequency, float amplitude, int dur) {
  /*
     offset: default value 2.5 volt as operating range voltage is 0~5V
     resolution: this determines the update speed. A lower number means a higher refresh rate.
     pin: outputs sin wave to this pin
     dur: duration of sin wave in milliseconds
  */
  float wav1[3];//0 frequency, 1 unscaled amplitude, 2 is final amplitude
  int average;
  float time;
  float percentage;
  float templitude;
  float minOutputScale = 0.0;
  float maxOutputScale = 3.3;
  const float pi = 3.14159;
  wav1[0] = frequency; //frequency of the sine wave
  wav1[1] = amplitude; // 0V - 2.5V amplitude (Max amplitude + offset) value must not exceed the "maxOutputScale"
  
  int t0 = millis(); //set initial time
  int tend = t0 + dur; //calc when to end, add initial time to duration
  int tnow; //initialize time now variable
  while (tnow < tend) { // continue doing sin wave for duration specificed
    //TCCR1B = TCCR1B & 0b11111000 | 1;//set timer 1B (pin 9) to 31250khz
    time = micros() % 1000000;
    percentage = time / 1000000;
    templitude = sin(((percentage) * wav1[0]) * 2 * pi);
    wav1[2] = (templitude * wav1[1]) + offset; //shift the origin of sinewave with offset.
    average = mapf(wav1[2], minOutputScale, maxOutputScale, 0, 255);
    analogWrite(pin, average);//set output "voltage"
    delayMicroseconds(resolution);//this is to give the micro time to set the "voltage"
    tnow = millis(); //set time now for comparing to end time
  }
}



void pwm(float dur, float power, float per, int in) {
  /*  int dur: duration of the power supply, in ms
      int power: The ratio of power desired, out of 100
      int per: the period of oscillatin between low and high (How long between each "flicker") (in tenth milliseconds)
      int in: the input pin desired for power supply
  */
  for (int k = 0; k < dur ; k++)
  {
    digitalWrite( in, HIGH);
    delayMicroseconds(power * per);
    digitalWrite( in, LOW);
    delayMicroseconds((100 - power) * per);
  }
  return;
}
