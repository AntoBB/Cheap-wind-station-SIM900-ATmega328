# Cheap-wind-station-SIM900-ATmega328
A cheap remote wind station that sends 3G data to a php page. The station is self powered.

![Alt Text](https://i.postimg.cc/Hk6597Xc/anemometer.gif)

This repository contains:
- The Arduino code of the Wind Station, to receive raw data from sensors (anemometer, windVane and temperature sensor), do stuff and pass it (TX->PIN2, RX->PIN3) to SIM900 module that send an HTTP-POST via 3G(or 4G) Mobile Data. 
- A couple of php lines for your server to receive the data of the HTTP POST sent by Arduino(SIM900). There are also a couple of sql query to store the wind speed, direction and temperature in a table of a sql db.

The Station is built as follows:

![Alt Text](https://i.postimg.cc/gJ1C83v2/IMG12.png)

The NRG # 40 Anemometer used here is a simple Hall effect sensor, so has 3-Wire (+ 12V, GND, SENS). Connect the SENS wire to Pin 3 of Arduino.

The Arduino schetch simply calculates the frequency at which the Hall effect sensor rotates by means of the function:

```
//using time between anemometer pulses calculate frequency of anemometer
float getAnemometerFreq(float pTime) { 
  return (1/pTime); 
  }
```
This is the interrupt service routine (ISR) for the anemometer input pin it is called whenever a falling edge is detected:

```
void anemometerISR() {
  unsigned long cTime = millis(); //get current time
  if(!start) { //This is not the first pulse and we are not at 0 MPH so calculate time between pulses
    pulseTime = (float)(cTime - sTime)/1000;
    culPulseTime += pulseTime; //add up pulse time measurements for averaging
    avgWindCount++; //anemomter went around so record for calculating average wind speed
  }
```
