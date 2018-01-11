/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include <mutex>
#include "device.hpp"
#include "srv/service/common.hpp"

using namespace std;

namespace celeste
{
namespace resource
{   
    // --- CLASS DEFINITIONS ---------
    Devices<nlohmann::json>::Devices(const celeste::SessionSettings& dbSettings)
        :   dbSettings(dbSettings),
            dbSession(dbSettings),
            celesteDB(dbSession.getSchema(dbSettings.db)),
            deviceTable(celesteDB.getTable("Device")),
            modelAssociator(dbSettings)
    {
        set_path("/device");
        set_method_handler("GET", [this] (const std::shared_ptr<restbed::Session> session) {GET(session);});
        set_method_handler("POST",   [this] (const std::shared_ptr<restbed::Session> session) {POST(session);});
        set_method_handler("DELETE", [this] (const std::shared_ptr<restbed::Session> session) {DELETE(session);});
    }

    Device Devices<nlohmann::json>::get(const std::string& deviceId)
    {
        lock_guard<mutex> guard(sqlMutex);
        auto res = 
            deviceTable.
            select("*").
            where("id = :DeviceId").
            bind(ValueMap{{"DeviceId", deviceId}}).
            execute();

        mysqlx::SerializableRow row = res.fetchOne();
        return row.as<Device>();
    }

    void Devices<nlohmann::json>::insert(const value_type& device)
    {
        lock_guard<mutex> guard(sqlMutex);
        try
        {
            dbSession.startTransaction();

            mysqlx::SerializableRow row;
            row.from(device);

            mysqlx::Row rtmp = row;

            deviceTable.
            insert("id", "man", "mod", "sn").
            values(rtmp).
            execute();

            dbSession.commit();
        } catch (const mysqlx::Error& e)
        {
            dbSession.rollback();
            throw e;
        }
    }

    // Note: The actions are already atomic so no need for the mutex lock
    void Devices<nlohmann::json>::insert(const value_type& device, std::vector<std::string> models)
    {
        this->insert(device); // atomic

        // associate the passed models
        for (const auto& modelId : models)
            modelAssociator.associate(device.DeviceId, modelId); // atomic
    }


    void Devices<nlohmann::json>::remove(const std::string& deviceId)
    {
        lock_guard<mutex> guard(sqlMutex);
        deviceTable.
        remove().
        where("id = :DeviceId").
        bind(ValueMap{{"DeviceId", deviceId}}).
        execute();
    }

    void Devices<nlohmann::json>::GET(const std::shared_ptr<restbed::Session> session)
    {   
        // get request
        const auto request = session->get_request();

        // get headers
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        // fetch data to access later
        session->fetch(content_length, [] (const shared_ptr<restbed::Session>, const restbed::Bytes&) {});

        // get json from request
        json_type data = get_json<json_type>(*request);

        // validate data
        if (data["DeviceId"].is_null())
            throw status::MISSING_FIELD_DEVICEID;

        // get device from db
        json_type response = this->get(data["DeviceId"]);

        // close
        session->close(restbed::OK,
                       response.dump(),
                       {
                            { "Content-Length", to_string(response.dump().size()) },
                            { "Connection",     "close" }
                       });
    }

    void Devices<nlohmann::json>::POST(const std::shared_ptr<restbed::Session> session)
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

        if (data["man"].is_null())
            throw status::MISSING_FIELD_MAN;

        if (data["mod"].is_null())
            throw status::MISSING_FIELD_MOD;

        if (data["sn"].is_null())
            throw status::MISSING_FIELD_SN;

        // insert device and get id
        // in case autogen was not set, will return null
        if (! data["models"].is_null())
            this->insert(data.get<Device>(), data["models"].get<vector<string>>());
        else
            this->insert(data.get<Device>());

        // close
        session->close(restbed::OK);
    }

    void Devices<nlohmann::json>::DELETE(const std::shared_ptr<restbed::Session> session)
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

        // remove device from DB.
        this->remove(data["DeviceId"]);

        // close
        session->close(restbed::OK);
    }
}
}

// --- SERIALIZERS -------------------
namespace mysqlx
{
    using namespace celeste::resource;

    void row_serializer<Device>::to_row (SerializableRow& row, const celeste::resource::Device& device)
    {
        row.set(0, device.DeviceId);
        row.set(1, device.man);
        row.set(2, device.mod);
        row.set(3, device.sn);
    }

    void row_serializer<Device>::from_row (const SerializableRow& row, Device& device)
    {
        SerializableRow tmp = row; // row.get() is not marked const, hence we need this tmp
        device = Device {
            .DeviceId   = tmp.get(0),
            .man        = tmp.get(1),
            .mod        = tmp.get(2),
            .sn         = tmp.get(3)
        };
    }
}

namespace nlohmann
{
    using namespace celeste::resource;

    void adl_serializer<Device>::to_json(json& j, const Device& device)
    {
        j = json {
            {"DeviceId",    device.DeviceId},
            {"man",         device.man.c_str()},
            {"mod",         device.mod.c_str()},
            {"sn",          device.sn.c_str()}
        };
    }

    void adl_serializer<Device>::from_json(const json& j, Device& device)
    {
        device = Device {
            .DeviceId   = j.at("DeviceId"),
            .man        = j.at("man"),
            .mod        = j.at("mod"),
            .sn         = j.at("sn")
        };
    }
}