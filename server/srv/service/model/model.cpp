/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include "model.hpp"
#include "srv/service/error.hpp"
#include "srv/service/common.hpp"

using namespace std;

namespace celeste
{
namespace resource
{   
    // --- CLASS DEFINITIONS ---------
    Models<nlohmann::json>::Models(const mysqlx::SessionSettings& dbSettings)
        :   dbSession(dbSettings),
            celesteDB(dbSession.getSchema("Celeste")),
            modelTable(celesteDB.getTable("Model"))
    {
        set_path("/Models");
        set_method_handler("GET", [this] (const std::shared_ptr<restbed::Session> session) {GET(session);});
        set_method_handler("POST",   [this] (const std::shared_ptr<restbed::Session> session) {POST(session);});
        set_method_handler("DELETE", [this] (const std::shared_ptr<restbed::Session> session) {DELETE(session);});
    }

    Model Models<nlohmann::json>::get(const std::string& modelId)
    {
        auto res = 
            modelTable.
            select("*").
            where("id = :ModelId").
            bind(ValueMap{{"ModelId", modelId.c_str()}}).
            execute();

        mysqlx::SerializableRow row = res.fetchOne();
        json_type j = row.as<Model>();
        return row.as<Model>();
    }

    void Models<nlohmann::json>::insert(const value_type& model)
    {
        mysqlx::SerializableRow row;
        row.from(model);

        mysqlx::Row tmp = row;

        modelTable.
        insert("id", "ns").
        values(tmp).
        execute();
    }

    void Models<nlohmann::json>::remove(const std::string& modelId)
    {
        modelTable.
        remove().
        where("id = :ModelId").
        bind(ValueMap{{"ModelId", modelId.c_str()}}).
        execute();
    }

    void Models<nlohmann::json>::GET(const std::shared_ptr<restbed::Session> session)
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
            throw 400;

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
                throw 400;

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

        // get headers
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        // fetch data to access later
        session->fetch(content_length, [] (const shared_ptr<restbed::Session> session, const restbed::Bytes &bytes) {});

        // get json from request
        json_type data = get_json<json_type>(*request);

        // validate data
        if (data["ModelId"].is_null())
            throw 400;

        // remove model from DB.
        this->remove(data["ModelId"]);

        // close
        session->close(restbed::OK);
    }
}
}

// --- SERIALIZERS -------------------
namespace mysqlx
{
    using Model = celeste::resource::Model;

    void row_serializer<Model>::to_row (SerializableRow& row, const celeste::resource::Model& obj)
    {
        row.set(0, EnhancedValue{obj.ModelId});
        row.set(1, EnhancedValue{obj.ns});
    }

    void row_serializer<Model>::from_row (const SerializableRow& row, Model& obj)
    {
        SerializableRow tmp = row; // row.get() is not marked const, hence we need this tmp
        obj = Model {
            .ModelId = tmp.get(0).get<std::string>(),
            .ns = tmp.get(1)
        };
    }
}

namespace nlohmann
{
    using namespace celeste::resource;

    void adl_serializer<Model>::to_json(json& j, const Model& obj)
    {
        j = json {
            {"ModelId", obj.ModelId},
            {"ns", obj.ns}
        };
    }

    void adl_serializer<Model>::from_json(const json& j, Model& obj)
    {
        obj = Model {
            .ModelId = j.at("ModelId"),
            .ns = j.at("ns")
        };
    }
}