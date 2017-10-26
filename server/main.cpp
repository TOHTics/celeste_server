#include <restbed>
#include <iostream>
// #include "db/entity/AbstractEntity.hpp"
#include "srv/resource/resource.hpp"

using namespace std;
using namespace solarplant::srv::resource;

int main( const int argc, const char** argv)
{  

    auto upload = make_logger_upload("/resource/logger_upload");

    auto settings = make_shared<restbed::Settings>();
    settings->set_port(10000);

    restbed::Service service;
    service.publish(upload);
    service.start(settings);


   //  sql::Driver *driver;
   //  shared_ptr<sql::Connection> conn;

   //  try
   //  {
   //      driver = get_driver_instance();
   //      conn = shared_ptr<sql::Connection>(driver->connect("localhost:3306", "root", "root"));
   //      conn->setSchema("mydb");
   // } catch (sql::SQLException e)
   //  {
   //      cout << e.what() << endl; 
   //  }

   //  dao::ModelRecordDAO dao_obj(conn);
   //  entity::ModelRecord obj;

   //  // obj.device_id = 12;
   //  // obj.index = 1;
   //  // obj.t = util::get_universal_time();
   //  try
   //  {
   //      // dao_obj.save(obj);
   //      std::tuple<int, std::string, int, int> id{42, "Thermometer", 1, 1};
   //      obj = dao_obj.get(id);
   //  } catch (sql::SQLException e)
   //  {
   //      cout << e.what() << endl;
   //  }

   //  cout << obj.device_id << endl
   //       << obj.model_id << endl;

    return 0;
}