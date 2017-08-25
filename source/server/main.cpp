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

#include "srv/resources/resource.hpp"

using namespace std;
using namespace restbed;


void post_method_handler( const shared_ptr< Session > session )
{
    const auto request = session->get_request( );

    size_t content_length = (size_t) request->get_header("Content-Length", 0 );

    session->fetch( content_length, [ request ]( const shared_ptr< Session > session, const Bytes & body )
    {
        fprintf( stdout, "%.*s\n", ( int ) body.size( ), body.data( ) );
        session->close( OK, "Hello, World!", { { "Content-Length", "13" }, { "Connection", "close" } } );
    } );
}

shared_ptr<Resource> getResource()
{
    Resource* resource = new Resource;
    resource->set_path("/solarplant/logger_upload");
    resource->set_method_handler( "POST", post_method_handler );
    return shared_ptr<Resource>(resource);
}

int main( const int, const char** )
{

//    auto resource = make_shared<Resource>();
    auto resource = getResource();

//    resource->set_path( "/solarplant/logger_upload" );
//    resource->set_method_handler( "POST", solarplant::srv::resource::post_logger_upload_handler );

    shared_ptr<Settings> settings = make_shared<Settings>();
    settings->set_port(10000);

    Service service;
    service.publish( resource );
    service.start(settings);

    return EXIT_SUCCESS;
}
