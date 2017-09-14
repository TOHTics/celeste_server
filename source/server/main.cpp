#include <memory>
#include <cstdlib>
#include <restbed>
#include <iostream>

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <db/db_exceptions.hpp>

#include "srv/resource/resource.hpp"
#include "db/db.hpp"

using namespace std;
using namespace solarplant::srv;
using namespace solarplant::db;
using namespace sunspec;

int main( const int, const char** )
{

    sql::Driver *driver;
    shared_ptr<sql::Connection> con;
    unique_ptr<dao::ClientDAO> client_dao;
    try
    {
        /* Create a connection */
        driver = get_driver_instance();
        con = shared_ptr<sql::Connection>(driver->connect("tcp://127.0.0.1:3305", "root", "root"));
        con->setSchema("mydb");
    } catch (const sql::SQLException &e)
    {
        cout << "Error: " << e.what() << endl;
        cout << "State: " << e.getSQLState() << endl;
    }


    entity::Client client;
    client.first_name = "Yo";
    client.last_name = "Yee";
    client.age = 22;
    client.id = 12;
    client.dob = boost::gregorian::from_string("1995-12-21");
    try
    {
        // Time to access the client
        client_dao = unique_ptr<dao::ClientDAO>(new dao::ClientDAO(con));
        entity::Client client;
        client = client_dao->get(10);

        cout << "id: " << client.id << endl
             << "name: " << client.first_name << " " << client.last_name << endl
             << "age: " << client.age << endl
             << "dob: " << client.dob << endl;
    } catch (const sql::SQLException &e)
    {
        cout << "Error: " << e.what() << endl;
        cout << "State: " << e.getSQLState() << endl;
    } catch (const dao::DAOException &e)
    {
        cout << "Error: " << e.what() << endl;
    }



//
//    auto resource = resource::make_logger_upload("/resource/logger_upload/");
//
//    shared_ptr<Settings> settings = make_shared<Settings>();
//    settings->set_port(10000);
//
//    Service service;
//    service.publish( resource );
//    service.start( settings );

    return EXIT_SUCCESS;
}