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
 * @brief Contains the function definitions for the `resource::make_upload` function.
 *
 * TODO
 * ----
 * Nothing for the moment.
 */
//</editor-fold>
#include <memory>
#include <restbed>
#include "logger_upload.hpp"

namespace solarplant
{
    namespace srv
    {
        namespace handler
        {
            void logger_upload_handler(const std::shared_ptr <restbed::Session> session)
            {
                const auto request = session->get_request();

            }
        }

        namespace resource
        {
            std::shared_ptr<restbed::Resource> make_logger_upload()
            {

            }
        }
    }
}