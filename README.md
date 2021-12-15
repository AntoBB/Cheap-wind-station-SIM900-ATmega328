# Cheap-wind-station-SIM900-ATmega328
A cheap remote wind station that sends 3G data to a php page. The station is self powered.

![Alt Text](https://i.postimg.cc/Hk6597Xc/anemometer.gif)

This repository contains:
- The Arduino code of the Wind Station, to receive raw data from sensors (anemometer, windVane and temperature sensor), do stuff and pass it to SIM900 module that send an HTTP-POST via 3G(or 4G) Mobile Data. 
- A couple of php lines for your server to receive the data of the HTTP POST sent by Arduino(SIM900). There are also a couple of sql query to store the wind speed, direction and temperature in a table of a sql db.

The Station is built as follows:

![Alt Text](https://i.postimg.cc/gJ1C83v2/IMG12.png)