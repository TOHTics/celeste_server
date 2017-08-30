//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/24/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief Contains the function definitions for the `make_logger_upload` function.
 */
//</editor-fold>
#include <memory>
#include <restbed>
#include <iostream>
#include <string>
#include "logger_upload.hpp"
#include "sunspec/data/data.hpp"
#include "sunspec/util/status_code.hpp"

namespace solarplant
{
    namespace srv
    {
        namespace handler
        {
            void handle_error()
            {

            }

            void logger_upload_handler(const std::shared_ptr<restbed::Session> session)
            {
                using namespace restbed;
                using namespace sunspec;

                // Get request and length of content
                const auto request = session->get_request();
                size_t content_length = (size_t) request->get_header("Content-Length", 0);

                // Fetch bytes from request and handle using callback function
                session->fetch(content_length, [ request ](const std::shared_ptr<Session> session, const Bytes &body)
                {
                    // --- Callback function ---//
                    char *body_c = (char*) malloc(body.size());
                    snprintf(body_c, (int) body.size(), "%s", body.data());

                    std::string body_str = body_c;

                    data::SunSpecData data;
                    data::SunSpecDataResponse ss_response;
                    bool error_happened = false;
                    try
                    {
                        data = data::SunSpecData::from_xml(body_str);
                    } catch (data::XMLError e)
                    {
                        ss_response.status = sdx::D_FAILURE;
                        ss_response.code = "D_FAILURE";
                        ss_response.message = e.what();
                        error_happened = true;
                    }

                    if (! error_happened )
                    {
                        // handle data here...

                        // build response
                        ss_response.status = sdx::SUCCESS;
                        ss_response.code = "SUCCESS";
                        ss_response.message = "We have accepted the records :-)";
                    }
                    std::string response_body = data::SunSpecDataResponse::to_xml(ss_response);

                    session->close(OK,
                                   response_body,
                                   {
                                           {"Content-Length", std::to_string(response_body.size())},
                                           {"Connection", "close"}
                                   });
                    // ------ //
                });
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