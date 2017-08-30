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
 * @brief Contains class definitions for `DeviceResult`
 */
//</editor-fold>
#include <boost/property_tree/xml_parser.hpp>
#include "DeviceResult.hpp"
#include "../util/SDX_Tags.hpp"

std::string sunspec::data::DeviceResult::to_xml(const sunspec::data::DeviceResult &dresult,
                                                std::shared_ptr<boost::property_tree::ptree> ptOut)
{
    using namespace boost::property_tree;

    // Body of response
    ptree body;

    // Put response message
    if (! dresult.message.empty() )
        body.put(sdx::SDX_DRESULT_MESSAGE, dresult.message);

    // Put dresult code
    if (! dresult.code.empty() )
        body.put(sdx::SDX_DRESULT_CODE, dresult.code);

    // Put dresult reason
    if (! dresult.reason.empty() )
        body.put(sdx::SDX_DRESULT_REASON, dresult.reason);

    // XML response
    ptree xml;
    // Put body
    xml.put_child(sdx::SDX_DRESULT, body);

    // Write XML to stream
    std::ostringstream oss;
    write_xml(oss, xml);

    // Return the ptree if necessary
    if (ptOut != nullptr)
        *ptOut = xml;
    return oss.str();
}

