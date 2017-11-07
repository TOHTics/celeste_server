/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 3/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */

#include <iostream>
#include <json.hpp>
#include <functional>
#include "query.hpp"
#include "methods/methods.hpp"

using namespace std;
using json = nlohmann::json;

namespace celeste
{
namespace srv
{
    namespace handler
    {

        template <typename Error>
        void handle_unknown_error(const shared_ptr<restbed::Session> session, Error &&e)
        {
            json rb;
            rb["status"] = restbed::BAD_REQUEST;
            rb["error"] = e.what();
            rb["reason"] = "Undocumented error";

            stringstream ss;
            ss << rb;
            string sb = ss.str();

            session->close(restbed::BAD_REQUEST,
                           sb,
                           {
                                   { "Content-Length", to_string(sb.length()) },
                                   { "Connection",     "close" }
                           });
        }


        void handle_accept(const shared_ptr<restbed::Session> session, const json& rb)
        {
            stringstream ss;
            ss << rb;
            string sb = ss.str();

            session->close(restbed::OK,
                           sb,
                           {
                                   { "Content-Length", to_string(sb.length()) },
                                   { "Connection",     "close" }
                           });
        }

        void query_handler(const shared_ptr<restbed::Session> session, const shared_ptr<mysqlx::Session> dbSession)
        {
            // Get request and length of content
            const auto request = session->get_request();
            size_t content_length = (size_t) request->get_header("Content-Length", 0);

            // Fetch bytes from request and handle using callback function
            session->fetch(content_length, 
                [ request, dbSession ] (const shared_ptr<restbed::Session> session, const restbed::Bytes &body)
                {
                    auto j = query::getLastReading("VOLTAGE", "VOLTMETER", 222, dbSession);
                    handle_accept(session, j);
                });
        }
    }


    namespace resource
    {
        shared_ptr<restbed::Resource> make_query(const string& path, const shared_ptr<mysqlx::Session> dbSession)
        {
            using namespace std::placeholders;
            restbed::Resource *resource = new restbed::Resource;
            resource->set_path(path);
            resource->set_method_handler("POST", bind(handler::query_handler, _1, dbSession));
            return shared_ptr<restbed::Resource>(resource);
        }
    }
}
}