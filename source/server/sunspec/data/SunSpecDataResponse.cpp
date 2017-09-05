//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/29/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief Contains class definitions for `SunSpecDataResponse`
 */
//</editor-fold>
#include <sstream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>
#include "SunSpecDataResponse.hpp"
#include "sunspec/util/sdx_tags.hpp"

namespace sunspec
{
    namespace data
    {
        std::string SunSpecDataResponse::to_xml(const SunSpecDataResponse &response,
                                                std::shared_ptr<boost::property_tree::ptree> ptOut)
        {
            using namespace boost::property_tree;
            using node = std::pair<std::string, ptree>;

            // Body of response
            ptree body;

            // Status is mandatory
            body.put(sdx::SDX_RESPONSE_STATUS, std::to_string(response.status));

            // Put response message
            if (! response.message.empty())
                body.put(sdx::SDX_RESPONSE_MESSAGE, response.message);

            // Put response code
            if (! response.code.empty())
                body.put(sdx::SDX_RESPONSE_CODE, response.code);

            // Put response reason
            if (! response.reason.empty())
                body.put(sdx::SDX_RESPONSE_REASON, response.reason);

            if (! response.devResults.empty())
            {
                for (auto it = response.devResults.begin(); it != response.devResults.end(); it++)
                {
                    std::shared_ptr<ptree> dresult = std::make_shared<ptree>();
                    DeviceResult::to_xml(*it, dresult);
                    ptree dr = dresult->get_child(sdx::SDX_DRESULT); // Get contents
                    body.add_child(sdx::SDX_DRESULT, dr);            // Add contents using SDX_DRESULT as the key
                }
            }

            // XML response
            ptree xml;

            // Put body
            xml.put_child(sdx::SDX_SUNSPEC_DATA_RESPONSE, body);

            // Write XML to stream
            std::ostringstream oss;
            write_xml(oss, xml);

            // Return the ptree if necessary
            if (ptOut != nullptr)
                *ptOut = xml;
            return oss.str();
        }

        SunSpecDataResponse::iterator SunSpecDataResponse::begin()
        {
            return devResults.begin();
        }

        SunSpecDataResponse::iterator SunSpecDataResponse::end()
        {
            return devResults.end();
        }

        SunSpecDataResponse::const_iterator SunSpecDataResponse::cbegin()
        {
            return devResults.cbegin();
        }

        SunSpecDataResponse::const_iterator SunSpecDataResponse::cend()
        {
            return devResults.cend();
        }

        void SunSpecDataResponse::add_device_result(const DeviceResult &dresult)
        {
            devResults.push_back(dresult);
        }

        size_t SunSpecDataResponse::size()
        {
            return devResults.size();
        }

        SunSpecDataResponse::SunSpecDataResponse(size_t n)
        {
            devResults.reserve(n);
        }
    }
}