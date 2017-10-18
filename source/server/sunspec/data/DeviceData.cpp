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
 * @brief Contains class definitions for `DeviceData`
 */
//</editor-fold>
#include <boost/property_tree/xml_parser.hpp>
#include "sunspec/util/exceptions.hpp"
#include "sunspec/util/sdx_tags.hpp"
#include "DeviceData.hpp"

namespace sunspec
{
namespace data
{
using namespace boost::property_tree;
using node = std::pair<std::string, ptree>;

DeviceData DeviceData::from_xml( const ptree &device_element )
{
    // Verify that the element contains data
    if ( device_element.data().empty())
        throw XMLException("Empty data value for a Device element");

    // Verify that there are attributes
    ptree attr = device_element.get_child(sdx::XML_ATTR);
    if ( attr.empty())
        throw XMLException("Empty attributes for a Device element");

    DeviceData d;

    // Get attributes
    for ( const node &n : attr )
    {
        std::string attr_tag = n.first;
        std::string attr_data = n.second.data();
        if ( attr_tag == sdx::SDX_DEVICE_LID )
        {
            d.lid = attr_data;
        } else if ( attr_tag == sdx::SDX_DEVICE_ID )
        {
            d.id = attr_data;
        } else if ( attr_tag == sdx::SDX_DEVICE_CID )
        {
            d.cid = attr_data;
        } else if ( attr_tag == sdx::SDX_DEVICE_IFC )
        {
            d.ifc = attr_data;
        } else if ( attr_tag == sdx::SDX_DEVICE_MAN )
        {
            d.man = attr_data;
        } else if ( attr_tag == sdx::SDX_DEVICE_MOD )
        {
            d.mod = attr_data;
        } else if ( attr_tag == sdx::SDX_DEVICE_NS )
        {
            d.ns = attr_data;
        } else if ( attr_tag == sdx::SDX_DEVICE_SN )
        {
            d.sn = attr_data;
        } else if ( attr_tag == sdx::SDX_DEVICE_TIME )
        {
            d.t = attr_data;
        } else
            throw XMLException("Unrecognized attribute while parsing a Device element");
    }

    // Get models and add them
    for ( const node &n : device_element )
    {
        std::string tag = n.first;
        if ( tag == sdx::SDX_MODEL )
        {
            ptree model_element = n.second;
            ModelData model = ModelData::from_xml(model_element);
            d.add_model(model);
        }
    }
    return d;
}

void DeviceData::add_model( const ModelData &model )
{
    models.push_back(model);
}

DeviceData DeviceData::from_xml( const std::string &device_record )
{
    if ( device_record.empty())
        throw XMLException("Device record must be a non-empty string");

    // Parse XML into a ptree
    std::istringstream iss(device_record);
    ptree xml;

    // Attempt to read XML
    try
    {
        xml_parser::read_xml<ptree>(iss, xml);
    } catch ( xml_parser_error e )
    {
        throw XMLException("Malformed XML near " + device_record);
    }

    try
    {
        // Get the child node which represents the model
        xml = xml.get_child(sdx::SDX_DEVICE);
    } catch ( ptree_bad_path e )
    {
        throw XMLException("XML Model record does not contain the <" + sdx::SDX_DEVICE + "> tag.");
    }

    DeviceData result = DeviceData::from_xml(xml);
    return result;
}

DeviceData::iterator DeviceData::begin()
{
    return models.begin();
}

DeviceData::iterator DeviceData::end()
{
    return models.end();
}

DeviceData::const_iterator DeviceData::cbegin() const
{
    return models.cbegin();
}

DeviceData::const_iterator DeviceData::cend() const
{
    return models.cend();
}

size_t DeviceData::size()
{
    return models.size();
}

DeviceData::DeviceData( size_t n )
{
    models.reserve(n);
}
}
}