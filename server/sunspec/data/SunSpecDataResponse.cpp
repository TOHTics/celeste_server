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

using namespace boost::property_tree;
using node = std::pair<std::string, ptree>;

namespace sunspec
{
namespace data
{
std::string SunSpecDataResponse::to_xml( const SunSpecDataResponse &response,
                                         std::shared_ptr<boost::property_tree::ptree> ptOut )
{
    // Body of response
    ptree body;

    // Status is mandatory
    body.put(sdx::SDX_RESPONSE_STATUS, std::to_string(response.status));

    // Put response message
    if ( !response.message.empty())
        body.put(sdx::SDX_RESPONSE_MESSAGE, response.message);

    // Put response code
    if ( !response.code.empty())
        body.put(sdx::SDX_RESPONSE_CODE, response.code);

    // Put response reason
    if ( !response.reason.empty())
        body.put(sdx::SDX_RESPONSE_REASON, response.reason);

    if ( !response.devResults.empty())
    {
        for ( auto it = response.devResults.begin(); it != response.devResults.end(); it++ )
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
    if ( ptOut != nullptr )
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

void SunSpecDataResponse::add_device_result( const DeviceResult &dresult )
{
    devResults.push_back(dresult);
}

size_t SunSpecDataResponse::size()
{
    return devResults.size();
}

SunSpecDataResponse::SunSpecDataResponse( size_t n )
{
    devResults.reserve(n);
}

SunSpecDataResponse SunSpecDataResponse::from_xml( const boost::property_tree::ptree &data_response_tr )
{
    // Declare result
    SunSpecDataResponse result;

    // Get all fields of the body
    for ( const node &dre : data_response_tr )
    {
        std::string element_tag = dre.first;
        std::string element_data = dre.second.data();
        if ( element_tag == sdx::SDX_DRESULT )
        {
            ptree dr_element = dre.second;
            DeviceResult dr = DeviceResult::from_xml(dr_element);
            result.add_device_result(dr);
        } else if ( element_tag == sdx::SDX_RESPONSE_STATUS )
        {
            try
            {
                result.status = std::stoi(element_data);
            } catch ( std::invalid_argument e )
            {
                throw XMLException("Status field must be an integer");
            }

        } else if ( element_tag == sdx::SDX_RESPONSE_REASON )
        {
            result.reason = element_data;
        } else if ( element_tag == sdx::SDX_RESPONSE_CODE )
        {
            result.code = element_data;
        } else if ( element_tag == sdx::SDX_RESPONSE_MESSAGE )
        {
            result.message = element_data;
        }
    }
    return result;
}

SunSpecDataResponse SunSpecDataResponse::from_xml( const std::string &data_response )
{
    if ( data_response.empty())
        throw XMLException("Data response XML must be a non-empty string");

    // Parse XML into a ptree
    std::istringstream iss(data_response);
    ptree xml;

    // Attempt to read XML
    try
    {
        xml_parser::read_xml<ptree>(iss, xml);
    } catch ( xml_parser_error e )
    {
        throw XMLException("Malformed XML");
    }

    try
    {
        // Get the child node which represents the model
        xml = xml.get_child(sdx::SDX_SUNSPEC_DATA_RESPONSE);
    } catch ( ptree_bad_path e )
    {
        throw XMLException("XML Model record does not contain the <" + sdx::SDX_SUNSPEC_DATA_RESPONSE + "> tag.");
    }

    SunSpecDataResponse result = SunSpecDataResponse::from_xml(xml);
    return result;
}

std::ostream &operator<<( std::ostream &os, const SunSpecDataResponse &obj )
{
    os << "Status: " << obj.status << "\n"
       << "Code: " << obj.code << "\n"
       << "Reason: " << obj.reason << "\n"
       << "Message: " << obj.message;
    return os;

}
}
}