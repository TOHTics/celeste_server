//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/24/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file logger_upload.hpp
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/24/17.
 * 
 * @brief Contains the function declarations for making a logger upload.
 *
 * TODO
 * ----
 * Nothing for the moment.
 */
//</editor-fold>
#ifndef SERVER_RESOURCE_LOGGER_UPLOAD_HPP
#define SERVER_RESOURCE_LOGGER_UPLOAD_HPP

#include <restbed>

namespace solarplant
{
    namespace srv
    {
        namespace resource
        {
            /**
             * Makes a Logger Upload Resource. If a Logger gains access to
             * this resource then it will get a response as per the SDX spec.
             *
             * This resource allows for a Logger to transmit the SDX data
             * to the Host. The host will attempt to parse it and in any case
             * of errors it will respond back with a description of the error
             * as per SDX. If no errors where found, then the Host will respond
             * back with a success message.
             * @param path Path where the logger upload shall be located
             * @return A `restbed::Resource` describing a logger upload.
             */
            std::shared_ptr<restbed::Resource> make_logger_upload(std::string path);
        }
    }
}
#endif //SERVER_RESOURCE_LOGGER_UPLOAD_HPP
