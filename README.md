# Navigation system documentation

### Requirements
* curl
* jsoncpp
* gpsd
* graphhopper

### Installation
__Install all with addlibs.sh__
```
sudo bash addlibs.sh
```
 
### Usage

__Run these scripts on the start of Raspberry Pi__

Configure gps
```
sudo ./setupgps.sh
```
 
To start the graphhopper
```
./rungraphhopper.sh
```

__Add navigation library to your code__
__Or compile and run the project__

Specify the destination point:
```
auto nav = new Navigator();
nav->setDestination(coordinates)
```

coordinates is a struct with double fields latitude, longitude.

Get next route step:
```
nav->getNextStep();
```
return the instruction struct with parameters:
* double bearing - direction (degrees) - compass angle from true north
* double distance (meters) - distance to the next maneuver
* int sign - next maneuver type:

_Sign number explanation:_
* 0: drive straight
* 2: trun right
* 0: turn  lest
* 4: finish

### Links
1. [GPS module info](http://www.waveshare.com/wiki/UART_GPS_NEO-6M)
2. [GPSD JSON protocol description](http://www.catb.org/gpsd/gpsd_json.html)
3. [Calculation formulas](http://www.movable-type.co.uk/scripts/latlong.html)
4. [Graphhopper api description](https://github.com/graphhopper/graphhopper/blob/master/docs/web/api-doc.md)
5. [Raspberry Pi3 UART GPS module fix](https://www.raspberrypi.org/forums/viewtopic.php?f=44&t=51788)
6. [gpspipe description](http://www.catb.org/gpsd/gpspipe.html)

