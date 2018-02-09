/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include <soci/mysql/soci-mysql.h>

#include "srv/error.hpp"
#include "srv/service/common.hpp"

#include "Model.hpp"

using namespace std;
using namespace soci;

namespace celeste
{
namespace resource
{   
    // --- CLASS DEFINITIONS ---------
    Models<nlohmann::json>::Models(const std::string& dbSettings)
        : m_db_settings(dbSettings)
    {
        set_path("/model");
        set_method_handler("GET", [this] (const std::shared_ptr<restbed::Session> session) {GET(session);});
        set_method_handler("POST",   [this] (const std::shared_ptr<restbed::Session> session) {POST(session);});
        set_method_handler("DELETE", [this] (const std::shared_ptr<restbed::Session> session) {DELETE(session);});
    }

    Model Models<nlohmann::json>::get(const std::string& modelId)
    {
        session sql(mysql, m_db_settings);
        Model model;
        sql    << "select * from Model where id = :ModelId",
                use(modelId), into(model);
        return model;
    }

    std::vector<Model>
    Models<nlohmann::json>
    ::get_all()
    {
        session sql(mysql, m_db_settings);
        
        int count;
        sql 
            << "select count(*) from Model",
            into(count);

        vector<Model> mod;
        mod.reserve(count);

        rowset<Model> res = (sql.prepare << "select * from Model");

        for (auto it = res.begin(); it != res.end(); ++it)
            mod.push_back(*it);

        return mod;
    }

    void Models<nlohmann::json>::insert(const value_type& model)
    {
        session sql(mysql, m_db_settings);
        sql    << "insert into Model(id, ns) values(:ModelId, :ns)",
                use(model);
    }

    void Models<nlohmann::json>::remove(const std::string& modelId)
    {
        session sql(mysql, m_db_settings);
        sql    << "delete from Model where id = :ModelId",
                use(modelId);
    }

    void Models<nlohmann::json>::GET(const std::shared_ptr<restbed::Session> session)
    {   
        // get request
        const auto request = session->get_request();

        // get json from parameters
        json_type data = request->get_query_parameters();

        try
        {
            json_type response;

            if (data["ModelId"].is_null())
                response = this->get_all();
            else
                response = this->get(data["ModelId"]);

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
            throw DatabaseError("Could not fetch Model");
        }
    }

    void Models<nlohmann::json>::POST(const std::shared_ptr<restbed::Session> session)
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
        if (data["ModelId"].is_null())
                throw MissingFieldError("ModelId");

        if (data["ns"].is_null())
            data["ns"] = nullptr;

        try
        {
            this->insert(data.get<Model>());
        }
        catch (mysql_soci_error& e)
        {
            if (e.err_num_ == 1062)
                throw runtime_error("Device already exists!");
            else
                throw DatabaseError("code" + to_string(e.err_num_));
        }

        // close
        session->close(restbed::OK);
    }

    void Models<nlohmann::json>::DELETE(const std::shared_ptr<restbed::Session> session)
    {
        // get request
        const auto request = session->get_request();

        // get json from parameters
        json_type data = request->get_query_parameters();

        // validate data
        if (data["ModelId"].is_null())
            throw MissingFieldError("ModelId");

        try
        {
            // remove model from DB.
            this->remove(data["ModelId"]);
            // close
            session->close(restbed::OK);
        }
        catch (mysql_soci_error& e)
        {
            if (e.err_num_ == 1062)
                throw DatabaseError("Model already exists!");
            else
                throw DatabaseError("Could not remove Model");
        }
    }
}
}

// --- SERIALIZERS -------------------
namespace soci
{
    using namespace celeste::resource;

    void type_conversion<Model>::from_base(values const& v, indicator , Model& p)
    {
        p.ModelId    = v.get<string>("id");
        p.ns         = v.get<boost::optional<string>>("ns");
    }

    void type_conversion<Model>::to_base(const Model& p, values& v, indicator& ind)
    {
        v.set("ModelId", p.ModelId);
        v.set("ns", p.ns);
        ind = i_ok;
    }
}

namespace nlohmann
{
    using namespace celeste::resource;

    void adl_serializer<Model>::to_json(json& j, const Model& obj)
    {
        j = json {
            {"ModelId", obj.ModelId},
            {"ns",      obj.ns}
        };
    }

    void adl_serializer<Model>::from_json(const json& j, Model& obj)
    {
        obj.ModelId    = j.at("ModelId");
        obj.ns         = j.at("ns").get<decltype(obj.ns)>();
    }
}