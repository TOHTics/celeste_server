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
#include <fstream>
#include <cstdlib>
#include <soci/mysql/soci-mysql.h>

#include "srv/error.hpp"
#include "srv/service/common.hpp"

#include "srv/service/base64.h"
#include "srv/crypt/CelesteEncrypter.hpp"

#include "APIUser.hpp"


using namespace std;
using namespace soci;


namespace celeste
{
namespace resource
{   
    // --- CLASS DEFINITIONS ---------
    APIUsers<nlohmann::json>
    ::APIUsers(const string& dbSettings, const std::string& config_filename)
        : m_db_settings(dbSettings)
    {
        set_path("/api/user");
        set_method_handler("GET", [this] (const shared_ptr<restbed::Session> session) {GET(session);});
        set_method_handler("PUT", [this] (const shared_ptr<restbed::Session> session) {PUT(session);});
        set_method_handler("POST",   [this] (const shared_ptr<restbed::Session> session) {POST(session);});
        set_method_handler("DELETE", [this] (const shared_ptr<restbed::Session> session) {DELETE(session);});
    
        ifstream i(config_filename);
        if (!i.is_open())
            throw runtime_error("Could not open permissions file: " + string(config_filename));

        json_type j;
        try
        {
            i >> j;
        } catch (exception& e)
        {
            throw runtime_error("Malformed permissions file: " + string(e.what()));
        }
    }

    APIUser
    APIUsers<nlohmann::json>
    ::get(const string& id)
    {
        session sql(mysql, m_db_settings);
        APIUser dev;
        sql << "select * from APIUser where id = :UserId",
            into(dev), use(id);
        if (sql.got_data())
            return dev;
        else
            throw runtime_error("User not found!");        
    }

    void
    APIUsers<nlohmann::json>
    ::insert(const value_type& user, const string& pwd)
    {
        session sql(mysql, m_db_settings);
        transaction tr(sql);

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

        sql << "insert into APIUser(id, ugroup, salt, pwd) values(:UserId, :ugroup, :salt, :pwd)",
            use(user),
            use(salt, "salt"),
            use(encrypted_pwd, "pwd");

        tr.commit();
    }

    void APIUsers<nlohmann::json>::remove(const string& id)
    {
        session sql(mysql, m_db_settings);

        string group;
        sql << "select ugroup from APIUser where id = :id",
            into(group), use(id);
        cout << group << endl;

        if (group == "Device")
        {
            throw BadRequest("Attempting to delete a Device. "
                             "Please use the appropiate method for this.");
        }
        else
        {
            sql << "delete from APIUser where id = :id",
            use(id);
        }
    }

    void
    APIUsers<nlohmann::json>
    ::update(const value_type& new_user)
    {
        session sql(mysql, m_db_settings);
        sql << "update APIUser set ugroup = :ugroup where id = :id",
            use(new_user);
    }

    void
    APIUsers<nlohmann::json>
    ::GET(const shared_ptr<restbed::Session> session)
    {   
        // get request
        const auto request = session->get_request();

        // get json from parameters
        json_type data = request->get_query_parameters();

        // validate data
        if (data["UserId"].is_null())
            throw MissingFieldError("UserId");

        json_type response;
        try
        {
            // get device from db
            response = this->get(data["UserId"]);

            // close
            session->close(restbed::OK,
                           response.dump(),
                           {
                                { "Content-Length", to_string(response.dump().size()) },
                                { "Connection",     "close" }
                           });
        }
        catch (mysql_soci_error& e)
        {
            throw DatabaseError("Could not get User");
        }
    }

    void
    APIUsers<nlohmann::json>
    ::POST(const shared_ptr<restbed::Session> session)
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
        if (data["UserId"].is_null())
            throw MissingFieldError("DeviceId");

        if (data["group"].is_null())
            throw MissingFieldError("group");

        if (data["pwd"].is_null())
            throw MissingFieldError("pwd");

        if ((data["pwd"].get<string>().size() < 4) || data["pwd"].get<string>().size() >= 100)
            throw runtime_error("Password must be at least 4 characters and less than 100.");

        try
        {
            this->insert(data.get<APIUser>(), data["pwd"]);
            session->close(restbed::OK);
        } catch (mysql_soci_error& e)
        {
            if (e.err_num_ == 1062)
                throw DatabaseError("User already exists!");
            else
                throw DatabaseError("Could not insert User with code: " + to_string(e.err_num_));
        }
    }

    void
    APIUsers<nlohmann::json>
    ::PUT(const shared_ptr<restbed::Session> session)
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
        if (data["UserId"].is_null())
            throw MissingFieldError("UserId");

        if (data["group"].is_null())
            throw MissingFieldError("group");

        try
        {
            this->update(data.get<APIUser>());
            session->close(restbed::OK);
        }
        catch (mysql_soci_error& e)
        {
            throw DatabaseError("Could not update User");
        }
    }

    void
    APIUsers<nlohmann::json>
    ::DELETE(const shared_ptr<restbed::Session> session)
    {
        // get request
        const auto request = session->get_request();

        // get json from parameters
        json_type data = request->get_query_parameters();

        // validate data
        if (data["UserId"].is_null())
            throw MissingFieldError("UserId");

        // remove user from DB.
        try
        {
            this->remove(data["UserId"]);
        } catch (mysql_soci_error& e)
        {
            throw DatabaseError("Could not remove User.");
        }
        
        // close
        session->close(restbed::OK);
    }

    void
    APIUsers<nlohmann::json> 
    ::AUTH(const std::shared_ptr<restbed::Session> session,
           const std::function<void(const std::shared_ptr<restbed::Session>)>& callback)
    {

    }
}
}

// --- SERIALIZERS -------------------
namespace soci
{
    using namespace celeste::resource;

    void type_conversion<APIUser>::from_base(values const& v, indicator , APIUser& user)
    {
        user.UserId   = v.get<string>("id");
        user.ugroup   = v.get<string>("ugroup");
    }

    void type_conversion<APIUser>::to_base(const APIUser& user, values& v, indicator& ind)
    {
        v.set("UserId",     user.UserId);
        v.set("ugroup",     user.ugroup);
        ind = i_ok;
    }
}

// --- SERIALIZERS -------------------
namespace nlohmann
{
    using namespace celeste::resource;

    void adl_serializer<APIUser>::to_json(json& j, const APIUser& user)
    {
        // DO NOT SHOW THE PASSWORDS OR SALTS
        j = json {
            {"UserId",  user.UserId},
            {"group",   user.ugroup}
        };
    }

    void adl_serializer<APIUser>::from_json(const json& j, APIUser& user)
    {
        user.UserId     = j.at("UserId");
        user.ugroup     = j.at("group");
    }
}