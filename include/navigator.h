#ifndef NAVIGATION_NAVIGATOR_H
#define NAVIGATION_NAVIGATOR_H

#include <vector>
#include <map>
#include <string>

struct coordinates {
    double latitude;
    double longitude;
};

struct Instruction {
    float distance;
    int sign;
    double bearing;
};

class Navigator {
private:
    coordinates destination;
    coordinates currentPosition;
    Json::Value getJson(std::string);
    coordinates getCurrentPosition();
    std::vector<Instruction> getInstructions(Json::Value);
    std::vector<coordinates> getCoordinates(Json::Value jsonValue);
public:
    Navigator();
    void setDestination(coordinates);
    Json::Value getRoute();
    Instruction getNextStep();
};





#endif //NAVIGATION_NAVIGATOR_H
