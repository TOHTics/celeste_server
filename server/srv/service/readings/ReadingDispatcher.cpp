/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 5/Dec/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */

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

    ReadingDispatcher::ReadingDispatcher(const celeste::SessionSettings& dbSettings, size_t fetcherCount)
        :   dbSettings(dbSettings),
            fetcherPool(fetcherCount, ReadingFetcher(dbSettings))
    {
        set_path("/reading");
        set_method_handler("GET", [this] (const std::shared_ptr<restbed::Session> session) {GET(session);});
    }

    // --- DISPATCH FUNCTIONS --------
    template <>
    json ReadingDispatcher::dispatch(const LastReadRequest& request) const
    {
        lock_guard<std::mutex> lock(mutex);
        auto readingFetcher = *fetcherPool.acquire_wait();
        auto reading = 
            readingFetcher.fetch<reading_type>(request);

        return json(reading);
    }

    template <>
    json ReadingDispatcher::dispatch(const RangeReadRequest& request) const
    {
        auto readingFetcher = *fetcherPool.acquire_wait();
        auto reading = 
            readingFetcher.fetch<vector<reading_type>>(request);

        return json(reading);
    }

    template <>
    json ReadingDispatcher::dispatch(const AccumulatedReadRequest& request) const
    {
        auto readingFetcher = *fetcherPool.acquire_wait();
        auto reading = 
            readingFetcher.fetch<vector<double>>(request);

        map<string, double> deviceTotalMap;
        int i = 0;
        for (const auto& DeviceId : request.DeviceIds)
            deviceTotalMap[DeviceId] = reading[i++];

        return json(deviceTotalMap);
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

        // if (data["DeviceId"].is_null())
        //     throw 400;
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
            json response = dispatch<json>
            (LastReadRequest{
                .DeviceId = data["DeviceId"],
                .ModelId  = data["ModelId"],
                .PointId  = data["PointId"]
            });
            response_body = response.dump();
            code = restbed::OK;
        }
        else if (data["method"].get<string>() == "range") 
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
        else if (data["method"].get<string>() == "accumulated")
        {
            if (data["DeviceIds"].is_null())
                throw status::MISSING_FIELD_DEVICEIDS;

            if (data["DeviceIds"].empty())
                throw status::EMPTY_ARRAY;

            json response = dispatch<json>
            (AccumulatedReadRequest{
                .DeviceIds = data["DeviceIds"],
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