/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 5/Dec/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */

#ifndef SERVER_RESOURCE_READING_DISPATCHER_HPP
#define SERVER_RESOURCE_READING_DISPATCHER_HPP

#include <memory>
#include <mutex>
#include <restbed>
#include <json.hpp>
#include "object_pool.hpp"

#include "ReadingFetcher.hpp"

namespace celeste
{
namespace resource
{   
    template <class Response>
    class ReadingDispatcher : public restbed::Resource
    {};

    template <>
    class ReadingDispatcher<nlohmann::json> : public restbed::Resource
    {
    public:
        using response_type = nlohmann::json;
        using dispatch_map_type = std::map<std::string, std::function<response_type(nlohmann::json, int&)>>;
        using fetcher_pool_type = carlosb::object_pool<ReadingFetcher>;

        // --- CONSTRUCTORS ----------
        ReadingDispatcher(const std::string& dbSettings, size_t max_connections);

        // --- PUBLIC METHODS --------
        template <class Request>
        response_type dispatch(const Request& request) const;
    private:
        // --- PRIVATE METHODS -------
        void POST(const std::shared_ptr<restbed::Session> session);

        // --- MEMBER ATTRIBUTES -----
        mutable fetcher_pool_type   fetcherPool;
        dispatch_map_type           dispatch_map;
    };

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const LastReadRequest&) const;

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const RangeReadRequest&) const;

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const YesterdayReadRequest&) const;

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const TodayReadRequest&) const;

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const AccumulatedReadRequest&) const;

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const AverageReadRequest&) const;

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const DayReadRequest&) const;

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const MonthReadRequest&) const;

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const YearReadRequest&) const;
}
}

#endif