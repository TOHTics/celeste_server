//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/24/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file resource.hpp
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
#ifndef SERVER_RESOURCE_HPP
#define SERVER_RESOURCE_HPP

#include <restbed>

namespace solarplant
{
    namespace srv
    {
        namespace resource
        {
            /**
             * Makes a Logger Upload Resource. If a Logger tries to access
             * this resource then it will get a response as per the SDX spec.
             * @return A `restbed::Resource` describing a logger upload.
             */
            std::shared_ptr<restbed::Resource> make_logger_upload(std::string);
        }
    }
}
#endif //SERVER_RESOURCE_HPP
