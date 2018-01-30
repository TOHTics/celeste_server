#include <restbed>
#include <iostream>
#include <thread>
#include <json.hpp>
#include "srv/celeste_server/celeste_server.hpp"

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
    std::string dbSettings("db=mydb user=root password=root port=3306");
    print_welcome_message();

    celeste_server server(9001, dbSettings, 10, "celeste");
    server.start();

    return 0;
}