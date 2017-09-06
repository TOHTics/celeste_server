#include <memory>
#include <cstdlib>
#include <restbed>
#include <iostream>
#include "srv/resource/resource.hpp"
#include "sunspec/data/data.hpp"
#include "sunspec/model/GPSReading.hpp"

using namespace std;
using namespace restbed;
using namespace solarplant::srv;
using namespace sunspec;

int main( const int, const char** )
{

//    data::DeviceData d1;
//    d1.id = "1";
//    d1.man = "Mulkin";
//
//    data::DeviceData d2;
//    d2.id = "2";
//
//
//    data::DeviceResult r1;
//    r1.code = "Yee";
//    r1.devData = d1;
//
//    data::DeviceResult r2;
//    r2.code = "Yee2";
//    r2.message = "Meamm eamema222";
//    r2.devData = d2;
//
//    data::SunSpecDataResponse resp;
//    resp.add_device_result(r1);
//    resp.add_device_result(r2);
//    resp.status = 5;
//
//    std::string xml = data::SunSpecDataResponse::to_xml(resp);
//    cout << xml << endl;
//    point::Altitude alt = 200.23;
//
//    model::GPSReading gpsReading(alt);
//
//    cout << gpsReading.alt << endl;
//    cout << alt.d << endl;

    auto resource = resource::make_logger_upload("/resource/logger_upload/");


    shared_ptr<Settings> settings = make_shared<Settings>();
    settings->set_port(10000);

    Service service;
    service.publish( resource );
    service.start( settings );


//    cout << rr.message << endl;

    return EXIT_SUCCESS;
}