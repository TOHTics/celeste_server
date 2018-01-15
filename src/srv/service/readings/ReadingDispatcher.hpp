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

#include <mysql_devapi.h>
#include <restbed>
#include <json.hpp>
#include "object_pool.hpp"

#include "ReadingFetcher.hpp"

namespace celeste
{
namespace resource
{   
    class ReadingDispatcher : public restbed::Resource
    {
    public:
        typedef Reading<mysqlx::EnhancedValue> reading_type;

        // --- CONSTRUCTORS ----------
        ReadingDispatcher(const celeste::SessionSettings& dbSettings, size_t workerLimit = 4);

        // --- PUBLIC METHODS --------
        template <class ReadResponse, class ReadRequest>
        ReadResponse dispatch(const ReadRequest& request) const;

    private:
        using fetcher_pool = carlosb::object_pool<ReadingFetcher>;

        // --- PRIVATE METHODS -------
        void GET(const std::shared_ptr<restbed::Session> session);

        // --- MEMBER ATTRIBUTES -----
        celeste::SessionSettings    dbSettings;
        mutable fetcher_pool        fetcherPool;
    };
    
}
}

#endif