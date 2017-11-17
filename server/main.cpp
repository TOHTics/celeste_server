#include <restbed>
#include <iostream>
#include <thread>
#include <json.hpp>
#include "srv/db/db.hpp"
#include "srv/service/services.hpp"

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
    print_welcome_message();

    cout << "\e[33m";

    cout << "@ Connecting to DB..." << endl;
    auto dbSession = db::make_db_session("localhost", 33060, "root", "root", "Celeste");

    cout << "@ Making resources...\n";
    auto upload = resource::make_logger_upload("/logger_upload", dbSession);
    auto device = resource::make_device(dbSession);
    auto model = resource::make_model(dbSession);
    auto point = resource::make_point(dbSession);
    auto add_model = resource::make_add_model(dbSession);

    cout << "@ Configuring server...\n";

    auto settings = make_shared<restbed::Settings>();
    settings->set_port(10000);
    settings->set_worker_limit(4);
    settings->set_root("celeste");

    cout << "@ Publishing resources...\n";
    
    restbed::Service service;
    service.publish(upload);
    service.publish(device);
    service.publish(model);
    service.publish(point);
    service.publish(add_model);

    cout << "@ Starting server...\n";
    cout << "\e[m";
    service.start(settings);

    return 0;
}