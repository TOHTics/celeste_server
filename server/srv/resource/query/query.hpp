/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 3/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#ifndef SERVER_RESOURCE_QUERY_HPP
#define SERVER_RESOURCE_QUERY_HPP

#include <memory>
#include <restbed>
#include <mysql_devapi.h>

namespace celeste
{
namespace srv
{
    namespace handler
    {
        /**
         * @brief      Handles an unknown error.
         *
         * @param[in]  session    Client-Server session.
         * @param[in]  e          Error that was thrown.
         *
         * @tparam     Error      Type of error. Must have member function `e.what()` defined.
         */
        template <typename Error>
        void handle_unknown_error(const std::shared_ptr<restbed::Session> session, Error &&e);

        /**
         * @brief      Handles an incoming query.
         *
         * @param[in]  session  Client-Server session
         * 
         */
        void query_handler(const std::shared_ptr<restbed::Session> session,
                           const std::shared_ptr<mysqlx::Session> dbSession);
    }

    namespace resource
    {
        std::shared_ptr<restbed::Resource> make_query(const std::string& path,
                                                      const std::shared_ptr<mysqlx::Session> dbSession);
    }
}
}

#endif