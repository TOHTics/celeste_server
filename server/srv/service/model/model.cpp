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
            // Get body
            string body;
            bytes2string(bytes, body);

            json data = json::parse(body);

            try
            {
                auto sch = dbSession->getSchema("Celeste");
                auto table = sch.getTable("Model");

                table.insert("id", "ns").values(data["id"], data["ns"]).execute();
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
        int id = stoi(request->get_path_parameter("id"));
        
        try
        {
            auto sch = dbSession->getSchema("Celeste");
            auto table = sch.getTable("Model");
            
            table.remove().where("id = :id").bind("id", id);
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
            "models/{id: [0-9]+}"});
        resource->set_method_handler("POST", bind(save_model, placeholders::_1, dbSession));
        resource->set_method_handler("DELETE", bind(delete_model, placeholders::_1, dbSession));

        return move(resource);
    }
}
}