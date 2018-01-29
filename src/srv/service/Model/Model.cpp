/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include <soci/mysql/soci-mysql.h>
#include "Model.hpp"
#include "srv/service/common.hpp"

using namespace std;
using namespace soci;

namespace celeste
{
namespace resource
{   
    // --- CLASS DEFINITIONS ---------
    Models<nlohmann::json>::Models(const std::string& dbSettings)
        : m_dbSettings(dbSettings)
    {
        set_path("/model");
        set_method_handler("GET", [this] (const std::shared_ptr<restbed::Session> session) {GET(session);});
        set_method_handler("POST",   [this] (const std::shared_ptr<restbed::Session> session) {POST(session);});
        set_method_handler("DELETE", [this] (const std::shared_ptr<restbed::Session> session) {DELETE(session);});
    }

    Model Models<nlohmann::json>::get(const std::string& modelId)
    {
        session sql(mysql, m_dbSettings);
        Model model;
        sql    << "select * from Model where id = :ModelId",
                use(modelId), into(model);

        if (! sql.got_data())
            throw status::MODEL_NOT_FOUND;

        indicator ind;
        string pointId;
        statement stmt = (sql.prepare << "select * from Point where Model_id = :ModelId",
                          into(pointId, ind), use(modelId)
                          );

        vector<string> pointIds;
        stmt.execute(true);
        pointIds.reserve(50);     
        
        do
        {
            pointIds.push_back(pointId);
        } while (stmt.fetch());
        model.PointIds = move(pointIds);

        return model;
    }

    void Models<nlohmann::json>::insert(const value_type& model)
    {
        session sql(mysql, m_dbSettings);
        sql    << "insert into Model(id, ns) values(:ModelId, :ns)",
                use(model);
    }

    void Models<nlohmann::json>::remove(const std::string& modelId)
    {
        session sql(mysql, m_dbSettings);
        sql    << "delete from Model where id = :ModelId",
                use(modelId);
    }

    void Models<nlohmann::json>::GET(const std::shared_ptr<restbed::Session> session)
    {   
        // get request
        const auto request = session->get_request();

        // get json from parameters
        json_type data = request->get_query_parameters();

        // validate data
        if (data["ModelId"].is_null())
            throw status::MISSING_FIELD_MODELID;

        // get model from db
        json_type response = this->get(data["ModelId"]);

        // close
        session->close(restbed::OK,
                       response.dump(),
                       {
                            { "Content-Length", to_string(response.dump().size()) },
                            { "Connection",     "close" }
                       });
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
                throw status::MISSING_FIELD_MODELID;

        if (data["ns"].is_null())
            data["ns"] = nullptr;

        this->insert(data.get<Model>());

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
            throw status::MISSING_FIELD_MODELID;

        // remove model from DB.
        this->remove(data["ModelId"]);

        // close
        session->close(restbed::OK);
    }
}
}

// --- SERIALIZERS -------------------
namespace soci
{
    using namespace celeste::resource;

    void type_conversion<Model>::from_base(values const& v, indicator , Model& p)
    {
        p = Model {
            .ModelId    = v.get<string>("id"),
            .ns         = v.get<boost::optional<string>>("ns")
        };
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
            {"ns",      obj.ns},
            {"Points",  obj.PointIds}
        };
    }

    void adl_serializer<Model>::from_json(const json& j, Model& obj)
    {
        obj = Model {
            .ModelId    = j.at("ModelId"),
            .ns         = j.at("ns"),
            .PointIds   = j.at("Points")
        };
    }
}