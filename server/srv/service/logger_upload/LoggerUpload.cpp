/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 24/08/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include "LoggerUpload.hpp"
#include "srv/service/error.hpp"
#include "srv/service/common.hpp"

#include <iostream>
using namespace std;
using namespace sunspec;

namespace celeste
{   
namespace resource
{
    // --- CLASS DEFINITIONS ---------
    LoggerUpload::LoggerUpload(const mysqlx::SessionSettings& dbSettings)
        :   dbSession(dbSettings),
            celesteDB(dbSession.getSchema("Celeste")),
            deviceRecordTable(celesteDB.getTable("DeviceRecord")),
            modelRecordTable(celesteDB.getTable("ModelRecord")),
            pointRecordTable(celesteDB.getTable("PointRecord"))
    {
        set_paths({"/logger/upload/verbose/", "/logger/upload"});
        set_method_handler("POST", [this] (const std::shared_ptr<restbed::Session> session) {POST(session);});
    }


    void LoggerUpload::persist_data(const sunspec::data::SunSpecData& data)
    {
        try
        {   
            // --- START TRANSACTION -
            dbSession.startTransaction();
            // -----------------------

            // persist all DeviceRecords
            for (auto devit = data.cbegin(); devit != data.cend(); devit++)
            {
                // get aggregated index of the devicerecord to be saved
                const int devrecord_idx = 
                    dbSession.sql(R"(
                                  SELECT IFNULL(MAX(idx) + 1, 0) FROM
                                  (
                                    SELECT idx FROM Celeste.DeviceRecord
                                    WHERE (Device_id = ?)
                                  ) as tmp 
                                  )"
                    ).bind(ValueList{devit->id.c_str()}).execute().fetchOne().get(0);

                // insert device packet into DB
                deviceRecordTable.
                insert("idx", "Device_id", "t", "cid", "if", "lid").
                values(devrecord_idx,
                       devit->id.c_str(),
                       devit->t.c_str(),
                       devit->cid.c_str(),
                       devit->ifc.c_str(),
                       devit->lid.c_str()
                       ).
                execute();

                // persist model records
                for (auto modit = devit->cbegin(); modit != devit->cend(); modit++)
                {   
                    // insert model packet
                    modelRecordTable.
                    insert("Model_idx", "Model_id", "DeviceRecord_idx", "Device_id").
                    values(modit->x.empty() ? "0" : modit->x.c_str(),
                           modit->id.c_str(),
                           devrecord_idx,
                           devit->id.c_str()
                           ).
                    execute();

                    // persists point records
                    for (auto pointit = modit->cbegin(); pointit != modit->cend(); pointit++)
                    {
                        // insert point packet
                        pointRecordTable.
                        insert("idx",
                               "Point_id",
                               "Model_idx",
                               "Model_id",
                               "DeviceRecord_idx",
                               "Device_id",
                               "sf",
                               "t",
                               "data").
                        values(pointit->x.empty() ? "0" : pointit->x.c_str(),
                               pointit->id.c_str(),
                               modit->x.empty() ? "0" : modit->x.c_str(),
                               modit->id.c_str(),
                               devrecord_idx,
                               devit->id.c_str(),
                               pointit->sf.c_str(),
                               pointit->t.empty() ? mysqlx::nullvalue : pointit->t.c_str(),
                               pointit->value.c_str()).
                        execute();
                    }
                }
            }  
            // --- TRANSACTION ENDS --
            dbSession.commit();
            // -----------------------
        } catch (const mysqlx::Error& e)
        {
            dbSession.rollback();
            throw e;
        }
    }

    template <typename Error>
    void handle_parsing_error(Error &&e, const std::shared_ptr<restbed::Session> session)
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


    void LoggerUpload::POST(const std::shared_ptr<restbed::Session> session)
    {
        // get request
        const auto request = session->get_request();

        // get headers
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        // fetch body
        string body;
        session->fetch(content_length,
        [request, &body] (const shared_ptr<restbed::Session> session,
                          const restbed::Bytes &bytes)
        {
            bytes2string(bytes, body);
        });

        bool verbose = false;
        verbose = ("/logger/upload/verbose/" == request->get_path());
        try
        {
            // Attempt to parse data
            data::SunSpecData data;
            data = data::SunSpecData::from_xml(body);

            // TODO
            /* Check device records and verify they stick to the rules.
             * If they don't, then add to the SunSpecDataResponse all 
             * the failed devices and why they failed using DeviceResult.
             */
            data::verifier sunspec_verifier;
            for ( auto dit = data.begin(); dit != data.end(); dit++ )
            {
                data::DeviceData device = *dit;

                if(!sunspec_verifier.verify(device))
                {
                    // do stuff
                }
            }
            // Persist records
            persist_data(data);
        
            // close
            session->close(restbed::OK);
        } catch (const data::XMLException& e)
        {
            if (verbose)
                session->close(400, e.what());
            else
                throw 400;
        }
        catch (const mysqlx::Error& e)
        {
            if (verbose)
                session->close(400, e.what());
            else
                throw 500;
        }
        catch (const exception& e)
        {
            cout << e.what() << "\n";

        }
    }
}
}