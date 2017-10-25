//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/23/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief Contains class definitions for `SunSpecData`
 */
//</editor-fold>
#include <boost/property_tree/xml_parser.hpp>
#include "SunSpecData.hpp"
#include "sunspec/util/sdx_tags.hpp"
#include "sunspec/util/exceptions.hpp"

namespace sunspec
{
namespace data
{
using namespace boost::property_tree;
using node = std::pair<std::string, ptree>;

void SunSpecData::add_device( const DeviceData &device )
{
    devices.push_back(device);
}

SunSpecData SunSpecData::from_xml( const boost::property_tree::ptree &ss_element )
{
    // Verify that there is data
    if ( ss_element.data().empty())
        throw XMLException("Empty value for ModelData.");

    // Get attributes of the subtree ss_element
    ptree attr = ss_element.get_child(sdx::XML_ATTR);
    if ( attr.empty())
        throw XMLException("Empty attributes for model.");

    // Declare model
    SunSpecData result;

    // Get attributes and set them to the result
    for ( const node &n : attr )
    {
        std::string attr_name = n.first;
        std::string attr_data = n.second.data();

        if ( attr_name == sdx::SDX_SUNSPEC_DATA_VERSION )
        {
            result.v = attr_data;
        }
    }

    // Get devices
    for ( const node &de : ss_element )
    {
        std::string element_tag = de.first;
        if ( element_tag == sdx::SDX_DEVICE )
        {
            ptree device_element = de.second;
            DeviceData d = DeviceData::from_xml(device_element);
            result.add_device(d);
        }
    }
    return result;
}

SunSpecData SunSpecData::from_xml( const std::string &ss_record )
{
    if ( ss_record.empty())
        throw XMLException("SunSpec record must be a non-empty string");

    // Parse XML into a ptree
    std::istringstream iss(ss_record);
    ptree xml;

    // Attempt to read XML
    try
    {
        xml_parser::read_xml<ptree>(iss, xml);
    } catch ( xml_parser_error e )
    {
        throw XMLException("Malformed XML near line " 
                           + std::to_string(e.line())
                           + ": " + e.message()
                           );
    }

    try
    {
        // Get the child node which represents the model
        xml = xml.get_child(sdx::SDX_SUNSPEC_DATA);
    } catch ( ptree_bad_path e )
    {
        throw XMLException("XML Model record does not contain the <" + sdx::SDX_SUNSPEC_DATA + "> tag.");
    }

    SunSpecData result = SunSpecData::from_xml(xml);
    return result;
}

SunSpecData::iterator SunSpecData::begin()
{
    return devices.begin();
}

SunSpecData::iterator SunSpecData::end()
{
    return devices.end();
}

SunSpecData::const_iterator SunSpecData::cbegin() const
{
    return devices.cbegin();
}

SunSpecData::const_iterator SunSpecData::cend() const
{
    return devices.cend();
}

size_t SunSpecData::size() const
{
    return devices.size();
}

SunSpecData::SunSpecData( size_t n )
{
    devices.reserve(n);
}
}
}