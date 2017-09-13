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

using namespace restbed;
using namespace sunspec;

namespace solarplant
{
namespace srv
{
namespace handler
{
void handle_parsing_error(const std::shared_ptr<restbed::Session> session, const data::error& e)
{
    data::SunSpecDataResponse ss_r;
    ss_r.status = sdx::FAILURE;
    ss_r.code = "FAILURE";
    ss_r.message = e.what();

    std::string rr_body = data::SunSpecDataResponse::to_xml(ss_r);

    session->close(OK,
                   rr_body,
                   {
                           {"Content-Length", std::to_string(rr_body.size())},
                           {"Connection",     "close"}
                   });
}

void handle_accept(const std::shared_ptr<restbed::Session> session)
{
    data::SunSpecDataResponse ss_r;
    ss_r.status = 400;
    ss_r.code = "SUCCESS";
    ss_r.message = "We have accepted the records :-)";

    std::string rr_b = data::SunSpecDataResponse::to_xml(ss_r);
    session->close(OK,
                   rr_b,
                   {
                           {"Content-Length", std::to_string(rr_b.size())},
                           {"Connection",     "close"}
                   });
}

void logger_upload_handler(const std::shared_ptr<restbed::Session> session)
{
    // Get request and length of content
    const auto request = session->get_request();
    size_t content_length = (size_t) request->get_header("Content-Length", 0);

    // Fetch bytes from request and handle using callback function
    session->fetch(content_length, [request](const std::shared_ptr<Session> session, const Bytes &body)
    {
        std::cout << "Request for a logger_upload from: " << session->get_origin() << std::endl;

        // Read body into char*
        char *req_body_c = (char *) malloc(body.size() + 1);
        snprintf(req_body_c, (int) body.size() + 1, "%s", body.data());

        // Convert char* to string
        std::string req_body_str = req_body_c;

        // Attempt to parse data
        data::SunSpecData data;
        try
        {
            data = data::SunSpecData::from_xml(req_body_str);
        } catch ( data::XMLError e )
        {
            handle_parsing_error(session, e);
            return;
        }

        data::SunSpecData::device_list_type device_list;
        device_list = data.devices;

        // TODO
        // Check device records and verify they stick to the rules.
        // If they don't, then add to the SunSpecDataResponse all
        // the failed devices and why they failed using DeviceResult.


        // TODO
        // Persist device records into db

        // If all went well then handle acceptance response
        handle_accept(session);
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