/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 24/08/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#ifndef SERVER_RESOURCE_LOGGER_UPLOAD1_HPP
#define SERVER_RESOURCE_LOGGER_UPLOAD1_HPP

#include <memory>
#include <restbed>
#include <soci.h>
#include <object_pool.hpp>
#include <sunspec.hpp>

namespace celeste
{   
namespace resource
{
    class LoggerUpload : public restbed::Resource
    {
    public:
        // --- Constructors ----------
        LoggerUpload(const std::string& dbSettings);

        // --- Public methods --------
        void persist_data(const sunspec::data::SunSpecData& data);

    private:
        // --- Private methods -------
        void POST(const std::shared_ptr<restbed::Session> session);

        template <typename Error>
        void get_parse_error(Error &&e, const std::shared_ptr<restbed::Session> session);

        template <typename Error>
        void get_db_error(Error &&e, const std::shared_ptr<restbed::Session> session);

        // --- Member attributes -----
        carlosb::object_pool<soci::session> sqlPool;
    };
}
}

#endif //SERVER_RESOURCE_LOGGER_UPLOAD_HPP
