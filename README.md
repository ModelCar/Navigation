## How-to run the project

__Clone dependencies__
```
git clone https://github.com/graphhopper/graphhopper.git
```

__Download the map__
```
mkdir map && cd map
wget http://download.geofabrik.de/europe/germany/bayern/oberbayern-latest.osm.pbf
```

__To start the graphhopper__
```
cd graphhopper
./graphhopper.sh web ../map/oberbayern-latest.osm.pbf
```