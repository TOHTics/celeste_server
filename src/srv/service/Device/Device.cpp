/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include <ctime>
#include <chrono>
#include <sstream>
#include <cstdlib>
#include <soci/mysql/soci-mysql.h>

#include "srv/error.hpp"
#include "srv/service/common.hpp"
#include "srv/crypt/CelesteEncrypter.hpp"

#include "Device.hpp"

using namespace std;
using namespace soci;

namespace celeste
{
namespace resource
{   
    // --- CLASS DEFINITIONS ---------
    Devices<nlohmann::json>
    ::Devices(const string& dbSettings)
        : m_db_settings(dbSettings)
    {
        set_path("/device");
        set_method_handler("GET", [this] (const shared_ptr<restbed::Session> session) {GET(session);});
        set_method_handler("POST",   [this] (const shared_ptr<restbed::Session> session) {POST(session);});
        set_method_handler("DELETE", [this] (const shared_ptr<restbed::Session> session) {DELETE(session);});
    }

    Device
    Devices<nlohmann::json>
    ::get(const string& deviceId)
    {
        session sql(mysql, m_db_settings);
        Device dev;
        sql << "select * from Device where id = :DeviceId",
            into(dev),
            use(deviceId);
        if (sql.got_data())
            return dev;
        else
            throw runtime_error("Device not found!");        
    }

    std::vector<Device>
    Devices<nlohmann::json>
    ::get_all()
    {
        session sql(mysql, m_db_settings);
        
        int count = 1;
        sql 
            << "select count(*) from Device",
            into(count);

        vector<Device> dev;
        dev.reserve(count);

        rowset<Device> res = (sql.prepare << "select * from Device");

        for (auto it = res.begin(); it != res.end(); ++it)
            dev.push_back(*it);

        return dev;
    }

    void
    Devices<nlohmann::json>
    ::insert(const value_type& device, const string& pwd)
    {
        session sql(mysql, m_db_settings);

        // Generate salt
        string salt;
        srand(time(nullptr));
        time_t seconds_past_epoch = time(0);
        stringstream ss;
        ss << seconds_past_epoch << rand();

        CelesteEncrypter crypt;
        crypt.encrypt(ss.str(), salt);

        std::string encrypted_pwd;
        crypt.encrypt(salt + pwd, encrypted_pwd);

        transaction tr(sql);
        
        // Register Device
        sql << "insert into APIUser(id, pwd, salt, ugroup) values(:DeviceId, :pwd, :salt, :ugroup)",
            use(device.DeviceId),
            use(encrypted_pwd),
            use(salt),
            use(string("Device"));

        sql << "insert into Device(id, APIUser_id, man, model, sn) values(:DeviceId, :DeviceId, :man, :mod, :sn)",
            use(device);

        tr.commit();
    }

    void
    Devices<nlohmann::json>
    ::insert(const value_type& device, const std::string& pwd, const std::vector<std::string>& models)
    {
        session sql(mysql, m_db_settings);

        // Generate salt
        string salt;
        srand(time(nullptr));
        time_t seconds_past_epoch = time(0);
        stringstream ss;
        ss << seconds_past_epoch << rand();

        CelesteEncrypter crypt;
        crypt.encrypt(ss.str(), salt);

        std::string encrypted_pwd;
        crypt.encrypt(salt + pwd, encrypted_pwd);

        transaction tr(sql);

        // Register Device
        sql << "insert into APIUser(id, pwd, salt, ugroup) values(:DeviceId, :pwd, :salt, :ugroup)",
            use(device.DeviceId),
            use(encrypted_pwd),
            use(salt),
            use(string("Device"));

        sql << "insert into Device(id, APIUser_id, man, model, sn) values(:DeviceId, :DeviceId, :man, :mod, :sn)",
            use(device);

        // insert models
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


    void Devices<nlohmann::json>::remove(const string& deviceId)
    {
        session sql(mysql, m_db_settings);

        transaction tr(sql);
        sql << "delete from Device where id = :DeviceId",
            use(deviceId);

        sql << "delete from APIUser where id = :DeviceId",
            use(deviceId);
        tr.commit();
    }

    void
    Devices<nlohmann::json>
    ::GET(const shared_ptr<restbed::Session> session)
    {   
        // get request
        const auto request = session->get_request();

        // get json from parameters
        json_type data = request->get_query_parameters();

        json_type response;
        if (data["DeviceId"].is_null())
            response = this->get_all();
        else
            response = this->get(data["DeviceId"]);   

        // close
        session->close(restbed::OK,
                       response.dump(),
                       {
                            { "Content-Length", to_string(response.dump().size()) },
                            { "Connection",     "close" }
                       });
    }

    void
    Devices<nlohmann::json>
    ::POST(const shared_ptr<restbed::Session> session)
    {
        // get request
        const auto request = session->get_request();

        // get headers
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        // fetch data to access later
        session->fetch(content_length,
        [this] (const shared_ptr<restbed::Session> session, const restbed::Bytes &bytes) {
            // convert to string
            string body;
            bytes2string(bytes, body);

            // convert to json
            nlohmann::json data = nlohmann::json::parse(body);

            // validate data
            if (data["DeviceId"].is_null())
                throw MissingFieldError("DeviceId");

            if (data["man"].is_null())
                throw MissingFieldError("man");

            if (data["mod"].is_null())
                throw MissingFieldError("mod");

            if (data["sn"].is_null())
                data["sn"] = data["DeviceId"];

            if (data["pwd"].is_null())
                throw MissingFieldError("pwd");

            if ((data["pwd"].get<string>().size() < 4) || data["pwd"].get<string>().size() >= 100)
                throw runtime_error("Password must be at least 4 characters and less than 100.");

            try
            {
                if (! data["models"].is_null())
                    this->insert(data.get<Device>(), data["pwd"], data["models"].get<vector<string>>());
                else
                    this->insert(data.get<Device>(), data["pwd"]);

                // close
                session->close(restbed::OK);
            } catch (mysql_soci_error& e)
            {
                if (e.err_num_ == 1062)
                    throw DatabaseError("Device already exists!");
                else
                    throw DatabaseError("Could not insert Device with code: " + to_string(e.err_num_));

            }
        });
    }

    void
    Devices<nlohmann::json>
    ::DELETE(const shared_ptr<restbed::Session> session)
    {
        // get request
        const auto request = session->get_request();

        // get json from parameters
        json_type data = request->get_query_parameters();

        // validate data
        if (data["DeviceId"].is_null())
            throw MissingFieldError("DeviceId");

        try
        {
            this->remove(data["DeviceId"]);
            session->close(restbed::OK);
        } catch (mysql_soci_error& e)
        {
            throw DatabaseError("Could not remove Device.");
        }
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