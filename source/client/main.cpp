/*
 * Example illustrating a HTTP client.
 *
 * Usage:
 *    ./distribution/example/http_client
 */

#include <memory>
#include <future>
#include <cstdio>
#include <cstdlib>
#include <restbed>
#include <iostream>

using namespace std;
using namespace restbed;

void print( const shared_ptr< Response >& response )
{
    fprintf( stderr, "*** Response ***\n" );
    fprintf( stderr, "Status Code:    %i\n", response->get_status_code( ) );
    fprintf( stderr, "Status Message: %s\n", response->get_status_message( ).data( ) );
    fprintf( stderr, "HTTP Version:   %.1f\n", response->get_version( ) );
    fprintf( stderr, "HTTP Protocol:  %s\n", response->get_protocol( ).data( ) );

    for ( const auto header : response->get_headers( ) )
    {
        fprintf( stderr, "Header '%s' > '%s'\n", header.first.data( ), header.second.data( ) );
    }

    auto length = response->get_header( "Content-Length", 0 );

    Http::fetch( length, response );
}

int main( const int, const char** )
{
    std::string body = "0987654321";
    auto request = make_shared< Request >( Uri( "http://localhost:10000/resource/logger_upload" ) );
    request->set_method("POST");
    request->set_body(body);
    request->set_header( "Host", "localhost" );
    request->set_header("Content-Length", "10");

    auto response = Http::sync( request );
    printf("%s\n", response->get_body().data());
    print( response );

    return EXIT_SUCCESS;
}