/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 5/Dec/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#include <soci/mysql/soci-mysql.h>
#include <json.hpp>
#include <utility>

#include "ReadingDispatcher.hpp"
#include "ReadRequest.hpp"

#include "srv/service/common.hpp"

using namespace std;

namespace celeste
{
namespace resource
{   
    using json = nlohmann::json;

    ReadingDispatcher<json>::ReadingDispatcher(const std::string& dbSettings)
    {
        set_path("/reading");
        set_method_handler("GET", [this] (const std::shared_ptr<restbed::Session> session) {GET(session);});
    
        for (int i = 0; i < 10; ++i)
            fetcherPool.emplace(dbSettings);
    }

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const LastReadRequest& req) const
    {
        auto readingFetcher = fetcherPool.acquire_wait();
        auto reading = readingFetcher->fetch<Reading>(req);
        return json(reading);
    }

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const RangeReadRequest& req) const
    {
        auto readingFetcher = fetcherPool.acquire_wait();
        auto reading = readingFetcher->fetch<vector<Reading>>(req);
        return json(reading);
    }

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const AccumulatedReadRequest& req) const
    {
        auto readingFetcher = fetcherPool.acquire_wait();
        auto reading = readingFetcher->fetch<map<string, double>>(req);
        return json(reading);
    }

    void ReadingDispatcher<nlohmann::json>::GET(const std::shared_ptr<restbed::Session> session)
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
            throw status::MISSING_FIELD_DEVICEID;
        if (data["ModelId"].is_null())
            throw status::MISSING_FIELD_MODELID;
        if (data["PointId"].is_null())
            throw status::MISSING_FIELD_POINTID;
        if (data["method"].is_null())
            throw status::MISSING_FIELD_METHOD;

        // Action map
        // TODO:
        // Must replace this whole `if` with a map of functions
        // Namely:
        // map<string, function<response(request)>>
        string response_body;
        int code;
        if (data["method"].get<string>() == "last")
        {
            json response = dispatch(
            LastReadRequest{
                .DeviceId = data["DeviceId"],
                .ModelId  = data["ModelId"],
                .PointId  = data["PointId"]
            });
            response_body = response.dump();
            code = restbed::OK;
        }
        else if (data["method"].get<string>() == "range") 
        {
            json response = dispatch(
            RangeReadRequest{
                .DeviceId = data["DeviceId"],
                .ModelId  = data["ModelId"],
                .PointId  = data["PointId"],
                .start = data["start"],
                .end = data["end"]
            });
            response_body = response.dump();
            code = restbed::OK;
        }
        else if (data["method"].get<string>() == "accumulated")
        {
            json response = dispatch(
            AccumulatedReadRequest{
                .DeviceIds = data["DeviceId"],
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