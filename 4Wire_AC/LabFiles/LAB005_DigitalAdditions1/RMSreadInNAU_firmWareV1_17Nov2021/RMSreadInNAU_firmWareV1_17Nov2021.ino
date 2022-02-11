/*
   4-wire Firmware
   WAG _ Project H.aHK Box _ 4-wire AC Daugter Card
   Version 1.1
   S. Shaw
   2/8/2022

   Outputs a sine wave with user defined frequency to Teensy DAC hardware
   Calculates and outputs an RMS Voltage value at a user defined Read_Out_Rate
*/

// User Inputs
float freq_sin = 10; // Frequency of Sine Wave Modulation (Hz), [1, 160] range allowed given Nyquist sampling rate of 2 samples per period, 320 max SPS of ADC
int Read_Out_Rate = 1; // integer used for Read Out Rate of code, or frequency of an RMS value being reported

// preset/calcuated values
float rms_sum, tim; //rms sum for RMS calculations, tim for outputting sinwave based on time
int frame; //integer used to keep track of what frame in a given window the current reading is
int window_length = 320 / Read_Out_Rate; //integer calculated to provide the # of frames, or readings, that will be used per RMS calculation (SPS/Read Out Rate)
float twopi = 3.14159 * 2; //For sin wave generation
float period = 1 / freq_sin; // Calculate period of sine wave

// Imports ADC library and defines hardware-to-software definition for library
#include <Adafruit_NAU7802.h>
Adafruit_NAU7802 nau;


void setup() {
  analogWriteResolution(12); //Define DAC output pin for sin wave generation
  Serial.begin(115200); //Begin serial comm for debug
  while ( !Serial ) ; //Wait for serial comm to initialize
  // Error handling if ADC not found. Might need to run nauStartupCode or check wiring
  if (! nau.begin()) {
    Serial.println("Failed to find NAU7802");
  }

  // Define ADC settings
  nau.setLDO(NAU7802_EXTERNAL);
  nau.setGain(NAU7802_GAIN_1);
  nau.setRate(NAU7802_RATE_320SPS);
  nau.setChannel(NAU7802_CH1);

  // Take 100 readings to flush out ADC
  for (uint8_t i = 0; i < 100; i++) {
    while (! nau.available()) delay(1);
    nau.read();
  }

  // Do system calibrations
  while (! nau.calibrate(NAU7802_CALMOD_INTERNAL)) {
    Serial.println("Failed to calibrate internal offset, retrying!");
    delay(1000);
  }
  Serial.println("Calibrated internal offset");
  while (! nau.calibrate(NAU7802_CALMOD_OFFSET)) {
    Serial.println("Failed to calibrate system offset, retrying!");
    delay(1000);
  }
  Serial.println("Calibrated system offset");

  // set dynamic variable starting values
  rms_sum = 0;
  frame = 1;
}






void loop() {
  /*
    // prototype code: delay until we are at wave peak
    tim = micros(); //Calculate microseconds
    // If current time is not lined up approximatly at 1/4 the sine wave, or with the first peak of a cycle, delay until alligned
    if ((int(tim) % int(period * 1000000)) > (period * 250100) | (int(tim) % int(period * 1000000)) < (period * 249900)) {
      delayMicroseconds(int(period * 1000000) - (int(tim) % int(period * 1000000)) + period * 250000);
      //Serial.print("Wait!!!!");
    }
    //tim = micros();
    //Serial.println(int(tim) % int(period * 1000000));
  */

  tim = micros() / 1000000.0; //Calculate Seconds
  Sin(freq_sin, tim, period); // Run next sinwave output voltage level based on frequency and time

  if (frame <= window_length)
  {
    if (nau.available() == 1)
    {
      rms_sum = rms_sum + sq(((nau.read() / 8388608.0) * 2.5));
      frame = frame + 1; // Update frame # for next ADC measurement
    }
  }

  else if (frame > window_length) {
    //Calculate final RMS value and output
    float rms_volt = sqrt(rms_sum / window_length);
    Serial.println("RMS_Voltage");
    Serial.println("");
    Serial.println(rms_volt, 6);
    Serial.println("");
    //Serial.print(tim, 7);
    //Serial.println("");

    rms_sum = 0;
    frame = 1; //reinitialize frame integer to 0 for next window of measurements/ next RMS value
  }
  delay(3);
}



// Sine Wave Generator Code function: Uses a table of 1024 sine wave values from [0, 1] as ratios for computing correct DAC output.

void Sin(float freq_m, float tim, float period) {
  //Full resolution is 4095, over 3.3 V -> 0.00080586 V per bit
  // Currently set to 500mV amplitude, 2V offset

  float window_tim = int(tim * 100000) % int(period * 100000); // window time in us
  float sin_values[] = {0.000000, 0.006142, 0.012284, 0.018425, 0.024565, 0.030705, 0.036843, 0.042980, 0.049116, 0.055249, 0.061381, 0.067510, 0.073636, 0.079760, 0.085881, 0.091999, 0.098113,
                        0.104223, 0.110330, 0.116432, 0.122530, 0.128623, 0.134712, 0.140795, 0.146873, 0.152946, 0.159012, 0.165073, 0.171128, 0.177176, 0.183217, 0.189251, 0.195279,
                        0.201299, 0.207311, 0.213316, 0.219312, 0.225300, 0.231280, 0.237251, 0.243213, 0.249166, 0.255110, 0.261043, 0.266967, 0.272881, 0.278785, 0.284678, 0.290561,
                        0.296432, 0.302292, 0.308141, 0.313978, 0.319804, 0.325617, 0.331418, 0.337206, 0.342982, 0.348745, 0.354495, 0.360231, 0.365954, 0.371663, 0.377358, 0.383039,
                        0.388705, 0.394356, 0.399993, 0.405615, 0.411221, 0.416812, 0.422387, 0.427946, 0.433489, 0.439016, 0.444526, 0.450019, 0.455495, 0.460954, 0.466396, 0.471820,
                        0.477227, 0.482615, 0.487985, 0.493337, 0.498670, 0.503985, 0.509280, 0.514556, 0.519813, 0.525050, 0.530267, 0.535464, 0.540641, 0.545798, 0.550934, 0.556050,
                        0.561144, 0.566217, 0.571269, 0.576299, 0.581308, 0.586294, 0.591259, 0.596201, 0.601121, 0.606018, 0.610892, 0.615743, 0.620571, 0.625375, 0.630156, 0.634913,
                        0.639646, 0.644355, 0.649040, 0.653700, 0.658336, 0.662947, 0.667532, 0.672093, 0.676628, 0.681138, 0.685622, 0.690080, 0.694512, 0.698918, 0.703297, 0.707650,
                        0.711977, 0.716276, 0.720548, 0.724794, 0.729012, 0.733202, 0.737365, 0.741500, 0.745607, 0.749685, 0.753736, 0.757758, 0.761751, 0.765716, 0.769652, 0.773559,
                        0.777437, 0.781285, 0.785104, 0.788893, 0.792653, 0.796382, 0.800082, 0.803751, 0.807390, 0.810999, 0.814577, 0.818124, 0.821641, 0.825126, 0.828580, 0.832003,
                        0.835395, 0.838755, 0.842084, 0.845380, 0.848645, 0.851878, 0.855079, 0.858247, 0.861383, 0.864487, 0.867558, 0.870596, 0.873601, 0.876574, 0.879513, 0.882419,
                        0.885292, 0.888132, 0.890938, 0.893710, 0.896449, 0.899154, 0.901825, 0.904462, 0.907064, 0.909633, 0.912167, 0.914667, 0.917133, 0.919563, 0.921959, 0.924321,
                        0.926647, 0.928939, 0.931195, 0.933416, 0.935602, 0.937753, 0.939869, 0.941949, 0.943993, 0.946002, 0.947975, 0.949912, 0.951814, 0.953680, 0.955509, 0.957303,
                        0.959060, 0.960782, 0.962467, 0.964116, 0.965728, 0.967304, 0.968843, 0.970346, 0.971813, 0.973242, 0.974635, 0.975992, 0.977311, 0.978593, 0.979839, 0.981048,
                        0.982219, 0.983354, 0.984451, 0.985512, 0.986535, 0.987521, 0.988469, 0.989381, 0.990255, 0.991092, 0.991891, 0.992653, 0.993377, 0.994064, 0.994714, 0.995326,
                        0.995900, 0.996437, 0.996937, 0.997398, 0.997822, 0.998209, 0.998557, 0.998869, 0.999142, 0.999378, 0.999576, 0.999736, 0.999859, 0.999943, 0.999991, 1.000000,
                        0.999972, 0.999906, 0.999802, 0.999661, 0.999481, 0.999264, 0.999010, 0.998718, 0.998388, 0.998020, 0.997615, 0.997172, 0.996692, 0.996174, 0.995618, 0.995025,
                        0.994394, 0.993726, 0.993020, 0.992277, 0.991496, 0.990678, 0.989823, 0.988930, 0.988000, 0.987032, 0.986028, 0.984986, 0.983907, 0.982791, 0.981638, 0.980448,
                        0.979221, 0.977957, 0.976656, 0.975318, 0.973943, 0.972532, 0.971084, 0.969599, 0.968078, 0.966521, 0.964926, 0.963296, 0.961629, 0.959926, 0.958186, 0.956411,
                        0.954599, 0.952751, 0.950868, 0.948948, 0.946993, 0.945002, 0.942975, 0.940913, 0.938815, 0.936682, 0.934514, 0.932310, 0.930071, 0.927797, 0.925488, 0.923144,
                        0.920766, 0.918352, 0.915904, 0.913422, 0.910905, 0.908353, 0.905767, 0.903147, 0.900494, 0.897806, 0.895084, 0.892328, 0.889539, 0.886716, 0.883860, 0.880970,
                        0.878048, 0.875092, 0.872103, 0.869081, 0.866026, 0.862939, 0.859819, 0.856667, 0.853482, 0.850266, 0.847017, 0.843736, 0.840423, 0.837079, 0.833703, 0.830296,
                        0.826857, 0.823387, 0.819886, 0.816354, 0.812792, 0.809198, 0.805575, 0.801920, 0.798236, 0.794521, 0.790777, 0.787002, 0.783198, 0.779365, 0.775502, 0.771609,
                        0.767688, 0.763737, 0.759758, 0.755750, 0.751714, 0.747649, 0.743557, 0.739436, 0.735287, 0.731110, 0.726906, 0.722674, 0.718416, 0.714130, 0.709817, 0.705477,
                        0.701111, 0.696718, 0.692299, 0.687854, 0.683383, 0.678886, 0.674364, 0.669816, 0.665243, 0.660644, 0.656021, 0.651373, 0.646701, 0.642004, 0.637283, 0.632538,
                        0.627769, 0.622976, 0.618160, 0.613320, 0.608458, 0.603572, 0.598664, 0.593733, 0.588779, 0.583804, 0.578806, 0.573787, 0.568746, 0.563683, 0.558599, 0.553495,
                        0.548369, 0.543222, 0.538055, 0.532868, 0.527661, 0.522434, 0.517187, 0.511920, 0.506635, 0.501330, 0.496006, 0.490664, 0.485303, 0.479923, 0.474526, 0.469111,
                        0.463678, 0.458227, 0.452759, 0.447274, 0.441773, 0.436254, 0.430719, 0.425168, 0.419601, 0.414018, 0.408420, 0.402806, 0.397177, 0.391532, 0.385873, 0.380200,
                        0.374512, 0.368810, 0.363094, 0.357365, 0.351622, 0.345865, 0.340096, 0.334314, 0.328519, 0.322712, 0.316892, 0.311061, 0.305218, 0.299363, 0.293498, 0.287621,
                        0.281733, 0.275834, 0.269926, 0.264007, 0.258078, 0.252139, 0.246191, 0.240233, 0.234267, 0.228291, 0.222307, 0.216315, 0.210314, 0.204306, 0.198290, 0.192266,
                        0.186235, 0.180197, 0.174152, 0.168101, 0.162043, 0.155980, 0.149910, 0.143835, 0.137754, 0.131668, 0.125577, 0.119481, 0.113381, 0.107277, 0.101168, 0.095056,
                        0.088940, 0.082821, 0.076699, 0.070573, 0.064446, 0.058315, 0.052183, 0.046048, 0.039912, 0.033774, 0.027635, 0.021495, 0.015354, 0.009213, 0.003071, -0.003071,
                        -0.009213, -0.015354, -0.021495, -0.027635, -0.033774, -0.039912, -0.046048, -0.052183, -0.058315, -0.064446, -0.070573, -0.076699, -0.082821, -0.088940, -0.095056, -0.101168,
                        -0.107277, -0.113381, -0.119481, -0.125577, -0.131668, -0.137754, -0.143835, -0.149910, -0.155980, -0.162043, -0.168101, -0.174152, -0.180197, -0.186235, -0.192266, -0.198290,
                        -0.204306, -0.210314, -0.216315, -0.222307, -0.228291, -0.234267, -0.240233, -0.246191, -0.252139, -0.258078, -0.264007, -0.269926, -0.275834, -0.281733, -0.287621, -0.293498,
                        -0.299363, -0.305218, -0.311061, -0.316892, -0.322712, -0.328519, -0.334314, -0.340096, -0.345865, -0.351622, -0.357365, -0.363094, -0.368810, -0.374512, -0.380200, -0.385873,
                        -0.391532, -0.397177, -0.402806, -0.408420, -0.414018, -0.419601, -0.425168, -0.430719, -0.436254, -0.441773, -0.447274, -0.452759, -0.458227, -0.463678, -0.469111, -0.474526,
                        -0.479923, -0.485303, -0.490664, -0.496006, -0.501330, -0.506635, -0.511920, -0.517187, -0.522434, -0.527661, -0.532868, -0.538055, -0.543222, -0.548369, -0.553495, -0.558599,
                        -0.563683, -0.568746, -0.573787, -0.578806, -0.583804, -0.588779, -0.593733, -0.598664, -0.603572, -0.608458, -0.613320, -0.618160, -0.622976, -0.627769, -0.632538, -0.637283,
                        -0.642004, -0.646701, -0.651373, -0.656021, -0.660644, -0.665243, -0.669816, -0.674364, -0.678886, -0.683383, -0.687854, -0.692299, -0.696718, -0.701111, -0.705477, -0.709817,
                        -0.714130, -0.718416, -0.722674, -0.726906, -0.731110, -0.735287, -0.739436, -0.743557, -0.747649, -0.751714, -0.755750, -0.759758, -0.763737, -0.767688, -0.771609, -0.775502,
                        -0.779365, -0.783198, -0.787002, -0.790777, -0.794521, -0.798236, -0.801920, -0.805575, -0.809198, -0.812792, -0.816354, -0.819886, -0.823387, -0.826857, -0.830296, -0.833703,
                        -0.837079, -0.840423, -0.843736, -0.847017, -0.850266, -0.853482, -0.856667, -0.859819, -0.862939, -0.866026, -0.869081, -0.872103, -0.875092, -0.878048, -0.880970, -0.883860,
                        -0.886716, -0.889539, -0.892328, -0.895084, -0.897806, -0.900494, -0.903147, -0.905767, -0.908353, -0.910905, -0.913422, -0.915904, -0.918352, -0.920766, -0.923144, -0.925488,
                        -0.927797, -0.930071, -0.932310, -0.934514, -0.936682, -0.938815, -0.940913, -0.942975, -0.945002, -0.946993, -0.948948, -0.950868, -0.952751, -0.954599, -0.956411, -0.958186,
                        -0.959926, -0.961629, -0.963296, -0.964926, -0.966521, -0.968078, -0.969599, -0.971084, -0.972532, -0.973943, -0.975318, -0.976656, -0.977957, -0.979221, -0.980448, -0.981638,
                        -0.982791, -0.983907, -0.984986, -0.986028, -0.987032, -0.988000, -0.988930, -0.989823, -0.990678, -0.991496, -0.992277, -0.993020, -0.993726, -0.994394, -0.995025, -0.995618,
                        -0.996174, -0.996692, -0.997172, -0.997615, -0.998020, -0.998388, -0.998718, -0.999010, -0.999264, -0.999481, -0.999661, -0.999802, -0.999906, -0.999972, -1.000000, -0.999991,
                        -0.999943, -0.999859, -0.999736, -0.999576, -0.999378, -0.999142, -0.998869, -0.998557, -0.998209, -0.997822, -0.997398, -0.996937, -0.996437, -0.995900, -0.995326, -0.994714,
                        -0.994064, -0.993377, -0.992653, -0.991891, -0.991092, -0.990255, -0.989381, -0.988469, -0.987521, -0.986535, -0.985512, -0.984451, -0.983354, -0.982219, -0.981048, -0.979839,
                        -0.978593, -0.977311, -0.975992, -0.974635, -0.973242, -0.971813, -0.970346, -0.968843, -0.967304, -0.965728, -0.964116, -0.962467, -0.960782, -0.959060, -0.957303, -0.955509,
                        -0.953680, -0.951814, -0.949912, -0.947975, -0.946002, -0.943993, -0.941949, -0.939869, -0.937753, -0.935602, -0.933416, -0.931195, -0.928939, -0.926647, -0.924321, -0.921959,
                        -0.919563, -0.917133, -0.914667, -0.912167, -0.909633, -0.907064, -0.904462, -0.901825, -0.899154, -0.896449, -0.893710, -0.890938, -0.888132, -0.885292, -0.882419, -0.879513,
                        -0.876574, -0.873601, -0.870596, -0.867558, -0.864487, -0.861383, -0.858247, -0.855079, -0.851878, -0.848645, -0.845380, -0.842084, -0.838755, -0.835395, -0.832003, -0.828580,
                        -0.825126, -0.821641, -0.818124, -0.814577, -0.810999, -0.807390, -0.803751, -0.800082, -0.796382, -0.792653, -0.788893, -0.785104, -0.781285, -0.777437, -0.773559, -0.769652,
                        -0.765716, -0.761751, -0.757758, -0.753736, -0.749685, -0.745607, -0.741500, -0.737365, -0.733202, -0.729012, -0.724794, -0.720548, -0.716276, -0.711977, -0.707650, -0.703297,
                        -0.698918, -0.694512, -0.690080, -0.685622, -0.681138, -0.676628, -0.672093, -0.667532, -0.662947, -0.658336, -0.653700, -0.649040, -0.644355, -0.639646, -0.634913, -0.630156,
                        -0.625375, -0.620571, -0.615743, -0.610892, -0.606018, -0.601121, -0.596201, -0.591259, -0.586294, -0.581308, -0.576299, -0.571269, -0.566217, -0.561144, -0.556050, -0.550934,
                        -0.545798, -0.540641, -0.535464, -0.530267, -0.525050, -0.519813, -0.514556, -0.509280, -0.503985, -0.498670, -0.493337, -0.487985, -0.482615, -0.477227, -0.471820, -0.466396,
                        -0.460954, -0.455495, -0.450019, -0.444526, -0.439016, -0.433489, -0.427946, -0.422387, -0.416812, -0.411221, -0.405615, -0.399993, -0.394356, -0.388705, -0.383039, -0.377358,
                        -0.371663, -0.365954, -0.360231, -0.354495, -0.348745, -0.342982, -0.337206, -0.331418, -0.325617, -0.319804, -0.313978, -0.308141, -0.302292, -0.296432, -0.290561, -0.284678,
                        -0.278785, -0.272881, -0.266967, -0.261043, -0.255110, -0.249166, -0.243213, -0.237251, -0.231280, -0.225300, -0.219312, -0.213316, -0.207311, -0.201299, -0.195279, -0.189251,
                        -0.183217, -0.177176, -0.171128, -0.165073, -0.159012, -0.152946, -0.146873, -0.140795, -0.134712, -0.128623, -0.122530, -0.116432, -0.110330, -0.104223, -0.098113, -0.091999,
                        -0.085881, -0.079760, -0.073636, -0.067510, -0.061381, -0.055249, -0.049116, -0.042980, -0.036843, -0.030705, -0.024565, -0.018425, -0.012284, -0.006142, 0.000000
                       };

  float valWrite = sin_values[int(1024 * (window_tim / (period * 100000)))]  * 620.0 + 2481.0;
  analogWrite(A14, (int)valWrite);
}
