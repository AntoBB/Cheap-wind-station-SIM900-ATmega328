# Cheap-wind-station-SIM900-ATmega328
A cheap remote wind station that sends 3G data to a php page. The station is self powered.

![Alt Text](https://i.postimg.cc/Hk6597Xc/anemometer.gif)

This repository contains the Arduino code to receive raw data from sensors, prepare and pass it to SIM900 module that send an HTTP POST on 3G-4G Network. The repository also contains a couple of js script and QUERY examples (for backend) to store the HTTP POST sent by Arduino(SIM900) on 3G-4G Network.

The Station is built as follows:

![Alt Text](https://ibb.co/zP3ndT8)