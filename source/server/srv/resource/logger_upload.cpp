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
#include <iostream>
#include <string>
#include "logger_upload.hpp"
#include "../../sunspec/data/data.hpp"
#include "../status_codes.hpp"

namespace solarplant
{
    namespace srv
    {
        namespace handler
        {
            void logger_upload_handler(const std::shared_ptr<restbed::Session> session)
            {
                using namespace restbed;
                using namespace sunspec;

                const auto request = session->get_request();
                size_t content_length = (size_t) request->get_header("Content-Length", 0);

                session->fetch(content_length, [request](const std::shared_ptr<Session> session, const Bytes &body)
                {
                    data::SunSpecDataResponse response;
                    response.status = status_code::SUCCESS;
                    response.message = "Hello from a SunSpec compatible host!";
                    std::string r_body = data::SunSpecDataResponse::to_xml(response);
                    size_t length = r_body.size();


                    session->close(OK, r_body, {{"Content-Length", std::to_string(length)},
                                                       {"Connection", "close"}});
                });

                if (! request->get_body().empty())
                    std::cout << request->get_body().data() << std::endl;
            }
        }

        namespace resource
        {
            std::shared_ptr<restbed::Resource> make_logger_upload(std::string path)
            {
                restbed::Resource *resource = new restbed::Resource;
                resource->set_path(path);
                resource->set_method_handler("POST", handler::logger_upload_handler);
                return std::shared_ptr<restbed::Resource>(resource);
            }
        }
    }
}