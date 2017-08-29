/*
 * Example illustrating HTTP service.
 *
 * Server Usage:
 *    sudo ./distribution/example/http_service
 *
 * Client Usage:
 *    curl -w'\n' -v -X POST --data 'Hello, Restbed' 'http://localhost/resource'
 */

#include <iostream>
#include <memory>
#include <cstdlib>
#include <restbed>


#include "sunspec/data/data.hpp"
#include "srv/resource/resource.hpp"

using namespace std;
using namespace restbed;


int main( const int, const char** )
{

    using namespace sunspec::data;
    using namespace solarplant::srv;
    auto resource = resource::make_logger_upload("/resource/logger_upload");

    shared_ptr<Settings> settings = make_shared<Settings>();
    settings->set_port(10000);

    Service service;
    service.publish( resource );
    service.start(settings);

    return EXIT_SUCCESS;
}
