/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include <mutex>
#include "device.hpp"
#include "srv/service/error.hpp"
#include "srv/service/common.hpp"

using namespace std;

namespace celeste
{
namespace resource
{   
    // --- CLASS DEFINITIONS ---------
    Devices<nlohmann::json>::Devices(const mysqlx::SessionSettings& dbSettings)
        :   dbSession(dbSettings),
            celesteDB(dbSession.getSchema("Celeste")),
            deviceTable(celesteDB.getTable("Device")),
            modelAssociator(dbSettings)
    {
        set_path("/devices");
        set_method_handler("GET", [this] (const std::shared_ptr<restbed::Session> session) {GET(session);});
        set_method_handler("POST",   [this] (const std::shared_ptr<restbed::Session> session) {POST(session);});
        set_method_handler("DELETE", [this] (const std::shared_ptr<restbed::Session> session) {DELETE(session);});
    }

    Device Devices<nlohmann::json>::get(int deviceId)
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

    boost::optional<int> Devices<nlohmann::json>::insert(const value_type& device, bool autogen)
    {
        lock_guard<mutex> guard(sqlMutex);
        Device dtmp = device;
        try
        {
            dbSession.startTransaction();

            if (autogen)
                dtmp.DeviceId = dbSession.sql("SELECT IFNULL(MAX(id) + 1, 0) FROM Device").execute().fetchOne().get(0);

            mysqlx::SerializableRow row;
            row.from(dtmp);

            mysqlx::Row rtmp = row;

            deviceTable.
            insert("id", "Client_id", "man", "mod", "sn").
            values(rtmp).
            execute();

            dbSession.commit();
        } catch (const mysqlx::Error& e)
        {
            dbSession.rollback();
            throw e;
        }
        

        if (autogen)
            return dtmp.DeviceId;
        else
            return boost::none;
    }

    // Note: The actions are already atomic so no need for the mutex lock
    boost::optional<int> Devices<nlohmann::json>::insert(const value_type& device, bool autogen, std::vector<std::string> models)
    {
        boost::optional<int> autogenId = this->insert(device, autogen); // atomic
        int deviceId;
        if (autogenId)
            deviceId = *autogenId;
        else
            deviceId = device.DeviceId;

        // associate the passed models
        for (const auto& modelId : models)
            modelAssociator.associate(deviceId, modelId); // atomic

        return autogenId;
    }


    void Devices<nlohmann::json>::remove(int deviceId)
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
        session->fetch(content_length, [] (const shared_ptr<restbed::Session> session, const restbed::Bytes &bytes) {});

        // get json from request
        json_type data = get_json<json_type>(*request);

        // validate data
        if (data["DeviceId"].is_null())
            throw 400;

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
        bool autogen = true;
        if (data["autogen"].is_null())
            autogen = false;
        else
            autogen = data["autogen"].get<bool>();

        if (data["DeviceId"].is_null())
        {
            data["DeviceId"] = -1;
            if (!autogen)
                throw 400;
        }

        if (data["man"].is_null())
            throw 400;

        if (data["mod"].is_null())
            throw 400;

        if (data["sn"].is_null())
            throw 400;

        data["ClientId"] = nullptr;

        // insert device and get id
        // in case autogen was not set, will return null
        boost::optional<int> autogen_id;

        if (! data["models"].is_null())
            autogen_id = this->insert(data.get<Device>(), autogen, data["models"].get<vector<string>>());
        else
            autogen_id = this->insert(data.get<Device>(), autogen);

        json_type response{{"x", autogen_id}};

        // close
        session->close(restbed::OK,
                       response.dump(),
                       {
                            { "Content-Length", to_string(response.dump().size()) },
                            { "Connection",     "close" }
                       });
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
            throw 400;

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
        row.set(1, EnhancedValue(device.ClientId));
        row.set(2, device.man);
        row.set(3, device.mod.c_str());
        row.set(4, device.sn.c_str());
    }

    void row_serializer<Device>::from_row (const SerializableRow& row, Device& device)
    {
        SerializableRow tmp = row; // row.get() is not marked const, hence we need this tmp
        device = Device {
            .DeviceId   = tmp.get(0),
            .ClientId   = tmp.get(1),
            .man        = tmp.get(2),
            .mod        = tmp.get(3),
            .sn         = tmp.get(4)
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
            {"ClientId",    device.ClientId},
            {"man",         device.man.c_str()},
            {"mod",         device.mod.c_str()},
            {"sn",          device.sn.c_str()}
        };
    }

    void adl_serializer<Device>::from_json(const json& j, Device& device)
    {
        device = Device {
            .DeviceId   = j.at("DeviceId"),
            .ClientId   = j.at("ClientId"),
            .man        = j.at("man"),
            .mod        = j.at("mod"),
            .sn         = j.at("sn")
        };
    }
}