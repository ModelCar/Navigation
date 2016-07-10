#include <sstream>
#include <iostream>
#include <cmath>

#include "curl/curl.h"
#include "jsoncpp.cpp"

#include "navigator.h"
#include "gps.cpp"
#include "calculations.cpp"

auto const serverUrl = "http://localhost:8989";
auto const angleError = 20;//degrees
auto const distanceError = 2;//meters


Navigator::Navigator() {
    //start the graphhopper
    //system("./graphhopper.sh web ../map/oberbayern-latest.osm.pbf");
}


void Navigator::setDestination(coordinates destination) {
    this->destination = destination;
}


//sign explanation dictionary
std::map<std::string, int> Navigator::instructions = {
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

//create an URL for graphopper
Json::Value Navigator::getRouteUrl() {
    std::string url;
    //generate request url
    url.append(serverUrl).append("/route?point=").append(std::to_string(currentPosition.latitude)).append(",").append(
            std::to_string(currentPosition.longitude));
    url.append("&point=").append(std::to_string(this->destination.latitude)).append(",").append(
            std::to_string(this->destination.longitude)).append("&points_encoded=false&instructions=false");
    return this->getJson(url);
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

//parse answer from API
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

//std::vector<Instruction> Navigator::getInstructions(Json::Value jsonValue) {
//    Json::Value jsonData = jsonValue["instructions"];
//    for ( int index = 0; index < jsonData.size(); index++ ) {
//        Instruction instruction;
//        instruction.distance = jsonData[index]["distance"].asFloat();
//        instruction.sign = jsonData[index]["sign"].asInt();
//        instructionList.push_back(instruction);
//    }
//    return instructionList;
//}


//parse JSON and save coordinates to coordinatesList
std::vector<coordinates> Navigator::getCoordinates(Json::Value jsonValue) {
    coordinatesList.clear();
    Json::Value jsonData = jsonValue["points"]["coordinates"];
    for ( int index = 0; index < jsonData.size(); index++ ) {
        coordinates point;
        point.latitude = jsonData[index][1].asDouble();
        point.longitude = jsonData[index][0].asDouble();
        coordinatesList.push_back(point);
    }
    return coordinatesList;
}

//return next step
// sign - maneuver
// bearing - angle from true north
// distance - obvious
Instruction Navigator::getNextStep() {
    this->currentPosition = GPS::getCurrentPosition();
    //problems with uturn, it doesn't work
    int coordIndex = isOnTheRoute(coordinatesList);
    //if cannot find next step in the route
    if(coordIndex == -1) {
        //get new route
        Json::Value json = getRouteUrl();
        getCoordinates(json);
        coordIndex = 1;
    }

    return generateInstruction(coordIndex);
}

//check if car is on the route, and existing route could be used for further navigation
//receive list of coordinates
int Navigator::isOnTheRoute(std::vector<coordinates> points) {
    if(points.size() < 2) return -1;
    //get the nearest point
    for(int i = 0; i < points.size()-1; i++) {
        if(CoordinatesToMeters(currentPosition, points[i+1]) < CoordinatesToMeters(currentPosition, points[i])) {
            continue;
        } else {
            //if too far => recalculate
            if(CoordinatesToMeters(currentPosition, points[i]) > CoordinatesToMeters(points[i], points[i+1])) {
                return -1;
            }
            //points[i] is the closest point
            auto diff = fabs(CoordinatesToAngle(currentPosition, points[i])-CoordinatesToAngle(points[i], points[i+1]));
            //check if they are looking in opposite directions
            //if yes it means that points[i+1] is the next point
            if(180-angleError < diff && diff < 180 + angleError) {
                //TODO: add uturn here
                return i+1;
            } else {
                //most probably points[i] is next point
                if(i > 0) {
                    //check if points[i] is the next point
                    diff = fabs(CoordinatesToAngle(currentPosition, points[i])-CoordinatesToAngle(points[i-1], points[i]));
                    if(0-angleError < diff && diff < angleError) {
                        //points[i] is the next
                        if(CoordinatesToMeters(currentPosition, points[i]) < distanceError) {
                            return i+1;
                        } else {
                            return i;
                        }
                    } else {
                        return -1;
                    }
                }
            }
        }
    }
    return -1;
}

//calculate the difference between two angles
// 0-180 - right
// 180-360 - left
double Navigator::getTurnAngle(double angleCar, double angleNext) {
    auto angle = angleNext + 360 - angleCar;
    if(angle > 360) angle -= 360;
    return angle;
}

//generate instruction from coordinatesList
//receive the pointer of next coordinate in the list
Instruction Navigator::generateInstruction(int coordNum) {
    Instruction newInstruction;
    newInstruction.bearing = CoordinatesToAngle(currentPosition, coordinatesList[coordNum]);
    newInstruction.distance = CoordinatesToMeters(currentPosition, coordinatesList[coordNum]);
    if(coordNum+1 > coordinatesList.size()) {
        newInstruction.sign = 4;
        return newInstruction;
    }
    auto angle = getTurnAngle(newInstruction.bearing, CoordinatesToAngle(coordinatesList[coordNum], coordinatesList[coordNum+1]));
    //turn right
    if(angle > 0 && angle < 180) {
        newInstruction.sign = 2;
    }
    //turn left
    if(angle > 180 && angle < 360) {
        newInstruction.sign = -2;
    }
    //go straight
    if(angle < angleError || angle > 360-angleError) {
        newInstruction.sign = 0;
    }
    return newInstruction;
}





















