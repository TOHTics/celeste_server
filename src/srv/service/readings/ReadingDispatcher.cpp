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
#include <map>

#include "ReadingDispatcher.hpp"
#include "ReadRequest.hpp"

#include "srv/service/common.hpp"

using namespace std;
using namespace soci;

namespace celeste
{
namespace resource
{   
    using json = nlohmann::json;

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const LastReadRequest& req) const
    {
        auto readingFetcher = fetcherPool.acquire_wait();
        auto reading =
            readingFetcher->fetch<Reading>(req);
        return json(reading);
    }

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const RangeReadRequest& req) const
    {
        auto readingFetcher = fetcherPool.acquire_wait();
        auto reading =
            readingFetcher->fetch<vector<Reading>>(req);
        return json(reading);
    }

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const YesterdayReadRequest& req) const
    {
        auto readingFetcher = fetcherPool.acquire_wait();
        auto reading =
            readingFetcher->fetch<map<string, vector<pair<int, double>>>>(req);
        return json(reading);
    }

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const TodayReadRequest& req) const
    {
        auto readingFetcher = fetcherPool.acquire_wait();
        auto reading =
            readingFetcher->fetch<map<string, vector<pair<int, double>>>>(req);
        return json(reading);
    }

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const AverageReadRequest& req) const
    {
        auto readingFetcher = fetcherPool.acquire_wait();
        auto reading =
            readingFetcher->fetch<map<string, double>>(req);
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

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const DayReadRequest& req) const
    {
        auto readingFetcher = fetcherPool.acquire_wait();
        auto reading =
            readingFetcher->fetch<map<string, vector<pair<int, double>>>>(req);
        return json(reading);
    }

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const MonthReadRequest& req) const
    {
        auto readingFetcher = fetcherPool.acquire_wait();
        auto reading =
            readingFetcher->fetch<map<string, vector<pair<int, double>>>>(req);
        return json(reading);
    }

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const YearReadRequest& req) const
    {
        auto readingFetcher = fetcherPool.acquire_wait();
        auto reading =
            readingFetcher->fetch<map<string, vector<pair<int, double>>>>(req);
        return json(reading);
    }

    ReadingDispatcher<json>::ReadingDispatcher(const std::string& dbSettings, size_t max_connections)
    {
        set_path("/reading");

        set_method_handler("POST", [this] (const std::shared_ptr<restbed::Session> session) {POST(session);});
    

        dispatch_map = dispatch_map_type{
        { "last",
            [this](json data, int& code)
            {
                code = restbed::OK;
                return dispatch(
                LastReadRequest{
                    .DeviceId = data["DeviceId"],
                    .ModelId  = data["ModelId"],
                    .PointId  = data["PointId"]
                });
            }
        },
        { "range",
            [this](json data, int& code)
            {
                if (data["limit"].is_null())
                data["limit"] = 200;

                code = restbed::OK;
                return dispatch(
                RangeReadRequest{
                    .DeviceId = data["DeviceId"],
                    .ModelId  = data["ModelId"],
                    .PointId  = data["PointId"],
                    .start  = data["start"],
                    .end    = data["end"],
                    .limit  = data["limit"]
                });
            }
        },
        { "today",
            [this](json data, int& code)
            {
                code = restbed::OK;
                return dispatch(
                TodayReadRequest{
                    .DeviceIds  = data["DeviceId"],
                    .ModelId    = data["ModelId"],
                    .PointId    = data["PointId"]
                });
            }
        },
        { "yesterday",
            [this](json data, int& code)
            {
                code = restbed::OK;
                return dispatch(
                YesterdayReadRequest{
                    .DeviceIds  = data["DeviceId"],
                    .ModelId    = data["ModelId"],
                    .PointId    = data["PointId"]
                });
            }
        },
        { "year",
            [this](json data, int& code)
            {
                code = restbed::OK;
                return dispatch(
                YearReadRequest{
                    .DeviceIds  = data["DeviceId"],
                    .ModelId    = data["ModelId"],
                    .PointId    = data["PointId"],
                    .year       = data["year"]
                });
            }
        },
        { "month",
            [this](json data, int& code)
            {
                code = restbed::OK;
                return dispatch(
                MonthReadRequest{
                    .DeviceIds  = data["DeviceId"],
                    .ModelId    = data["ModelId"],
                    .PointId    = data["PointId"],
                    .month       = data["month"]
                });
            }
        },
        { "accumulated",
            [this](json data, int& code)
            {
                code = restbed::OK;
                return dispatch(
                AccumulatedReadRequest{
                    .DeviceIds  = data["DeviceId"],
                    .ModelId    = data["ModelId"],
                    .PointId    = data["PointId"],
                    .start      = data["start"],
                    .end        = data["end"]
                });
            }
        },
        { "average",
            [this](json data, int& code)
            {
                code = restbed::OK;
                return dispatch(
                AverageReadRequest{
                    .DeviceIds  = data["DeviceId"],
                    .ModelId    = data["ModelId"],
                    .PointId    = data["PointId"],
                    .start      = data["start"],
                    .end        = data["end"]
                });
            }
        },
        { "day",
            [this](json data, int& code)
            {
                code = restbed::OK;
                return dispatch(
                DayReadRequest{
                    .DeviceIds  = data["DeviceId"],
                    .ModelId    = data["ModelId"],
                    .PointId    = data["PointId"],
                    .day      = data["day"]
                });
            }
        }
        };

        for (int i = 0; i < max_connections; ++i)
            fetcherPool.emplace(dbSettings);
    }

    void ReadingDispatcher<nlohmann::json>::POST(const std::shared_ptr<restbed::Session> session)
    {
        // POST request
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

        // method name
        string method = data["method"].get<string>();

        // response
        response_type response;
        int code = restbed::INTERNAL_SERVER_ERROR;
        
        // dispatch people to find the reading
        auto search = dispatch_map.find(method);
        if (search != dispatch_map.end())
        {
            auto dispatch = search->second;
            response = dispatch(data, code);
        }
        else
        {
            code = restbed::BAD_REQUEST;
            response = "Error: Unknown method.";
        }

        // to string
        string body = response.dump();

        // close
        session->close(code,
                       body,
                       {
                            { "Content-Length", to_string(body.size()) },
                            { "Connection",     "close" }
                       });
    }
}
}