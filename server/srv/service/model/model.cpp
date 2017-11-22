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

                string rb = json(move(res)).dump();
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
                values(data["id"].get<string>().c_str(),
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
        
        if (! request->has_path_parameter("ModelId"))
        {
            handle_error(restbed::BAD_REQUEST,
                         "You must specify the Id of the Model to delete",
                         session);
            return;
        }

        int id;
        try
        {
            id = stoi(request->get_path_parameter("id"));
        }
        catch (const invalid_argument& e)
        {
            handle_error(restbed::BAD_REQUEST, "Device id must be a positive integer!", session);
            return;
        } catch (const out_of_range& e)
        {
            handle_error(restbed::BAD_REQUEST, "Number too large. Nice try hackers.", session);
            return;
        }
        
        try
        {
            auto sch = dbSession->getSchema("Celeste");
            auto table = sch.getTable("Model");
            
            table.
            remove().
            where("id = :ModelId").
            bind("ModelId", id).
            execute();

            session->close(restbed::OK);
        } catch (const mysqlx::Error& e)
        {
            handle_error(restbed::INTERNAL_SERVER_ERROR,
                         "Could not delete Model " + to_string(id),
                         session);
        }
    }

    shared_ptr<restbed::Resource> make_model(const shared_ptr<mysqlx::Session> dbSession)
    {
        auto resource = make_shared<restbed::Resource>();
        resource->set_paths({
            "models/",
            "models/{ModelId: [0-9]+}"});
        resource->set_method_handler("POST", bind(save_model, placeholders::_1, dbSession));
        resource->set_method_handler("GET", bind(get_model, placeholders::_1, dbSession));
        resource->set_method_handler("DELETE", bind(delete_model, placeholders::_1, dbSession));

        return move(resource);
    }
}
}