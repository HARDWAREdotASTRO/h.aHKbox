<h1>4 Wire AC Expansion Card</h1>

Documentation of all firmware (in C++ for Teensy) and circuit designs for WAG implemented solution to a 4 wire AC expansion card under the H.aHK Box Project.
<br><br>

<h3>Project Background:</h3>

A 4 wire AC temperature measurement circuit is proposed for interface between the H.aHK Box (Hardware.astronomy Housekeeping Box) and the Zeus-2 Spectrometer <sup>a</sup>. Specifically, the Zeus-2 Spectrometer temperature region of interest in application of this circuit operates down to about 100 mK. The Zeus-2 spectrometer uses Lake Shore Cryogenics Interchangeable ROX sensors mainly, but also uses some Germanium GR300 sensors as well <sup>b</sup>.
<br><br>
<h3>Design Specifications</h3>
- System reports temperature value at up to 10 sps <br>
- Obtain resolution of &#177; 10mk in sub-milliKelvin scale and &#177; 1K at higher temperature ranges <br>
- Optimize (minimize) power dissipation across sensor to minimize heat dissipation into low temerature region <br>
- Optimize circuit for low component count <br>
- Digitize read in and output to circuit (frequency output, etc.) <br>
- Optically isolated sensor from digital circuitry <br>
- Operature through +5V and +24V lines <br>
- Interface with H.aHK Box systems and software/firmware communication protocols <br>
<br><br>

<h3>Project Staff</h3>
- Stephen Shaw (General Engineering, Electronics), May 2020 - May 2022

<br><br>
<h3>References</h3>
<sup>a</sup> - https://www.researchgate.net/publication/47351146_ZEUS-2_A_second_generation_submillimeter_grating_spectrometer_for_exploring_distant_galaxies
<br>
<sup>b</sup> - https://www.lakeshore.com/products/categories/temperature-products/cryogenic-temperature-sensors
