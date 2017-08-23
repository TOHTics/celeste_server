//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/23/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file ${HEADER_FILENAME}
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/23/17.
 * 
 * @brief No description available.
 *
 * TODO
 * ----
 * Nothing for the moment.
 */
//</editor-fold>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "PointData.hpp"
#include "../error.hpp"
#include "../util/sdx_tags.hpp"

namespace sunspec
{
namespace data
{
    using namespace boost::property_tree;
    using node = std::pair<std::string, ptree>;

    PointData PointData::from_xml(const ptree &point_element) {
            // Verify that there is data
            if ( point_element.data().empty() )
                throw XMLError("Empty data value for PointData");

            // Get attributes of point element
            ptree attr = point_element.get_child("<xmlattr>");
            if ( attr.empty() )
                throw XMLError("Empty attributes for point");

            // Build PointData
            PointData result;
            for ( const node &n : attr )
            {
                std::string attr_name = n.first;
                std::string attr_data = n.second.data();
                if ( attr_name == SDX_POINT_ID )
                {
                    result.id = attr_data;
                }
                else if ( attr_name == SDX_POINT_SF )
                {
                    result.sf = attr_data;
                }
                else if ( attr_name == SDX_POINT_INDEX )
                {
                    result.x = attr_data;
                }
                else if ( attr_name == SDX_POINT_DESC )
                {
                    result.d = attr_data;
                }
                else if ( attr_name == SDX_POINT_UNITS )
                {
                    result.u = attr_data;
                }
                else if ( attr_name == SDX_POINT_TIME )
                {
                    result.t = attr_data;
                }
                else
                    throw XMLError("Undefined attribute while parsing PointData");
            }
            result.value = point_element.data();

            return result;
        }

        PointData PointData::from_xml(const std::string &point_record)
        {
            // Verify point_record isn't empty
            if ( point_record.empty() )
                throw XMLError("Point record is empty.");

            // Read the string and convert to a ptree
            std::istringstream iss(point_record);
            ptree xml;
            xml_parser::read_xml<ptree>(iss, xml);

            try
            {
                // Get the child node which represents the point
                xml = xml.get_child(SDX_POINT);
            } catch (ptree_bad_path e)
            {
                throw XMLError("XML Point Record does not contain the <p> tag.");
            }

            // Build PointData
            PointData result = PointData::from_xml(xml);
            return result;
        }
}
}
