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
#include <mysql_devapi.h>
#include <restbed>
#include <mutex>

#include "srv/db/db.hpp"
#include "sunspec/sunspec.hpp"

namespace celeste
{   
namespace resource
{
    class LoggerUpload : public restbed::Resource
    {
    public:
        // --- Constructors ----------
        LoggerUpload(const mysqlx::SessionSettings& dbSettings);

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
        mysqlx::SessionSettings     dbSettings;
        mysqlx::Session             dbSession;
        mysqlx::Schema              celesteDB;
        mysqlx::Table               deviceRecordTable;
        mysqlx::Table               modelRecordTable;
        mysqlx::Table               pointRecordTable;

        std::mutex                  upload_mutex;
    };
}
}

#endif //SERVER_RESOURCE_LOGGER_UPLOAD_HPP
