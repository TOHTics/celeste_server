/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains definitions and helper functions for a logger upload resource.
 */


#include <restbed>
#include <iostream>
#include <string>
#include "logger_upload.hpp"
#include "sunspec/data/data.hpp"
#include "sunspec/util/util.hpp"
#include "srv/util.hpp"


using namespace sunspec;
using namespace std;

namespace solarplant
{
namespace srv
{
namespace handler
{
template <typename Error>
void handle_unknown_error(const shared_ptr<restbed::Session> session, Error &&e)
{
    data::SunSpecDataResponse ssr;
    ssr.status = sdx::FAILURE;
    ssr.code = "FAILURE";
    ssr.message = e.what();
    ssr.reason = "Undocumented error";

    string rr_body = data::SunSpecDataResponse::to_xml(ssr);

    session->close(sdx::FAILURE,
                   rr_body,
                   {
                           { "Content-Length", to_string(rr_body.size()) },
                           { "Connection",     "close" }
                   });
}

template <typename Error>
void handle_db_error(const shared_ptr<restbed::Session> session, Error &&e)
{
    data::SunSpecDataResponse ssr;
    ssr.status = sdx::FAILURE;
    ssr.code = "FAILURE";
    ssr.message = e.what();
    ssr.reason = "Database Error";

    string rr_body = data::SunSpecDataResponse::to_xml(ssr);

    session->close(sdx::FAILURE,
                   rr_body,
                   {
                           { "Content-Length", to_string(rr_body.size()) },
                           { "Connection",     "close" }
                   });
}

template <typename Error>
void handle_parsing_error(const shared_ptr<restbed::Session> session, Error &&e)
{
    data::SunSpecDataResponse ssr;
    ssr.status = sdx::FAILURE;
    ssr.code = "FAILURE";
    ssr.message = e.what();
    ssr.reason = "Parsing Error";

    string rr_body = data::SunSpecDataResponse::to_xml(ssr);

    session->close(sdx::FAILURE,
                   rr_body,
                   {
                           { "Content-Length", to_string(rr_body.size()) },
                           { "Connection",     "close" }
                   });
}

void handle_accept(const shared_ptr<restbed::Session> session)
{
    data::SunSpecDataResponse ssr;
    ssr.status = 400;
    ssr.code = "SUCCESS";
    ssr.reason = "No errors found";
    ssr.message = "We have accepted the records :-)";

    string rr_b = data::SunSpecDataResponse::to_xml(ssr);
    session->close(sdx::SUCCESS,
                   rr_b,
                   {
                           { "Content-Length", to_string(rr_b.size()) },
                           { "Connection",     "close" }
                   });
}


template<typename... Args>
shared_ptr<mysqlx::Session> make_db_session(Args... args)
{   
    return shared_ptr<mysqlx::Session>(new mysqlx::Session(args ...));
}

shared_ptr<mysqlx::Session> make_db_session(std::string host, size_t port, std::string user, std::string pwd)
{
    return shared_ptr<mysqlx::Session>(new mysqlx::Session(host, port, user, pwd));
}

void persist_data(const data::SunSpecData &data, const shared_ptr<mysqlx::Session> dbSession)
{
    cout << "Persisting records..." << endl;

    auto schema = dbSession->getSchema("Celeste");

    if (!schema.existsInDatabase())
    {
        throw mysqlx::Error("CRITICAL ERROR: The schema \"Celeste\" does not exist!\n"
                            "This is the schema where the records are persisted.\n"
                            "Please report ASAP to carlos.brito524@gmail.com.\n");
    }

    auto devRecordTable   = schema.getTable("DeviceRecord");
    auto modRecordTable   = schema.getTable("ModelRecord");
    auto pointRecordTable = schema.getTable("PointRecord");

    if (util::pair_equal
        (false,
         devRecordTable.existsInDatabase(),
         modRecordTable.existsInDatabase(),
         pointRecordTable.existsInDatabase()
         ))
    {
        throw mysqlx::Error("CRITICAL ERROR: One of the ___Record tables is missing from the schema \"Celeste\".\n"
                            "These are strictly required for persisting the records!\n"
                            "Please report ASAP to carlos.brito524@gmail.com."
                            );
    }

    cout << "Number of DeviceRecords received: " << data.size() << "\n";
    try
    {   
        for (auto devit = data.cbegin(); devit != data.cend(); devit++)
        {
            dbSession->startTransaction();
                dbSession->sql(
                                "SELECT @devrr_idx := IFNULL(MAX(`idx`) + 1, 0) FROM (SELECT `idx` FROM `Celeste`.`DeviceRecord` WHERE (`Device_id`=" + util::as_value_list(devit->id) + ")) as tmp;"
                                ).execute();

                dbSession->sql(
                    "INSERT INTO `Celeste`.`DeviceRecord` "
                    "(" 
                        + util::as_column_list("idx", "Device_id", "t", "cid", "if", "lid") + ")"
                    "VALUES"
                    "("
                        + util::as_value_list("@devrr_idx", devit->id, devit->t, devit->cid, devit->ifc, devit->lid) +
                    ");"
                ).execute();

                for (auto modit = devit->cbegin(); modit != devit->cend(); modit++)
                {   
                    dbSession->sql(
                        "INSERT INTO `Celeste`.`ModelRecord`"
                        "(" 
                            + util::as_column_list("DeviceRecord_idx", "Device_id", "Model_idx", "Model_id") + 
                        ")"
                        "VALUES"
                        "("
                            + util::as_value_list(
                                                  "@devrr_idx",
                                                  devit->id,
                                                  modit->x.empty() ? "0" : modit->x,
                                                  modit->id
                                                  ) +
                        ");"
                    ).execute();

                    for (auto pointit = modit->cbegin(); pointit != modit->cend(); pointit++)
                    {
                        dbSession->sql(
                            "INSERT INTO `Celeste`.`PointRecord`" 
                            "(" 
                                + util::as_column_list("idx", "Point_id", "Model_idx", "Model_id", "DeviceRecord_idx", "Device_id", "t", "sf", "value") +
                            ")"
                            "VALUES"
                            "("
                                + util::as_value_list(
                                                    pointit->x.empty() ? "0" : pointit->x,
                                                    pointit->id,
                                                    modit->x.empty() ? "0" : modit->x,
                                                    modit->id,
                                                    "@devrr_idx",
                                                    devit->id,
                                                    pointit->t.empty() ? "NULL" : pointit->t,
                                                    pointit->sf,
                                                    pointit->value
                                                    ) +
                            ");"
                            ).execute();
                    }
                }
            dbSession->commit();
        }
        
    } catch (const mysqlx::Error &e)
    {
        throw e;
    }

}

void logger_upload_handler(const shared_ptr<restbed::Session> session)
{   
    cout << "ACCESSED UPLOAD RESOURCE" << endl;
    // Get request and length of content
    const auto request = session->get_request();
    size_t content_length = (size_t) request->get_header("Content-Length", 0);

    // Fetch bytes from request and handle using callback function
    session->fetch(content_length, 
        [ request ] (const shared_ptr<restbed::Session> session, const restbed::Bytes &body)
        {
            // Try the following block and catch
            try 
            {
                cout << "Request for a logger_upload from: " << session->get_origin() << "\n";

                // Read n bytes of data into a char* 
                char *req_body_c = (char *) malloc(body.size() + 1);
                snprintf(req_body_c, (int) body.size() + 1, "%s", body.data());

                // Convert char* to string
                string req_body_str = req_body_c;

                // Attempt to parse data
                data::SunSpecData data;
                data = data::SunSpecData::from_xml(req_body_str);

                // TODO
                /* Check device records and verify they stick to the rules.
                 * If they don't, then add to the SunSpecDataResponse all 
                 * the failed devices and why they failed using DeviceResult.
                 */
                for ( auto dit = data.begin(); dit != data.end(); dit++ )
                {
                    data::DeviceData device = *dit;
                    // Check here
                }

                // Attempt connection to database
                shared_ptr<mysqlx::Session> dbSession;
                dbSession = make_shared<mysqlx::Session>("localhost", 33060, "root", "root", "Celeste");

                // Persist records
                persist_data(data, dbSession);
            }
            catch (const data::XMLException &e)
            {
                handle_parsing_error(session, e);
            }
            catch (const mysqlx::Error &e)
            {
                handle_db_error(session, e);
            }
            catch (const std::exception &e)
            {
                handle_unknown_error(session, e);
            }

            // If all went well then handle acceptance response
            handle_accept(session);
        });
}
}

namespace resource
{
shared_ptr<restbed::Resource> make_logger_upload(string path)
{
    restbed::Resource *resource = new restbed::Resource;
    resource->set_path(path);
    resource->set_method_handler("POST", handler::logger_upload_handler);
    return shared_ptr<restbed::Resource>(resource);
}

}
}
}