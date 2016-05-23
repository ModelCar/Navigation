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

#include "navigator.h"

Navigator::Navigator(std::string mapPath) {

    // Configure based on a .osrm base path, and no datasets in shared mem from osrm-datastore
    osrm::EngineConfig config;
    config.storage_config = {mapPath};
    config.use_shared_memory = false;

    // Routing machine with several services (such as Route, Table, Nearest, Trip, Match)
    Navigator::osrm = new osrm::OSRM{config};
}

void Navigator::getRoute() {
    // The following shows how to use the Route service; configure this service
    osrm::RouteParameters params;

    // Route in monaco
    //TODO: check destination & history not empty
    params.coordinates.push_back(coordinateHistory.back());
    params.coordinates.push_back(destination);
    params.steps = true;

    // Response is in JSON format
    osrm::json::Object result;

    // Execute routing request, this does the heavy lifting
    const auto status = osrm->Route(params, result);

    if (status == osrm::Status::Ok)
    {
        auto &routes = result.values["routes"].get<osrm::json::Array>();

        // Let's just use the first route
        auto &route = routes.values.at(0).get<osrm::json::Object>();
        const auto distance = route.values["distance"].get<osrm::json::Number>().value;
        const auto duration = route.values["duration"].get<osrm::json::Number>().value;

        auto &leg = route.values["legs"].get<osrm::json::Array>().values.at(0).get<osrm::json::Object>();
        auto &steps = leg.values["steps"].get<osrm::json::Array>();

        for(int i=0; i < steps.values.size(); i++) {
            auto &step = steps.values.at(i).get<osrm::json::Object>();
            auto &maneuver = step.values["maneuver"].get<osrm::json::Object>();
            std::cout << maneuver.values["type"].get<osrm::json::String>().value << ":" << maneuver.values["modifier"].get<osrm::json::String>().value << std::endl;
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
    else if (status == osrm::Status::Error)
    {
        const auto code = result.values["code"].get<osrm::json::String>().value;
        const auto message = result.values["message"].get<osrm::json::String>().value;

        std::cout << "Code: " << code << "\n";
        std::cout << "Message: " << code << "\n";
    }
}

void Navigator::addDestination(double longitude, double latitude) {
    Navigator::destination = {osrm::util::FloatLongitude(longitude), osrm::util::FloatLatitude(latitude)};
}

void Navigator::addCoordinate(double longitude, double latitude) {
    Navigator::coordinateHistory.push_back({osrm::util::FloatLongitude(longitude), osrm::util::FloatLatitude(latitude)});
}








