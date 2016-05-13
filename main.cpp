#include "navigator.cpp"

#include <iostream>
#include <exception>


int main(int argc, const char *argv[]) try
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " data.osrm\n";
        return EXIT_FAILURE;
    }

    auto navigator = new Navigator(argv[1]);
    navigator->getRoute();
}
catch (const std::exception &e)
{
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
}
