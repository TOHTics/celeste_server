/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains definitions and helper functions for a logger upload resource.
 */


#include <iostream>
#include <string>
#include <functional>
#include "logger_upload.hpp"
#include "sunspec/data/data.hpp"
#include "sunspec/util/util.hpp"

using namespace sunspec;
using namespace std;

namespace celeste
{
namespace resource
{
namespace logger_upload
{
    using ValueList = std::vector<mysqlx::Value>;

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

        session->close(restbed::INTERNAL_SERVER_ERROR,
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

        session->close(restbed::BAD_REQUEST,
                       rr_body,
                       {
                               { "Content-Length", to_string(rr_body.size()) },
                               { "Connection",     "close" }
                       });
    }

    void handle_accept(const shared_ptr<restbed::Session> session)
    {
        session->close(sdx::SUCCESS);
    }

    void persist_data(const data::SunSpecData &data, const shared_ptr<mysqlx::Session> dbSession)
    {
        using ValueList = std::vector<mysqlx::Value>;

        try
        {   
            
            for (auto devit = data.cbegin(); devit != data.cend(); devit++)
            {
                dbSession->startTransaction();
                    dbSession->sql(
                                    "SELECT @devrr_idx := IFNULL(MAX(idx) + 1, 0) FROM "
                                    "( "
                                        "SELECT idx FROM Celeste.DeviceRecord "
                                        "WHERE ( Device_id= ? )"
                                    ") as tmp;"
                                    ).bind(ValueList{devit->id.c_str()}).execute();


                    dbSession->sql(
                        "INSERT INTO Celeste.DeviceRecord "
                        "( `idx`, `Device_id`, `t`, `cid`, `if`, `lid` ) "
                        "VALUES "
                        "("
                            "@devrr_idx, ?, ?, ?, ?, ?"
                        ");"
                    ).bind(ValueList{
                        devit->id.c_str(),
                        devit->t.c_str(),
                        devit->cid.c_str(),
                        devit->ifc.c_str(),
                        devit->lid.c_str()
                    }).execute();

                    for (auto modit = devit->cbegin(); modit != devit->cend(); modit++)
                    {   
                        dbSession->sql(
                            "INSERT INTO Celeste.ModelRecord"
                            "(" 
                                "`DeviceRecord_idx`, `Device_id`, `Model_idx`, `Model_id`"
                            ")"
                            "VALUES"
                            "("
                                "@devrr_idx, ?, ?, ?"
                            ");"
                        ).bind(ValueList
                        {
                            devit->id.c_str(),
                            modit->x.empty() ? "0" : modit->x.c_str(),
                            modit->id.c_str()
                        }).execute();

                        for (auto pointit = modit->cbegin(); pointit != modit->cend(); pointit++)
                        {
                            dbSession->sql(
                                "INSERT INTO Celeste.PointRecord" 
                                "(" 
                                    "`idx`, `Point_id`, `Model_idx`, `Model_id`, `DeviceRecord_idx`, `Device_id`, `t`, `sf`, `data`"
                                ")"
                                "VALUES"
                                "("
                                    "?, ?, ?, ?, @devrr_idx, ?, ?, ?, ?"
                                ");"
                                ).bind(ValueList
                                {
                                    pointit->x.empty() ? "0" : pointit->x.c_str(),
                                    pointit->id.c_str(),
                                    modit->x.empty() ? "0" : modit->x.c_str(),
                                    modit->id.c_str(),
                                    devit->id.c_str(),
                                    pointit->t.empty() ? mysqlx::nullvalue : pointit->t.c_str(),
                                    pointit->sf.c_str(),
                                    pointit->value.c_str()
                                }).execute();
                        }
                    }
                dbSession->commit();
            }    
        } catch (const mysqlx::Error &e)
        {
            dbSession->rollback();
            throw e;
        }
    }

    void logger_upload_handler(const shared_ptr<restbed::Session> session,
                               const shared_ptr<mysqlx::Session> dbSession)
    {   
        // Get request and length of content
        const auto request = session->get_request();
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        // Fetch bytes from request and handle using callback function
        session->fetch(content_length, 
            [ request, dbSession ] (const shared_ptr<restbed::Session> session, const restbed::Bytes &body)
            {
                // Try the following block and catch
                try 
                {
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

    shared_ptr<restbed::Resource> make_logger_upload(const shared_ptr<mysqlx::Session> dbSession)
    {
        auto resource = make_shared<restbed::Resource>();
        resource->set_paths({
            "/logger/upload/",
            "/logger/upload/verbose?={(0|1)}"});
        resource->set_method_handler("POST", bind(logger_upload::logger_upload_handler, _1, dbSession));
        return std::move(resource);
    }
}
}