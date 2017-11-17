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
#include "device.hpp"

using namespace std;

using json = nlohmann::json;

namespace celeste
{
namespace resource
{

    void get_device(const shared_ptr<restbed::Session> session,
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

            auto sch = dbSession->getSchema("Celeste");
            auto table = sch.getTable("Device");

            try
            {
                auto res = table.
                    select("*").
                    where("id = :Id").
                    bind(ValueMap{{"Id", data["id"].get<int>()}}).
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
                             "Could not get Device " + to_string(data["id"].get<int>()),
                             session
                             );
            }
        });

    }

    void save_device(const shared_ptr<restbed::Session> session,
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
            cout << body << endl;

            json data = json::parse(body);

            auto sch = dbSession->getSchema("Celeste");
            auto table = sch.getTable("Device");

            if (data["autogen"].get<bool>())
            {
                int max_id = table.select("MAX(id)").execute().fetchOne().get(0);

                table.
                insert("id", "Client_id", "man", "mod", "sn").
                values(++max_id,
                       data["client_id"].get<int>(),
                       data["man"].get<string>().c_str(),
                       data["mod"].get<string>().c_str(),
                       data["sn"].get<string>().c_str()).
                execute();

                string rb = json(max_id).dump();
                session->close(restbed::OK,
                                   rb,
                                   {
                                       { "Content-Length", to_string(rb.size()) },
                                       { "Connection",     "close" }
                                   });
            } else
            {
                table.insert("id", "Client_id", "man", "mod", "sn").
                values(data["id"].get<int>(),
                       data["client_id"].get<int>(),
                       data["man"].get<string>().c_str(),
                       data["mod"].get<string>().c_str(),
                       data["sn"].get<string>().c_str()).
                execute();
                
                string rb = json(data["id"].get<int>()).dump();
                session->close(restbed::OK,
                                   rb,
                                   {
                                       { "Content-Length", to_string(rb.size()) },
                                       { "Connection",     "close" }
                                   });
            }
        });
    }

    void delete_device(const shared_ptr<restbed::Session> session,
                       const shared_ptr<mysqlx::Session> dbSession)
    {
        const auto request = session->get_request();

        if (! request->has_path_parameter("id"))
        {
            handle_error(restbed::BAD_REQUEST,
                         "Please specify the id of the Device to delete",
                         session);
            return;
        }

        int device_id;
        try
        {
            device_id = stoi(request->get_path_parameter("id"));
        } catch (const exception& e)
        {
            json error = "Device id must be a positive integer!";
            handle_error(restbed::BAD_REQUEST, "Device id must be a positive integer!", session);
        }

        try
        {
            auto sch = dbSession->getSchema("Celeste");
            auto table = sch.getTable("Device");

            table.
            remove().
            where("id = :id").
            bind(ValueMap{{"id", device_id}}).
            execute();

            session->close(restbed::OK,
                           "Succesfully deleted Device " + to_string(device_id) + " and all its records.");
        }
        catch (const mysqlx::Error& e)
        {
            handle_error(restbed::INTERNAL_SERVER_ERROR,
                         "Failed to delete device with id " + to_string(device_id),
                         session);
        }
    }

    void add_model(const shared_ptr<restbed::Session> session,
                   const shared_ptr<mysqlx::Session> dbSession)
    {
        const auto request = session->get_request();
        int device_id = stoi(request->get_path_parameter("DeviceId"));
        string model_id = request->get_path_parameter("ModelId");

        try
        {
            auto sch = dbSession->getSchema("Celeste");
            auto table = sch.getTable("Device_Model");

            auto res = table.select("MAX(idx)").
                where("Device_id = :DeviceId AND Model_id = :ModelId").
                bind(ValueMap{{"DeviceId", device_id}, {"ModelId", model_id.c_str()}}).execute();

            int max_idx = res.fetchOne().get(0);
            table.insert("idx", "Device_id", "Model_id").values(max_idx + 1, device_id, model_id.c_str()).execute();

        } catch (const mysqlx::Error& e)
        {
            handle_error(restbed::INTERNAL_SERVER_ERROR,
                         "Could not add Model " + model_id + " to Device " + to_string(device_id),
                         session);
        }
    }

    shared_ptr<restbed::Resource> make_device(const shared_ptr<mysqlx::Session> dbSession)
    {
        auto resource = make_shared<restbed::Resource>();
        resource->set_paths({
            "/devices",
            "/devices/{id: [0-9]+}"
        });

        resource->set_method_handler("POST", bind(save_device, placeholders::_1, dbSession));
        resource->set_method_handler("DELETE", bind(delete_device, placeholders::_1, dbSession));
        return move(resource);
    }

    shared_ptr<restbed::Resource> make_add_model(const shared_ptr<mysqlx::Session> dbSession)
    {
        auto resource = make_shared<restbed::Resource>();
        resource->set_path("/devices/{DeviceId: [0-9]+}/add_model/{ModelId: [a-zA-Z][a-zA-Z0-9]+}");

        resource->set_method_handler("POST", bind(add_model, placeholders::_1, dbSession));
        return move(resource);
    }
}
}