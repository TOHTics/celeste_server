/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 5/Dec/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */

#include <json.hpp>

#include "ReadingDispatcher.hpp"
#include "ReadRequest.hpp"

#include "srv/service/common.hpp"

#include <iostream>

using namespace std;

namespace celeste
{
namespace resource
{   
    using json = nlohmann::json;

    ReadingDispatcher::ReadingDispatcher(const mysqlx::SessionSettings& dbSettings)
        : dbSettings(dbSettings)
    {
        set_path("/readings");
        set_method_handler("GET", [this] (const std::shared_ptr<restbed::Session> session) {GET(session);});
    }

    template <>
    json ReadingDispatcher::dispatch(const LastReadRequest& request) const
    {
        static mysqlx::Session _temporary_sql_session(dbSettings);

        auto reading = reading_fetcher.fetch<atomic_type>(mysqlx::Session(dbSettings), request);
        
        cout << reading << "\n";
        return json(boost::get<int>(reading));
    }

    void ReadingDispatcher::GET(const std::shared_ptr<restbed::Session> session)
    {
        // get request
        const auto request = session->get_request();

        // get headers
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        // fetch data to access later
        session->fetch(content_length, [] (const shared_ptr<restbed::Session> session, const restbed::Bytes &bytes) {});

        // get json_type from request
        json data = get_json<json>(*request);

        if (data["method"].is_null())
            throw 400;

        string response_body;
        if (data["method"].get<string>() == "last")
        {
            json response = dispatch<json>(LastReadRequest{3, "2", "1"});
            response_body = response.dump();
        }

        cout << response_body << "\n";
    }

}
}