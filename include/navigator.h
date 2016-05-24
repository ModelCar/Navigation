#ifndef NAVIGATION_NAVIGATOR_H
#define NAVIGATION_NAVIGATOR_H


class Navigator {
private:
    std::vector<osrm::Coordinate> coordinateHistory;
    osrm::Coordinate destination;
    osrm::OSRM *osrm;
public:
    Navigator(std::string);
    void getRoute();
    void setDestination(double, double);
    void setCoordinate(double, double);
};


#endif //NAVIGATION_NAVIGATOR_H
