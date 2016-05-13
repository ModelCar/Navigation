#include "osrm/route_parameters.hpp"
#include "osrm/table_parameters.hpp"
#include "osrm/nearest_parameters.hpp"
#include "osrm/trip_parameters.hpp"
#include "osrm/match_parameters.hpp"

#include "osrm/coordinate.hpp"
#include "osrm/engine_config.hpp"
#include "osrm/json_container.hpp"

#include "osrm/status.hpp"
#include "osrm/osrm.hpp"

#include <string>
#include <utility>
#include <iostream>
#include <exception>

#include <cstdlib>

int main(int argc, const char *argv[]) try
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " data.osrm\n";
        return EXIT_FAILURE;
    }

    using namespace osrm;

    // Configure based on a .osrm base path, and no datasets in shared mem from osrm-datastore
    EngineConfig config;
    config.storage_config = {argv[1]};
    config.use_shared_memory = false;

    // Routing machine with several services (such as Route, Table, Nearest, Trip, Match)
    OSRM osrm{config};

    // The following shows how to use the Route service; configure this service
    RouteParameters params;

    // Route in monaco
    params.coordinates.push_back({util::FloatLongitude(11.378231048583984), util::FloatLatitude(48.1789071002632)});
    params.coordinates.push_back({util::FloatLongitude(11.715238399999976), util::FloatLatitude(48.1138828)});
    params.steps = true;

    // Response is in JSON format
    json::Object result;

    // Execute routing request, this does the heavy lifting
    const auto status = osrm.Route(params, result);

    if (status == Status::Ok)
    {
        auto &routes = result.values["routes"].get<json::Array>();

        // Let's just use the first route
        auto &route = routes.values.at(0).get<json::Object>();
        const auto distance = route.values["distance"].get<json::Number>().value;
        const auto duration = route.values["duration"].get<json::Number>().value;

        auto &leg = route.values["legs"].get<json::Array>().values.at(0).get<json::Object>();
        auto &steps = leg.values["steps"].get<json::Array>();

        for(int i=0; i < steps.values.size(); i++) {
            auto &step = steps.values.at(i).get<json::Object>();
            auto &maneuver = step.values["maneuver"].get<json::Object>();
            std::cout << maneuver.values["type"].get<json::String>().value << ":" << maneuver.values["modifier"].get<json::String>().value << std::endl;
        }



        // Warn users if extract does not contain the default Berlin coordinates from above
        if (distance == 0 or duration == 0)
        {
            std::cout << "Note: distance or duration is zero. ";
            std::cout << "You are probably doing a query outside of the OSM extract.\n\n";
        }

        std::cout << "Distance: " << distance << " meter\n";
        std::cout << "Duration: " << duration << " seconds\n";
    }
    else if (status == Status::Error)
    {
        const auto code = result.values["code"].get<json::String>().value;
        const auto message = result.values["message"].get<json::String>().value;

        std::cout << "Code: " << code << "\n";
        std::cout << "Message: " << code << "\n";
        return EXIT_FAILURE;
    }
}
catch (const std::exception &e)
{
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
}
