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
    double distance;
    int sign;
    double bearing;
};

class Navigator {
private:
    coordinates destination;
    coordinates currentPosition;
    //std::vector<Instruction> instructionList;
    std::vector<coordinates> coordinatesList;
    Json::Value getJson(std::string);
    //std::vector<Instruction> getInstructions();
    Instruction generateInstruction(int);
    std::vector<coordinates> getCoordinates(Json::Value jsonValue);
    int isOnTheRoute(std::vector<coordinates>);
    double getTurnAngle(double angleCar, double angleNext);
    Json::Value getRouteUrl();
public:
    Navigator();
    void setDestination(coordinates);
    Instruction getNextStep();
    static std::map<std::string, int> instructions;
};





#endif //NAVIGATION_NAVIGATOR_H
