#!/bin/bash

clear

#upload libraries
git clone https://github.com/graphhopper/graphhopper.git

#upload map
mkdir map
cd map
wget http://download.geofabrik.de/europe/germany/bayern/oberbayern-latest.osm.pbf

#install curl & gpsd
curl --version || sudo apt-get install libcurl4-openssl-dev
gpsd --version || sudo apt-get install gpsd gpsd-clients
