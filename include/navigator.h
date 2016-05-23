#ifndef NAVIGATION_NAVIGATOR_H
#define NAVIGATION_NAVIGATOR_H


class Navigator {
    std::vector<osrm::Coordinate> coordinateHistory;
    osrm::Coordinate destination;
    osrm::OSRM *osrm;
public:
    Navigator(std::string);
    void getRoute();
    void addDestination(double, double);
    void addCoordinate(double, double);
};


#endif //NAVIGATION_NAVIGATOR_H
