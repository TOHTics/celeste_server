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
    { };

    template <>
    class ReadingDispatcher<nlohmann::json> : public restbed::Resource
    {
    public:
        using response_type = nlohmann::json;

        // --- CONSTRUCTORS ----------
        ReadingDispatcher(const std::string& dbSettings);

        // --- PUBLIC METHODS --------
        template <class Request>
        response_type dispatch(Request&& request) const;
    private:
        using fetcher_pool_type = carlosb::object_pool<ReadingFetcher>;

        // --- PRIVATE METHODS -------
        void GET(const std::shared_ptr<restbed::Session> session);

        // --- MEMBER ATTRIBUTES -----
        mutable fetcher_pool_type        fetcherPool;
    };

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const LastReadRequest&) const;

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const RangeReadRequest&) const;

    template <>
    nlohmann::json
    ReadingDispatcher<nlohmann::json>::dispatch(const AccumulatedReadRequest&) const;
}
}

#endif