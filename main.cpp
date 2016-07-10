#include <iostream>
#include <vector>

#include "navigator.cpp"

int main(int argc, const char *argv[]) try
{
    //usage
    coordinates destination;
    destination.latitude = 48.267505;
    destination.longitude = 11.672671;

    //create instance
    auto navigator = new Navigator();
    navigator->setDestination(destination);
    for(;;) {
        auto step = navigator->getNextStep();
        std::cout << "Bearing: " << step.bearing << "; Distance: " << step.distance << "; Sign: " << step.sign << std::endl;
        if(step.distance < 5 && step.sign == 4) break;
    }

}
catch (const std::exception &e)
{
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
}
