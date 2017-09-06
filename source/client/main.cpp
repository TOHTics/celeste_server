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
#include "sunspec/sunspec.hpp"
#include "sunspec/data/SunSpecDataResponse.hpp"

using namespace std;
using namespace restbed;
using namespace sunspec;
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
    string xml_body = (char*) response->get_body().data();

    data::SunSpecDataResponse rr;
    try
    {
        rr = data::SunSpecDataResponse::from_xml(xml_body);
    } catch (data::XMLError e)
    {
        cout << e.what() << endl;
    }
//    fprintf( stderr, "Body:           %.*s\n\n", (int) response->get_body().size(), xml_body );

    cout << rr.message << endl << rr.code << endl << rr.status << endl << rr.reason << endl;

}

int main( const int, const char** )
{
    std::string body = "<SunSpecData v=\"1\">\n"
            "<d lid=\"11:22:33:44:55:66\" man=\"gsc\" mod=\"r100\" sn=\"A123456\" t=\"2011-05-\n"
            "12T09:20:50Z\" cid=\"AR45\"> <m id=\"401\">\n"
            "<p id=\"DCA_Max\">100</p>\n"
            "<p id=\"N\">3</p>\n"
            "<p id=\"DCV\">489</p>\n"
            "<p id=\"DCA\" x=\"1\">4.78</p> <p id=\"DCA\" x=\"2\">12.30</p> <p id=\"DCA\" x=\"3\">0.42</p>\n"
            "</m> </d>\n"
            "</SunSpecData>";

    auto request = make_shared< Request >( Uri( "http://localhost:10000/resource/logger_upload" ) );
    request->set_method("POST");
    request->set_body(body);
    request->set_header( "Host", "localhost" );
    request->set_header("Content-Length", to_string(body.size()));

    auto response = Http::sync( request );
    print( response );

    return EXIT_SUCCESS;
}