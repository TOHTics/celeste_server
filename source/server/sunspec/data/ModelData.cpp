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
 * @brief Contains class definitions for `ModelData`
 */
//</editor-fold>
#include <boost/property_tree/xml_parser.hpp>
#include "ModelData.hpp"
#include "sunspec/util/exceptions.hpp"
#include "sunspec/util/sdx_tags.hpp"

namespace sunspec
{
namespace data
{
using namespace boost::property_tree;
using node = std::pair<std::string, ptree>;

ModelData ModelData::from_xml( const ptree &model_element )
{
    // Verify that there is data
    if ( model_element.data().empty())
        throw XMLException("Empty value for ModelData.");

    // Get attributes of the subtree model_element
    ptree attr = model_element.get_child(sdx::XML_ATTR);
    if ( attr.empty())
        throw XMLException("Empty attributes for model.");

    // Declare model
    ModelData result;

    // Get attributes and set them to the result
    for ( const node &n : attr )
    {
        std::string attr_name = n.first;
        std::string attr_data = n.second.data();

        if ( attr_name == sdx::SDX_MODEL_ID )
        {
            result.id = attr_data;
        } else if ( attr_name == sdx::SDX_MODEL_NAMESPACE )
        {
            result.ns = attr_data;
        } else if ( attr_name == sdx::SDX_MODEL_INDEX )
        {
            result.x = attr_data;
        }

    }

    // Get points
    for ( const node &pe : model_element )
    {
        std::string element_tag = pe.first;
        if ( element_tag == sdx::SDX_POINT )
        {
            ptree point_element = pe.second;
            PointData p = PointData::from_xml(point_element);
            result.add_point(p);
        }
    }
    return result;
}

ModelData ModelData::from_xml( const std::string &model_record )
{
    // Verify if model record is empty
    if ( model_record.empty())
        throw XMLException("Model record is empty.");

    // Parse XML into a ptree
    std::istringstream iss(model_record);
    ptree xml;

    // Attempt to read XML
    try
    {
        xml_parser::read_xml<ptree>(iss, xml);
    } catch ( xml_parser_error e )
    {
        throw XMLException("Malformed XML near " + model_record);
    }

    try
    {
        // Get the child node which represents the model
        xml = xml.get_child(sdx::SDX_MODEL);
    } catch ( boost::property_tree::ptree_bad_path e )
    {
        throw XMLException("XML Model record does not contain the <" + sdx::SDX_MODEL + "> tag.");
    }

    ModelData result = ModelData::from_xml(xml);
    return result;
}

void ModelData::add_point( const PointData &p )
{
    points.push_back(p);
}

ModelData::iterator ModelData::begin()
{
    return points.begin();
}

ModelData::iterator ModelData::end()
{
    return points.end();
}

ModelData::const_iterator ModelData::cbegin() const
{
    return points.cbegin();
}

ModelData::const_iterator ModelData::cend() const
{
    return points.cend();
}

size_t ModelData::size()
{
    return points.size();
}

ModelData::ModelData( size_t n )
{
    points.reserve(n);
}

}
}