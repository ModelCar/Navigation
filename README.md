## How-to run the project

__Clone osrm-backend__
```
git clone https://github.com/Project-OSRM/osrm-backend.git -b 5.0
```
_Tested in 5.0. Should work in other versions._

__Build OSRM (for MAC)__

1. install packages: https://github.com/Project-OSRM/osrm-backend/wiki/Building-OSRM#mac-os-x-1071-1082
2. install stxxl lib: `brew install libstxxl`
3. build: https://github.com/Project-OSRM/osrm-backend/wiki/Building-OSRM#building

__Download & extract the map__
```
mkdir map && cd map
wget http://download.geofabrik.de/europe/germany/bayern/oberbayern-latest.osm.pbf
osrm-extract -p ../profiles/car.lua oberbayern-latest.osm.pbf
osrm-contract oberbayern-latest.osrm
```
