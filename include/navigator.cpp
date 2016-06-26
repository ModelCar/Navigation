#include <sstream>
#include <iostream>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <stdio.h>
#include <cmath>

#include "curl/curl.h"
#include "jsoncpp.cpp"

#include "navigator.h"
#include "calculations.cpp"

auto const serverUrl = "http://localhost:8989";

Navigator::Navigator() {
    //start the graphhopper
    //system("./graphhopper.sh web ../map/oberbayern-latest.osm.pbf");
}


void Navigator::setDestination(coordinates destination) {
    this->destination = destination;
}


std::map<std::string, int> const instructions = {
        {"TURN_SHARP_LEFT" , -3},
        {"TURN_LEFT" , -2},
        {"TURN_SLIGHT_LEFT" , -1},
        {"CONTINUE_ON_STREET" , 0},
        {"TURN_SLIGHT_RIGHT" , 1},
        {"TURN_RIGHT" , 2},
        {"TURN_SHARP_RIGHT" , 3},
        {"FINISH" , 4},
        {"VIA_REACHED" , 5},
        {"USE_ROUNDABOUT" , 6}
};

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



Json::Value Navigator::getRoute() {
    this->currentPosition = getCurrentPosition();
    std::string url;
    //generate request url
    url.append(serverUrl).append("/route?point=").append(std::to_string(currentPosition.latitude)).append(",").append(
            std::to_string(currentPosition.longitude));
    url.append("&point=").append(std::to_string(this->destination.latitude)).append(",").append(
            std::to_string(this->destination.longitude)).append("&points_encoded=false");
    return this->getJson(url);
}

coordinates Navigator::getCurrentPosition() {
    //get position from gps
    Json::Value jsonData;
    Json::Reader jsonReader;
    //std::string result = exec("gpspipe -w -n 6 | grep mode");
    std::string result;

    //to be sure that the coordinate is intercepted
    for(int i=0; i < 5; i++) {
        //result = exec("cat /Users/Alex/Dropbox/Study/LEGOcar/navigation/gpspipe.txt");
        result = exec("node /Users/Alex/Dropbox/Study/LEGOcar/getnextcoordinate.js");
        if(result != "") break;
    }

    jsonReader.parse(result, jsonData);
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



std::size_t callback(
        const char *in,
        std::size_t size,
        std::size_t num,
        std::string *out) {
    const std::size_t totalBytes(size * num);
    out->append(in, totalBytes);
    return totalBytes;
}

Json::Value Navigator::getJson(std::string URL) {
    const std::string url(URL);

    CURL *curl = curl_easy_init();

    // Set remote URL.
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Don't bother trying IPv6, which would increase DNS resolution time.
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

    // Don't wait forever, time out after 10 seconds.
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

    // Follow HTTP redirects if necessary.
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Response information.
    int httpCode(0);
    std::unique_ptr<std::string> httpData(new std::string());

    // Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);

    // Hook up data container (will be passed as the last parameter to the
    // callback handling function).  Can be any pointer type, since it will
    // internally be passed as a void pointer.
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

    // Run our HTTP GET command, capture the HTTP response code, and clean up.
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if (httpCode == 200) {
        //std::cout << "\nGot successful response from " << url << std::endl;

        // Response looks good - done using Curl now.  Try to parse the results
        // and print them out.
        Json::Value jsonData;
        Json::Reader jsonReader;

        if (jsonReader.parse(*httpData, jsonData)) {
            std::cout << "Successfully parsed JSON data" << std::endl;
            return jsonData["paths"][0];
        }
        else {
            std::cout << "Could not parse HTTP data as JSON" << std::endl;
            std::cout << "HTTP data was:\n" << *httpData.get() << std::endl;
            return nullptr;
        }
    }

    std::cout << "Couldn't GET from " << url << " - exiting" << std::endl;
    return nullptr;
}

std::vector<Instruction> Navigator::getInstructions(Json::Value jsonValue) {
    Json::Value jsonData = jsonValue["instructions"];
    std::vector<Instruction> instructions;
    int size = jsonData.size() > 5 ? 5 : jsonData.size();
    for ( int index = 0; index < size; index++ ) {
        Instruction instruction;
        instruction.distance = jsonData[index]["distance"].asFloat();
        instruction.sign = jsonData[index]["sign"].asInt();
        instructions.push_back(instruction);
    }
    return instructions;
}

std::vector<coordinates> Navigator::getCoordinates(Json::Value jsonValue) {
    Json::Value jsonData = jsonValue["points"]["coordinates"];
    std::vector<coordinates> points;
    int size = jsonData.size() > 5 ? 5 : jsonData.size();
    for ( int index = 0; index < size; index++ ) {
        coordinates point;
        point.latitude = jsonData[index][1].asDouble();
        point.longitude = jsonData[index][0].asDouble();
        points.push_back(point);
    }
    return points;
}

Instruction Navigator::getNextStep() {
    Json::Value json = getRoute();
    auto points = getCoordinates(json);
    auto instructions = getInstructions(json);

    if(instructions.size() < 2) {
        //this is the last instruction
        return instructions[0];
    }

    Instruction instruction;
    instruction.distance = instructions[0].distance;
    instruction.sign = instructions[1].sign;
    instruction.bearing = CoordinatesToAngle(currentPosition.latitude, currentPosition.longitude, points[1].latitude, points[1].longitude);
    return instruction;
}














