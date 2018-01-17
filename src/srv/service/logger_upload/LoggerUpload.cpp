/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 24/08/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include <soci/mysql/soci-mysql.h>
#include <soci/error.h>

#include "LoggerUpload.hpp"
#include "srv/service/status.hpp"
#include "srv/service/common.hpp"

using namespace std;
using namespace sunspec;
using namespace soci;

namespace celeste
{   
namespace resource
{
    // --- CLASS DEFINITIONS ---------
    LoggerUpload::LoggerUpload(const std::string& dbSettings)
    {
        set_paths({"/logger/upload/verbose/", "/logger/upload"});
        set_method_handler("POST", [this] (const std::shared_ptr<restbed::Session> session) {POST(session);});
        
        for (int i = 0; i < 10; ++i)
            sqlPool.emplace(mysql, dbSettings);
    }

    void LoggerUpload::persist_data(const sunspec::data::SunSpecData& data)
    {
        auto sql = sqlPool.acquire_wait();
        transaction tr(*sql);

        data::DeviceData dev_record;
        statement insert_device_rec
            = (sql->prepare
                << "insert into "
                << "DeviceRecord(Device_id, t, cid, if, lid) "
                << "values(:DeviceId, :t, :cid, :if, :lid) ",
                use(dev_record.id), use(dev_record.t), use(dev_record.cid),
                use(dev_record.ifc), use(dev_record.lid)
            );

        data::ModelData mod_record;
        statement insert_model_rec
            = (
                sql->prepare
                << "insert into "
                << "ModelRecord(Device_id, Model_id, Model_idx) "
                << "values(:DeviceId, :ModelId, :ModelIdx)",
                use(dev_record.id), use(mod_record.id),
                use(mod_record.x.empty() ? mod_record.x : "0")
            );

        data::PointData point_record;
        statement insert_point_rec
            = (
                sql->prepare
                << "insert into "
                << "PointRecord(Device_id, Model_id, Point_id, Model_idx sf, t, data) "
                << "values(:DeviceId, :ModelId, :PointId, :ModelIdx, :sf, :t, :data)",
                use(dev_record.id), use(mod_record.id), use(point_record.id),
                use(!mod_record.x.empty() ? mod_record.x : "0"),
                use(point_record.sf), use(!point_record.sf.empty() ? point_record.sf : "0"),
                use(point_record.value)
            );

        // persist all DeviceRecords
        for (auto devit = data.cbegin(); devit != data.cend(); devit++)
        {
            dev_record = *devit;
            insert_device_rec.execute(true);

            // persist model records
            for (auto modit = devit->cbegin(); modit != devit->cend(); modit++)
            {   
                // insert model packet
                dev_record = *devit;
                insert_model_rec.execute(true);

                // persists point records
                for (auto pointit = modit->cbegin(); pointit != modit->cend(); pointit++)
                {
                    point_record = *pointit;
                    insert_point_rec.execute(true);
                }
            }
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

        bool verbose = ("/logger/upload/verbose/" == request->get_path());
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
                session->close(status::XML_SYNTAX_ERROR, e.what());
            else
                throw status::XML_SYNTAX_ERROR;
        }
        catch (const exception& e)
        {
            throw status::UNHANDLED_EXCEPTION;
        }
    }
}
}