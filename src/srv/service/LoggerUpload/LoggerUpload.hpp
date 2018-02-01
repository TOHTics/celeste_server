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

#include "srv/authenticator/CelesteAuthenticator.hpp"

namespace celeste
{   
namespace resource
{
    class LoggerUpload : public restbed::Resource
    {
    public:
        // ==== Constructors =============================
        LoggerUpload(const std::string& dbSettings, size_t max_connections);

        // ==== Public methods ===========================

        /**
         * @brief      Saves the data into the DB.
         *
         * @param[in]  data  SunSpec data packet.
         */
        void persist_data(const sunspec::data::SunSpecData& data);

    private:
        // ==== Private methods ==========================

        /**
         * @brief      Handles a POST request.
         *
         * @param[in]  session  The session
         */
        void POST(const std::shared_ptr<restbed::Session> session);

        void AUTH(const std::shared_ptr<restbed::Session> session,
                  const std::function<void(const std::shared_ptr<restbed::Session>)>& callback);

        template <typename Error>
        void get_parse_error(Error &&e, const std::shared_ptr<restbed::Session> session);

        template <typename Error>
        void get_db_error(Error &&e, const std::shared_ptr<restbed::Session> session);

        // ===== Member attributes =======================
        carlosb::object_pool<soci::session>         m_sql_pool;
        carlosb::object_pool<CelesteAuthenticator>  m_auth_pool;
        std::string                                 m_db_settings;
    };
}
}

#endif //SERVER_RESOURCE_LOGGER_UPLOAD_HPP
