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
#include <mysql_devapi.h>
#include <restbed>

#include <json.hpp>
#include <unordered_map>

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
        ReadingDispatcher(const mysqlx::SessionSettings& dbSettings);

        // --- PUBLIC METHODS --------
        template <class ReadResponse, class ReadRequest>
        ReadResponse dispatch(const ReadRequest& request) const;

    private:
        // --- PRIVATE METHODS -------
        void GET(const std::shared_ptr<restbed::Session> session);

        // --- MEMBER ATTRIBUTES -----
        ReadingFetcher              reading_fetcher;
        mysqlx::SessionSettings     dbSettings;
    };
    
}
}

#endif