/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 24/08/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include <soci/mysql/soci-mysql.h>
#include <soci/error.h>

#include "srv/error.hpp"

#include "srv/service/common.hpp"
#include "srv/service/base64.h"

#include "LoggerUpload.hpp"

using namespace std;
using namespace sunspec;
using namespace soci;

namespace celeste
{   
namespace resource
{
    // --- CLASS DEFINITIONS ---------
    LoggerUpload::LoggerUpload(const string& dbSettings, size_t max_connections)
        : m_db_settings(dbSettings)
    {
        set_path("/logger/upload");
        set_method_handler("POST", [this] (const shared_ptr<restbed::Session> session) {POST(session);});

        for (int i = 0; i < max_connections; ++i)
            m_sql_pool.emplace(mysql, dbSettings);
    }

    void LoggerUpload::persist_data(const sunspec::data::SunSpecData& data)
    {
        // we dont even want to wait for a connection to open up
        auto sql = m_sql_pool.allocate(mysql, m_db_settings);

        try
        {
            *sql << "select 1"; // "ping" the server
        } catch (mysql_soci_error&)
        {
            sql->reconnect(); // attempt reconnect
        } catch (exception&)
        {
            sql->open(mysql, m_db_settings); // if everything went bad attempt to open connection again
        }

        data::DeviceData dev_record;
        statement insert_device_rec
            = (sql->prepare
                << "insert into "
                << "DeviceRecord(Device_id, t, cid, ifc, lid) "
                << "values(:DeviceId, :t, :cid, :ifc, :lid) ",
                use(dev_record.id), use(dev_record.t), use(dev_record.cid),
                use(dev_record.ifc), use(dev_record.lid)
            );

        long devRecordIdx;
        data::ModelData mod_record;
        statement insert_model_rec
            = (
                sql->prepare
                << "insert into "
                << "ModelRecord(Device_id, DeviceRecord_idx, Model_id, Model_idx) "
                << "values(:DeviceId, :DeviceRecordIdx, :ModelId, :ModelIdx)",
                use(dev_record.id), use(devRecordIdx),
                use(mod_record.id), use(mod_record.x)
            );

        data::PointData point_record;
        statement insert_point_rec
            = (
                sql->prepare
                << "insert into "
                << "PointRecord(Device_id, Model_id, Point_id, Model_idx, sf, t, v, DeviceRecord_idx) "
                << "values(:DeviceId, :ModelId, :PointId, :ModelIdx, :sf, :t, :v, :DeviceRecordIdx)",
                use(dev_record.id), use(mod_record.id), use(point_record.id),
                use(mod_record.x),
                use(0), use(point_record.t),
                use(point_record.value), use(devRecordIdx)
            );

        transaction tr(*sql);

        for (auto devit = data.cbegin(); devit != data.cend(); ++devit)
        {
            dev_record = *devit;
            insert_device_rec.execute(true);
            sql->get_last_insert_id("DeviceRecord", devRecordIdx);

            for (auto modit = devit->cbegin(); modit != devit->cend(); ++modit)
            {   
                mod_record = *modit;
                insert_model_rec.execute(true);

                for (auto pointit = modit->cbegin(); pointit != modit->cend(); ++pointit)
                {
                    point_record = *pointit;
                    insert_point_rec.execute(true);
                }
            }
        }
        tr.commit();
    }

    template <typename Error>
    void handle_parsing_error(Error &&e, const shared_ptr<restbed::Session> session)
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

    void LoggerUpload::POST(const shared_ptr<restbed::Session> session)
    {
        // get request
        const auto request = session->get_request();

        // get headers
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        if (content_length == 0)
            throw MissingHeaderError("Content-Length");

        session->fetch(content_length,
        [this, request] (const shared_ptr<restbed::Session> session,
                          const restbed::Bytes& bytes)
        {
            // fetch body
            string body;
            bytes2string(bytes, body);

            try
            {
                // Persist records
                persist_data(data::SunSpecData::from_xml(body));
                
                // close
                if (request->get_header("Connection", "close") == "keep-alive")
                {
                    session->yield(restbed::OK, {{"Content-Length", "0"}});
                }
                else
                {
                   session->close(restbed::OK, {{"Content-Length", "0"}});
                }
            }
            catch (data::XMLException& e)
            {
                throw XMLError(e.what());
            }
            catch (mysql_soci_error& e)
            {
                throw DatabaseError("Could not upload data with error code: " + to_string(e.err_num_));
            }
        });
    }
}
}