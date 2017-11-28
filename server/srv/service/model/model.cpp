/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include <iostream>
#include "srv/db/db.hpp"
#include "srv/service/common.hpp"
#include "model.hpp"

using namespace std;

using json = nlohmann::json;

namespace celeste
{
namespace resource
{

    void get_model(const std::shared_ptr<restbed::Session> session,
                   const std::shared_ptr<mysqlx::Session> dbSession)
    {
        const auto request = session->get_request();
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        session->fetch(content_length,
        [ request, dbSession ] (const shared_ptr<restbed::Session> session, const restbed::Bytes &bytes)
        {
            string body;
            bytes2string(bytes, body);

            json data = json::parse(body);

            auto sch = dbSession->getSchema("Celeste");
            auto table = sch.getTable("Model");

            try
            {
                auto res = table.
                select("*").
                where("id = :ModelId").
                bind(ValueMap{{"ModelId", data["ModelId"].get<string>().c_str()}}).
                execute();

                string rb = json(move(res))[0].dump();
                session->close(restbed::OK,
                               rb,
                               {
                                   { "Content-Length", to_string(rb.size()) },
                                   { "Connection",     "close" }
                               });
            } catch (const mysqlx::Error& e)
            {
                handle_error(restbed::INTERNAL_SERVER_ERROR,
                             "Could not get Device " + data["ModelId"].get<string>(),
                             session);
            } catch (const json::exception& e)
            {
                handle_error(restbed::BAD_REQUEST,
                             "",
                             session);
            }
        });

    }

    void save_model(const shared_ptr<restbed::Session> session,
                     const shared_ptr<mysqlx::Session> dbSession)
    {
        const auto request = session->get_request();
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        session->fetch(content_length,
        [ request, dbSession ] (const shared_ptr<restbed::Session> session, const restbed::Bytes &bytes)
        {
            string body;
            bytes2string(bytes, body);

            json data = json::parse(body);

            try
            {
                auto sch = dbSession->getSchema("Celeste");
                auto table = sch.getTable("Model");

                table.
                insert("id", "ns").
                values(data["ModelId"].get<string>().c_str(),
                       data["ns"].get<string>().c_str()).
                execute();

                session->close(restbed::OK);
            }
            catch (const mysqlx::Error& e)
            {
                handle_error(restbed::INTERNAL_SERVER_ERROR,
                             "Could not save Model", session);
            }
        });
    }

    void delete_model(const std::shared_ptr<restbed::Session> session,
                       const std::shared_ptr<mysqlx::Session> dbSession)
    {
        const auto request = session->get_request();
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        json data;
        session->fetch(content_length,
        [ request, dbSession, &data] (const shared_ptr<restbed::Session> session, const restbed::Bytes &bytes)
        {
            // Get body
            string body;
            bytes2string(bytes, body);

            data = json::parse(body);
        });

        int model_id = data["ModelId"];
        try
        {
            auto sch = dbSession->getSchema("Celeste");
            auto table = sch.getTable("Model");

            table.
            remove().
            where("id = :ModelId").
            bind(ValueMap{{"ModelId", model_id}}).
            execute();

            session->close(restbed::OK);
        }
        catch (const mysqlx::Error& e)
        {
            handle_error(restbed::INTERNAL_SERVER_ERROR,
                         "Failed to delete model with id " + to_string(model_id),
                         session);
        }
    }

    shared_ptr<restbed::Resource> make_model(const shared_ptr<mysqlx::Session> dbSession)
    {
        auto resource = make_shared<restbed::Resource>();
        resource->set_path("models/");
        resource->set_method_handler("POST", bind(save_model, placeholders::_1, dbSession));
        resource->set_method_handler("GET", bind(get_model, placeholders::_1, dbSession));
        resource->set_method_handler("DELETE", bind(delete_model, placeholders::_1, dbSession));

        return move(resource);
    }
}
}