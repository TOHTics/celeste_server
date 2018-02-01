#define ELPP_THREAD_SAFE
#include <iostream>
#include <boost/program_options.hpp>
#include <string>
#include <soci/mysql/soci-mysql.h>
#include "srv/CelesteServer.hpp"

using namespace std;
using namespace celeste;
using namespace boost::program_options;


int main( const int argc, const char** argv)
{  
    if (argc == 1)
    {
        cout << "\e[37mNo arguments passed. "
                "Will attempt to start with default parameters.\n\e[m";
    }

    try
    {
        options_description desc{"Options for CelesteServer"};
        desc.add_options()
            ("help,h",
                "Help screen")
            
            ("api-root,r",
                value<string>()->default_value("celeste"),
                "Root where to run the service; where the API may be accessed.\n"
                "E.g. http://[host]:[port]/[api-root]/")

            ("port,p",
                value<int>()->default_value(9001),
                "Port where CelesteServer will run.")
            
            ("worker-limit,j",
                value<int>()->default_value(4),
                "Limit of workers that the server has available for use.")
            
            ("db",
                value<string>()->default_value("celestesensordata"),
                "The name of the schema.")
            
            ("user",
                value<string>()->default_value("root"),
                "Name of user for access to the DB.")

            ("password",
                value<string>()->default_value(""),
                "Password to authenticate the user on the DB.")

            ("db-port",
                value<int>()->default_value(3306),
                "Port where the DB listens for incoming connections.")
            ;

        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << '\n';
        }
        else
        {
            std::string dbSettings("db=" + vm["db"].as<string>() +
                               " user=" + vm["user"].as<string>() +
                               " password=" + vm["password"].as<string>() +
                               " port=" + to_string(vm["db-port"].as<int>())
                               );

            CelesteServer server(vm["port"].as<int>(),
                                  dbSettings,
                                  vm["worker-limit"].as<int>(),
                                  vm["api-root"].as<string>()
                                  );

            cout << "Running...\nPress ctrl-c to stop.\n"; 

            server.start();
        }

    }
    catch (error& e)
    {
        cerr 
            << "\e[0;31merror: \e[0m"
            << e.what() 
            << '\n';
    }
    catch (soci::mysql_soci_error& e)
    {
        cerr
            << "\e[0;31mdatabase specific error: \e[0m"
            << e.what()
            << '\n';
    }
    catch (exception& e)
    {
        cerr 
            << "\e[0;31merror: \e[0m"
            << e.what() 
            << '\n';
    }
    catch (...)
    {
        cerr 
            << "Something went wrong while starting the server!\n"
            << "Please use --help or -h to see the available options.\n";
    }

    return 0;
}