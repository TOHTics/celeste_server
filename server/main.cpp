#include <restbed>
#include <iostream>
#include <thread>
#include <json.hpp>
#include "srv/db/db.hpp"
#include "srv/service/services.hpp"

#include "srv/service/readings/ReadRequest.hpp"
using namespace std;
using namespace celeste;
void print_welcome_message()
{
    cout << "\e[37m";
    cout << "| ---------------------------------------------------- |\n"
                "|  _______ ____  _    _   _______ _____ _____  _____   |\n"
                "| |__   __/ __ \\| |  | | |__   __|_   _/ ____|/ ____|  |\n"
                "|    | | | |  | | |__| |    | |    | || |    | (___    |\n"
                "|    | | | |  | |  __  |    | |    | || |     \\___\\    |\n"
                "|    | | | |__| | |  | |    | |   _| || |____ ____) |  |\n"
                "|    |_|  \\____/|_|  |_|    |_|  |_____\\_____|_____/   |\n" 
             << "| ---------------------------------------------------- |\n";
    cout << "\e[m";
}

int main( const int argc, const char** argv)
{  
    mysqlx::SessionSettings dbSettings("localhost", 33060, "root", "root", "Celeste");

    using DeviceResource = resource::Devices<nlohmann::json>;
    using ModelResource = resource::Models<nlohmann::json>;
    using PointResource = resource::Points<nlohmann::json>;
    using DeviceModelAssocResource = resource::DeviceModelAssocs<nlohmann::json>;
    using LoggerResource = resource::LoggerUpload;
    using ReadingResource = resource::ReadingDispatcher;

    print_welcome_message();

    cout << "\e[33m";
    cout << "@ Making resources...\n";

    auto devices = make_shared<DeviceResource>(dbSettings);
    auto models = make_shared<ModelResource>(dbSettings);
    auto points = make_shared<PointResource>(dbSettings);
    auto upload = make_shared<LoggerResource>(dbSettings);
    auto reading = make_shared<ReadingResource>(dbSettings);
    auto device_model = make_shared<DeviceModelAssocResource>(dbSettings);

    cout << "@ Configuring server...\n";

    auto settings = make_shared<restbed::Settings>();
    settings->set_port(10000);
    settings->set_root("celeste");

    cout << "@ Publishing resources...\n";
    
    restbed::Service api;
    api.publish(devices);
    api.publish(models);
    api.publish(points);
    api.publish(upload);
    api.publish(reading);
    api.publish(device_model);

    cout << "@ Starting server...\n";
    cout << "\e[m";
    api.start(settings);

    return 0;
}