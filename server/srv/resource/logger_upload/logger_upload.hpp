/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 24/08/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains class declarations.
 */
#ifndef SERVER_RESOURCE_LOGGER_UPLOAD_HPP
#define SERVER_RESOURCE_LOGGER_UPLOAD_HPP

#include <memory>
#include <restbed>
#include <mysql_devapi.h>
#include "sunspec/data/SunSpecData.hpp"

namespace celeste
{
namespace srv
{
    namespace handler
    {
    /**
     * @brief      Method used to handle a request to the Logger Upload Resource.
     * 
     * @param[in]  session  The client-server session
     * 
     * This method tries to stick to the SunSpec Data Exchange HTTP communication protocol.
     */
    void logger_upload_handler(const std::shared_ptr<restbed::Session> session);


    /**
     * @brief      Handles an unknown error. Replies with an XML SunSpecDataResponse
     * and closes the session.
     *
     * @param[in]  session    Client-Server session.
     * @param[in]  e          Error that was thrown.
     *
     * @tparam     Error      Type of error. Must have member function `e.what()` defined.
     */
    template <typename Error>
    void handle_unknown_error(const std::shared_ptr<restbed::Session> session, Error &&e);

    /**
     * @brief      Handles a database error. Replies with an XML SunSpecDataResponse
     * and closes the session.
     *
     * @param[in]  session    Client-Server session.
     * @param[in]  e          Error that was thrown.
     *
     * @tparam     Error      Type of error. Must have member function `e.what()` defined.
     */
    template <typename Error>
    void handle_db_error(const std::shared_ptr<restbed::Session> session, Error &&e);


    /**
     * @brief      Handles a parsing error that may have ocurred while parsing the received
     * XML encoded SunSpec Data Object. Replies with an XML SunSpecDataResponse and closes
     * the session.
     *
     * @param[in]  session    Client-Server session.
     * @param[in]  e          Error that was thrown.
     *
     * @tparam     Error      Type of error. Must have member function `e.what()` defined.
     */
    template <typename Error>
    void handle_parsing_error(const std::shared_ptr<restbed::Session> session, Error &&e);


    /**
     * @brief      Handles an acceptance response.
     *
     * @param[in]  session  Client-Server session.
     */
    void handle_accept(const std::shared_ptr<restbed::Session> session);


    /**
     * @brief      Persists the received data into the DB.
     *
     * @param[in]  data       Data containing DeviceRecords.
     * @param[in]  dbSession  MySQL database session.
     */
    void persist_data(const sunspec::data::SunSpecData &data, const std::shared_ptr<mysqlx::Session> dbSession);

    }

    namespace resource
    {
    /**
     * @brief      Makes a Logger Upload Resource. If a Logger gains access to
     * this resource then it will get a response as per the SDX spec.
     * 
     * @param[in]  path  Path where the logger upload shall be located.

     * @return     A `restbed::Resource` describing a logger upload.
     * 
     * This resource allows for a Logger to transmit the SDX data
     * to the Host. The host will attempt to parse it and in any case
     * of errors it will respond back with a description of the error
     * as per SDX. If no errors where found, then the Host will respond
     * back with a success message.
     */
    std::shared_ptr<restbed::Resource> make_logger_upload(std::string path);
    }
}
}
#endif //SERVER_RESOURCE_LOGGER_UPLOAD_HPP
