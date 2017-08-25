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

    fprintf( stderr, "Body:           %.*s...\n\n", 25, response->get_body( ).data( ) );
}

int main( const int, const char** )
{
    auto request = make_shared< Request >( Uri( "http://localhost:10000/solarplant/logger_upload" ) );
    request->set_method("POST");
    request->set_body("Hello restbed!");
    request->set_header( "Host", "localhost" );
    request->set_header("Content-Length", ::to_string(request->get_body().size()));

    auto response = Http::sync( request );
    print( response );

    return EXIT_SUCCESS;
}