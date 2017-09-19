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
#include "sunspec/util/sdx_tags.hpp"
#include "sunspec/util/exceptions.hpp"
#include "DeviceResult.hpp"

using namespace boost::property_tree;
using node = std::pair<std::string, ptree>;

void add_attribute( std::string attr, std::string tag, ptree &tree )
{
    if ( !attr.empty())
        tree.put("<xmlattr>." + tag, attr);
}

namespace sunspec
{
namespace data
{
std::string DeviceResult::to_xml( const DeviceResult &dresult,
                                  std::shared_ptr<boost::property_tree::ptree> ptOut )
{
    // Body of response
    ptree body;

    // Put response message
    if ( !dresult.message.empty())
        body.put(sdx::SDX_DRESULT_MESSAGE, dresult.message);

    // Put dresult code
    if ( !dresult.code.empty())
        body.put(sdx::SDX_DRESULT_CODE, dresult.code);

    // Put dresult reason
    if ( !dresult.reason.empty())
        body.put(sdx::SDX_DRESULT_REASON, dresult.reason);

    // Add device data
    add_attribute(dresult.devData.id, sdx::SDX_DEVICE_ID, body);
    add_attribute(dresult.devData.sn, sdx::SDX_DEVICE_SN, body);
    add_attribute(dresult.devData.ns, sdx::SDX_DEVICE_NS, body);
    add_attribute(dresult.devData.mod, sdx::SDX_DEVICE_MOD, body);
    add_attribute(dresult.devData.man, sdx::SDX_DEVICE_MAN, body);
    add_attribute(dresult.devData.cid, sdx::SDX_DEVICE_CID, body);
    add_attribute(dresult.devData.ifc, sdx::SDX_DEVICE_IFC, body);
    add_attribute(dresult.devData.t, sdx::SDX_DEVICE_TIME, body);
    add_attribute(dresult.devData.lid, sdx::SDX_DEVICE_LID, body);

    // XML response
    ptree xml;
    // Put body
    xml.put_child(sdx::SDX_DRESULT, body);

    // Write XML to stream
    std::ostringstream oss;
    write_xml(oss, xml);

    // Return the ptree if necessary
    if ( ptOut != nullptr )
    {
        *ptOut = xml;
    }
    return oss.str();
}

DeviceResult DeviceResult::from_xml( const ptree &dresult_tr )
{
    // Declare result
    DeviceResult result;

    // Get attributes of point element
    ptree attr = dresult_tr.get_child(sdx::XML_ATTR);
    if ( attr.empty())
        throw XMLException("Empty attributes for DeviceResult");

    // Get data of device result
    for ( const node &n : dresult_tr )
    {
        std::string field_name = n.first;
        std::string field_data = n.second.data();

        if ( field_name == sdx::SDX_DRESULT_REASON )
        {
            result.reason = field_data;
        } else if ( field_name == sdx::SDX_DRESULT_CODE )
        {
            result.code = field_data;
        } else if ( field_name == sdx::SDX_DRESULT_MESSAGE )
        {
            result.message = field_data;
        } else if ( field_name == sdx::XML_ATTR )
        {
            continue;
        } else throw XMLException("Unrecognized field: " + field_name);

    }

    // The attributes of the DeviceResult are the attributes of a DeviceData object
    DeviceData devData;
    for ( const node &n : attr )
    {
        std::string attr_data = n.second.data();
        std::string attr_tag = n.first;
        if ( attr_tag == sdx::SDX_DEVICE_LID )
        {
            devData.lid = attr_data;
        } else if ( attr_tag == sdx::SDX_DEVICE_ID )
        {
            devData.id = sdx::SDX_DEVICE_ID;
        } else if ( attr_tag == sdx::SDX_DEVICE_CID )
        {
            devData.cid = sdx::SDX_DEVICE_CID;
        } else if ( attr_tag == sdx::SDX_DEVICE_IFC )
        {
            devData.ifc = attr_data;
        } else if ( attr_tag == sdx::SDX_DEVICE_MAN )
        {
            devData.man = attr_data;
        } else if ( attr_tag == sdx::SDX_DEVICE_MOD )
        {
            devData.mod = attr_data;
        } else if ( attr_tag == sdx::SDX_DEVICE_NS )
        {
            devData.ns = attr_data;
        } else if ( attr_tag == sdx::SDX_DEVICE_SN )
        {
            devData.sn = attr_data;
        } else if ( attr_tag == sdx::SDX_DEVICE_TIME )
        {
            devData.t = attr_data;
        } else
            throw XMLException("Unrecognized attribute while parsing a DeviceResult element");
    }
    result.devData = devData;

    return result;
}

DeviceResult DeviceResult::from_xml( const std::string &dresult )
{
    // Verify point_record isn't empty
    if ( dresult.empty())
        throw XMLException("Device result is empty.");

    // Read the string and convert to a ptree
    std::istringstream iss(dresult);
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
        // Get the child node which represents the point
        xml = xml.get_child(sdx::SDX_DRESULT);
    } catch ( ptree_bad_path e )
    {
        throw XMLException("XML DeviceResult does not contain the <" + sdx::SDX_DRESULT + "> tag.");
    }

    // Build PointData
    DeviceResult result = DeviceResult::from_xml(xml);
    return result;
}
}
}


