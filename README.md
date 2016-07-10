# Navigation system documentation

## Introduction


## Requirements
Libraries:
* curl
* jsoncpp

Services:
* gpsd
* graphhopper

## Installation
Installation of all requirements 
```
sudo ./addlibs.sh
```
 
## Usage

### Background services
Run these scripts on the start of Raspberry Pi

Configure gps service
```
sudo ./setupgps.sh
```

Start the graphhopper
```
./rungraphhopper.sh
```

### Navigation service
Add navigation library to your code or compile and run the project

Specify the destination point:
```C
auto nav = new Navigator();
nav->setDestination(coordinates)
```
coordinates is a struct with double fields latitude, longitude.

Get next route step:
```C
nav->getNextStep();
```
return the instruction struct with parameters:
* double bearing - direction (degrees) - compass angle from true north
* double distance (meters) - distance to the next maneuver
* int sign - next maneuver type:

Sign numbers meaning:
* 0: go straight
* 2: trun right
* -2: turn  left
* 4: finish

## Usefull links
1. [GPS module info](http://www.waveshare.com/wiki/UART_GPS_NEO-6M)
2. [GPSD JSON protocol description](http://www.catb.org/gpsd/gpsd_json.html)
3. [Calculation formulas](http://www.movable-type.co.uk/scripts/latlong.html)
4. [Graphhopper api description](https://github.com/graphhopper/graphhopper/blob/master/docs/web/api-doc.md)
5. [Raspberry Pi3 UART GPS module fix](https://www.raspberrypi.org/forums/viewtopic.php?f=44&t=51788)
6. [gpspipe description](http://www.catb.org/gpsd/gpspipe.html)

