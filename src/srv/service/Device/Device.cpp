/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include <mutex>
#include <soci/mysql/soci-mysql.h>
#include "Device.hpp"
#include "srv/service/common.hpp"

using namespace std;
using namespace soci;


namespace celeste
{
namespace resource
{   
    // --- CLASS DEFINITIONS ---------
    Devices<nlohmann::json>::Devices(const std::string& dbSettings)
        : m_dbSettings(dbSettings)
    {
        set_path("/device");
        set_method_handler("GET", [this] (const std::shared_ptr<restbed::Session> session) {GET(session);});
        set_method_handler("POST",   [this] (const std::shared_ptr<restbed::Session> session) {POST(session);});
        set_method_handler("DELETE", [this] (const std::shared_ptr<restbed::Session> session) {DELETE(session);});
    }

    Device Devices<nlohmann::json>::get(const std::string& deviceId)
    {
        session sql(mysql, m_dbSettings);
        Device dev;
        sql << "select * from Device where id = :DeviceId", into(dev), use(deviceId);
        if (sql.got_data())
            return dev;
        else
            throw status::DEVICE_NOT_FOUND;        
    }

    void Devices<nlohmann::json>::insert(const value_type& device)
    {
        session sql(mysql, m_dbSettings);
        sql << "insert into Device(id, man, model, sn) values(:DeviceId, :man, :mod, :sn)",
            use(device);
    }

    void Devices<nlohmann::json>::insert(const value_type& device, std::vector<std::string> models)
    {
        session sql(mysql, m_dbSettings);
        
        transaction tr(sql);
        sql << "insert into Device(id, man, model, sn) values(:DeviceId, :man, :mod, :sn)",
            use(device);

        string modelId;
        statement stmt = (sql.prepare 
                          << "insert into Device_Model(Device_id, Model_id) "
                          << "values(:DeviceId, :ModelId)",
                          use(device.DeviceId), use(modelId));

        for (const auto& id : models)
        {
            modelId = id;
            stmt.execute(true);
        }

        tr.commit();
    }


    void Devices<nlohmann::json>::remove(const std::string& deviceId)
    {
        session sql(mysql, m_dbSettings);
        sql << "delete from Device where id = :DeviceId",
            use(deviceId);
    }

    void Devices<nlohmann::json>::GET(const std::shared_ptr<restbed::Session> session)
    {   
        // get request
        const auto request = session->get_request();

        // get json from parameters
        json_type data = request->get_query_parameters();

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

        // get json from parameters
        json_type data = request->get_query_parameters();

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
namespace soci
{
    using namespace celeste::resource;

    void type_conversion<Device>::from_base(values const& v, indicator , Device& p)
    {
        p.DeviceId   = v.get<string>("id");
        p.man        = v.get<string>("man");
        p.mod        = v.get<string>("model");
        p.sn         = v.get<string>("sn");
    }

    void type_conversion<Device>::to_base(const Device& p, values& v, indicator& ind)
    {
        v.set("DeviceId",     p.DeviceId);
        v.set("man",    p.man);
        v.set("mod",    p.mod);
        v.set("sn",     p.sn);
        ind = i_ok;
    }
}

// --- SERIALIZERS -------------------
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
        device.DeviceId   = j.at("DeviceId");
        device.man        = j.at("man");
        device.mod        = j.at("mod");
        device.sn         = j.at("sn");
    }
}