/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include "DeviceModel.hpp"
#include "srv/service/common.hpp"
#include "srv/service/device/device.hpp"
#include "srv/service/model/model.hpp"

using namespace std;

namespace celeste
{
namespace resource
{   
    // --- CLASS DEFINITIONS ---------
    DeviceModelAssocs<nlohmann::json>::DeviceModelAssocs(const celeste::SessionSettings& dbSettings)
        :   dbSettings(dbSettings),
            dbSession(dbSettings),
            celesteDB(dbSession.getSchema(dbSettings.db)),
            associationTable(celesteDB.getTable("Device_Model"))
    {
        set_path("/device_model");
        set_method_handler("GET", [this] (const std::shared_ptr<restbed::Session> session) {GET(session);});
        set_method_handler("POST",   [this] (const std::shared_ptr<restbed::Session> session) {POST(session);});
        set_method_handler("DELETE", [this] (const std::shared_ptr<restbed::Session> session) {DELETE(session);});
    }

    std::vector<DeviceModelAssoc> DeviceModelAssocs<nlohmann::json>::get(const std::string& deviceId)
    {
        auto res = 
            associationTable.
            select("*").
            where("Device_id = :DeviceId").
            bind(ValueMap{{"DeviceId", deviceId.c_str()}}).
            execute();

        std::vector<DeviceModelAssoc> assocs(res.count());
        int i = 0;
        for (auto&& r : res.fetchAll())
            assocs[i++] = static_cast<mysqlx::SerializableRow>(move(r)).as<DeviceModelAssoc>();
        return assocs;
    }

    std::vector<DeviceModelAssoc> DeviceModelAssocs<nlohmann::json>::get(const std::string& deviceId, const std::string& modelId)
    {
        auto res = 
            associationTable.
            select("*").
            where("Device_id = :DeviceId AND Model_id = :ModelId").
            bind(ValueMap{{"DeviceId", deviceId.c_str()}, {"ModelId", modelId.c_str()}}).
            execute();

        std::vector<DeviceModelAssoc> assocs(res.count());
        int i = 0;
        for (auto&& r : res.fetchAll())
            assocs[i++] = static_cast<mysqlx::SerializableRow>(move(r)).as<DeviceModelAssoc>();
        return assocs;
    }


    DeviceModelAssoc DeviceModelAssocs<nlohmann::json>::get(const std::string& deviceId, const std::string& modelId, int idx)
    {
        auto res = 
            associationTable.
            select("*").
            where("id = :DeviceId AND idx = :idx AND Model_id = :ModelId").
            bind(ValueMap{{"DeviceId", deviceId.c_str()}, {"idx", idx}, {"ModelId", modelId.c_str()}}).
            execute();
        mysqlx::SerializableRow row = res.fetchOne();
        return row.as<DeviceModelAssoc>();
    }

    int DeviceModelAssocs<nlohmann::json>::associate(const std::string& deviceId, const std::string& modelId, const boost::optional<std::string>& note)
    {
        dbSession.startTransaction();
        try
        {
            const int idx = 
                dbSession.sql(R"(
                              SELECT IFNULL(MAX(idx) + 1, 0) FROM
                              (
                                SELECT idx FROM Device_Model
                                WHERE
                                Device_id = ?
                                AND Model_id = ?
                              ) as tmp 
                              )"
                ).
                bind(ValueList{deviceId.c_str(), modelId.c_str()}).
                execute().
                fetchOne().
                get(0);

            associationTable.
            insert("idx", "Device_id", "Model_id", "note").
            values(idx, deviceId.c_str(), modelId.c_str(), mysqlx::EnhancedValue{note}).
            execute();

            dbSession.commit();
            return idx;
        }
        catch (const mysqlx::Error& e)
        {
            dbSession.rollback();
        }
        return 0;
    }

    void DeviceModelAssocs<nlohmann::json>::dissasociate(const std::string& deviceId, const std::string& modelId, int idx)
    {
        associationTable.
        remove().
        where("Device_id = :DeviceId AND Model_id = :ModelId AND idx = :idx").
        bind(ValueMap{{"DeviceId", deviceId.c_str()}, {"ModelId", modelId.c_str()}, {"idx", idx}}).
        execute();
    }

    void DeviceModelAssocs<nlohmann::json>::GET(const std::shared_ptr<restbed::Session> session)
    {   
        // get request
        const auto request = session->get_request();

        // get headers
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        // fetch data to access later
        session->fetch(content_length, [] (const shared_ptr<restbed::Session> session, const restbed::Bytes &bytes) {});

        // get json from request
        json_type data = get_json<json_type>(*request);

        // validate data
        if (data["DeviceId"].is_null())
            throw status::MISSING_FIELD_DEVICEID; // DeviceId is required for any request

        // get device from db
        json_type response;

        if (data["idx"].is_null() && data["ModelId"].is_null())
        {
            response = this->get(data["DeviceId"]);
        }
        else if (data["ModelId"].is_null())
        {
            response = this->get(data["DeviceId"], data["ModelId"]);
        }
        else
        {
            response = this->get(data["DeviceId"], data["ModelId"], data["idx"]);
        }

        // close
        session->close(restbed::OK,
                       response.dump(),
                       {
                            { "Content-Length", to_string(response.dump().size()) },
                            { "Connection",     "close" }
                       });
    }

    void DeviceModelAssocs<nlohmann::json>::POST(const std::shared_ptr<restbed::Session> session)
    {
        // get request
        const auto request = session->get_request();

        // get headers
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        // fetch data to access later
        session->fetch(content_length, [] (const shared_ptr<restbed::Session> session, const restbed::Bytes &bytes) {});

        // get json from request
        json_type data = get_json<json_type>(*request);

        // validate data
        if (data["DeviceId"].is_null())
            throw status::MISSING_FIELD_DEVICEID;

        if (data["ModelId"].is_null())
            throw status::MISSING_FIELD_MODELID;

        if (data["note"].is_null())
            data["note"] = nullptr;

        DeviceModelAssoc dm = data.get<DeviceModelAssoc>();
        auto idx = this->associate(dm.DeviceId, dm.ModelId, dm.note);
        json_type response{{"idx", idx}};

        // close
        session->close(restbed::OK,
                       response.dump(),
                       {
                            { "Content-Length", to_string(response.dump().size()) },
                            { "Connection",     "close" }
                       });
    }

    void DeviceModelAssocs<nlohmann::json>::DELETE(const std::shared_ptr<restbed::Session> session)
    {
        // get request
        const auto request = session->get_request();

        // get headers
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        // fetch data to access later
        session->fetch(content_length, [] (const shared_ptr<restbed::Session> session, const restbed::Bytes &bytes) {});

        // get json from request
        json_type data = get_json<json_type>(*request);

        // validate data
        if (data["DeviceId"].is_null())
            throw status::MISSING_FIELD_DEVICEID;

        if (data["ModelId"].is_null())
            throw status::MISSING_FIELD_MODELID;

        if (data["idx"].is_null())
            throw status::MISSING_FIELD_IDX;

        // dissasociate the device and the model
        this->dissasociate(data["DeviceId"], data["ModelId"], data["idx"]);

        // close
        session->close(restbed::OK);
    }
}
}

// --- SERIALIZERS -------------------
namespace mysqlx
{
    using namespace celeste::resource;

    void row_serializer<DeviceModelAssoc>::to_row (SerializableRow& row, const DeviceModelAssoc& dm)
    {
        row.set(0, dm.idx);
        row.set(1, dm.DeviceId);
        row.set(2, dm.ModelId);
        row.set(3, dm.note);
    }

    void row_serializer<DeviceModelAssoc>::from_row (const SerializableRow& row, DeviceModelAssoc& dm)
    {
        SerializableRow tmp = row; // row.get() is not marked const, hence we need this tmp
        dm = DeviceModelAssoc {
            .idx        = tmp.get(0),
            .DeviceId   = tmp.get(1),
            .ModelId    = tmp.get(2),
            .note       = tmp.get(3)
        };
    }
}

namespace nlohmann
{
    using namespace celeste::resource;

    void adl_serializer<DeviceModelAssoc>::to_json(json& j, const DeviceModelAssoc& dm)
    {
        j = json {
            {"DeviceId",    dm.DeviceId},
            {"ModelId",     dm.ModelId},
            {"idx",         dm.idx},
            {"note",        dm.note}
        };
    }

    void adl_serializer<DeviceModelAssoc>::from_json(const json& j, DeviceModelAssoc& dm)
    {
        dm = DeviceModelAssoc {
            .DeviceId   = j.at("DeviceId"),
            .ModelId    = j.at("ModelId"),
            // .idx        = j.at("idx"),
            .note       = j.at("note")
        };
    }
}