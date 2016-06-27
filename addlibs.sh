#!/bin/bash

clear

#upload libraries
git clone https://github.com/open-source-parsers/jsoncpp.git
git clone https://github.com/graphhopper/graphhopper.git

#upload map
mkdir map
cd map
wget http://download.geofabrik.de/europe/germany/bayern/oberbayern-latest.osm.pbf

#build jsoncpp (needs python)
python --version || echo "Python required"
cd jsoncpp
python amalgamate.py
cd ..
cp -R jsoncpp/dist/ include/
rm -rf jsoncpp

#install curl
curl --version || sudo apt-get install libcurl4-openssl-dev
