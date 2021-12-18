# Cheap-wind-station-SIM900-ATmega328
A cheap remote wind station that sends data sensors to a php page on mobile data connection (3G-4G). The station is powered by solar energy.

![Alt Text](https://i.postimg.cc/Hk6597Xc/anemometer.gif)

This repository contains:
- The Arduino code of the Wind Station, to receive raw data from sensors (anemometer, windVane and temperature sensor), do stuff and pass it (TX->PIN2, RX->PIN3) to SIM900 module that send an HTTP-POST via 3G(or 4G) Mobile Data. 
- A couple of php lines for your server to receive the data of the HTTP POST sent by Arduino(SIM900). There are also a couple of sql query to store the wind speed, direction and temperature in a table of a sql db.

The Station is built as follows:

![Alt Text](https://i.postimg.cc/gJ1C83v2/IMG12.png)

- The NRG # 40 Anemometer used here is a simple Hall effect sensor, so has 3-Wire (+ 24V, GND, SENS). Connect the SENS wire to Pin 3 of Arduino.

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


- The windvane Mierij Meteo MW36 has a frequency output. In this case we have + 24V, GND and SIGN cable. An analog signal with a frequency proportional to the angle of the flag is output from SIGN. It is connected to the Arduino A0 PIN. In the Arduino sketch this is the function to read from A0:

```
void setup() {
  
  pinMode(WindVanePin, INPUT);
    ...
}

void loop() {
  
  WindVaneDuration = pulseIn(WindVanePin, HIGH);
}

```

- The WINOMO DS18B20 Temperature Sensor is the classic temperature sensor of a few euros on Amazon. It is also powered only with a high Pin of Arduino (3.3V) and the SIGN cable (connected to PIN 2 of Arduino + 3.6k resistor in parallel), it is easily used by importing the libraries:

```
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
OneWire oneWire (ONE_WIRE_BUS);
DallasTemperature sensors (& oneWire);

```
and with the functions:

```
sensors.requestTemperatures ();
```
