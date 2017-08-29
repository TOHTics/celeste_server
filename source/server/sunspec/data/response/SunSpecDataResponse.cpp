//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/29/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file SunSpecDataResponse.hpp
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/29/17.
 * 
 * @brief No description available.
 *
 * TODO
 * ----
 * Nothing for the moment.
 */
//</editor-fold>
#include <sstream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "SunSpecDataResponse.hpp"
#include "../util/SDX_Tags.hpp"

namespace sunspec
{
    namespace data
    {
        std::string SunSpecDataResponse::to_xml(const SunSpecDataResponse &response,
                                                std::shared_ptr<boost::property_tree::ptree> ptOut)
        {
            using namespace boost::property_tree;

            ptree body;

            body.put(sdx::SDX_RESPONSE_STATUS, std::to_string(response.status)); // Status is mandatory

            if (! response.message.empty())
                body.put(sdx::SDX_RESPONSE_MESSAGE, response.message);

            if (! response.code.empty())
                body.put(sdx::SDX_RESPONSE_CODE, response.code);

            if (! response.reason.empty())
                body.put(sdx::SDX_RESPONSE_REASON, response.reason);

            ptree xml;
            xml.put_child(sdx::SDX_SUNSPEC_DATA_RESPONSE, body);

            std::ostringstream oss;
            write_xml(oss, xml);

            if (ptOut != nullptr)
                *ptOut = xml;
            return oss.str();
        }
    }
}