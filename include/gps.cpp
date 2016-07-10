//
// Created by Alex on 13/05/16.
//

#include <json/json.h>
#include "gps.h"

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

//receive location from gpspipe
//the data received in json
coordinates GPS::getCurrentPosition() {
    Json::Value jsonData;
    Json::Reader jsonReader;
    std::string result;

    //to be sure that the coordinate is intercepted
    for(int i=0; i < 5; i++) {
        //result = exec("gpspipe -w -n 6 | grep mode");
        result = exec("node /Users/Alex/Dropbox/Study/LEGOcar/getnextcoordinate.js");
        if(result != "") break;
    }

    jsonReader.parse(result, jsonData);
    //mode 2,3 - the gps receives coordinates
    if(jsonData["mode"].asInt() == 2 || jsonData["mode"].asInt() == 3) {
        coordinates point;
        point.latitude = jsonData["lat"].asDouble();
        point.longitude = jsonData["lon"].asDouble();
        return point;
    } else {
        coordinates point;
        point.latitude = 0;
        point.longitude = 0;
        return point;
    }
}

