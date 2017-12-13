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
#include <utility>
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

    // --- DISPATCH FUNCTIONS --------
    template <>
    json ReadingDispatcher::dispatch(const LastReadRequest& request) const
    {
        auto reading = 
            reading_fetcher.fetch<reading_type>(move(mysqlx::Session(dbSettings)), request);
        return json(reading);
    }

    template <>
    json ReadingDispatcher::dispatch(const RangeReadRequest& request) const
    {
        auto reading = 
            reading_fetcher.fetch<vector<reading_type>>(move(mysqlx::Session(dbSettings)), request);
        return json(reading);
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

        if (data["DeviceId"].is_null())
            throw 400;
        if (data["ModelId"].is_null())
            throw 400;
        if (data["PointId"].is_null())
            throw 400;
        if (data["method"].is_null())
            throw 400;

        // Action map
        // TODO:
        // Must replace this whole `if` with a map of functions
        // Namely:
        // map<string, function<response(request)>>
        string response_body;
        int code;
        if (data["method"].get<std::string>() == "last")
        {
            json response = dispatch<json>
            (LastReadRequest{
                .DeviceId = data["DeviceId"],
                .ModelId  = data["ModelId"],
                .PointId  = data["PointId"]
            });
            response_body = response.dump();
            code = restbed::OK;
        }
        else if (data["method"].get<std::string>() == "range") 
        {
            json response = dispatch<json>
            (RangeReadRequest{
                .DeviceId = data["DeviceId"],
                .ModelId  = data["ModelId"],
                .PointId  = data["PointId"],
                .start = data["start"],
                .end = data["end"]
            });
            response_body = response.dump();
            code = restbed::OK;
        }
        else
        {
            code = restbed::BAD_REQUEST;
            response_body = "Error: Unknown method.";
        }

        // close
        session->close(code,
                       response_body,
                       {
                            { "Content-Length", to_string(response_body.size()) },
                            { "Connection",     "close" }
                       });
    }
}
}