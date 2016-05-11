## How-to run the project

#### Clone osrm-backend
```
    git clone https://github.com/Project-OSRM/osrm-backend.git
```

#### Building OSRM (for MAC)
1. install packages: https://github.com/Project-OSRM/osrm-backend/wiki/Building-OSRM#mac-os-x-1071-1082
2. install stxxl lib: `brew install libstxxl`
3. build: https://github.com/Project-OSRM/osrm-backend/wiki/Building-OSRM#building

#### Generate the map
```
    osrm-extract -p ../profiles/car.lua planet-latest.osm.pbf
```
