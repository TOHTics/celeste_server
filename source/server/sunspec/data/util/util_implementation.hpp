//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/18/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file ${HEADER_FILENAME}
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/18/17.
 *
 * @brief No description available.
 *
 * TODO
 * ----
 * Nothing.
 */
//</editor-fold>
#ifndef SERVER_UTIL_IMPLEMENTATION_HPP
#define SERVER_UTIL_IMPLEMENTATION_HPP

#include <string>
#include <algorithm>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "../data.hpp"
#include "../error.hpp"
#include "sdx_tags.hpp"

namespace sunspec {
    namespace data {
        using namespace boost::property_tree;
        using boost::posix_time::ptime;
        using boost::posix_time::time_from_string;
        namespace {
            typedef std::pair<std::string, ptree> node;
        }

        PointData point_from_xml(const ptree &point_element) {
            // Verify that there is data
            std::string data = point_element.data();
            if ( data.empty() )
                throw XMLError("Empty data value for PointData");

            // Get attributes of point element
            ptree attr = point_element.get_child("<xmlattr>");
            if ( attr.empty() )
                throw XMLError("Empty attributes for point");

            // Build PointData
            PointData result;
            for ( const node &n : attr )
            {
                string attr_name = n.first;
                string data = n.second.data();
                if ( attr_name == SDX_POINT_ID )
                {
                    result.id = data;
                }
                else if ( attr_name == SDX_POINT_SF )
                {
                    result.scale_factor = data;
                }
                else if ( attr_name == SDX_POINT_INDEX )
                {
                    result.record_index = data;
                }
                else if ( attr_name == SDX_POINT_DESC )
                {
                    result.description = data;
                }
                else if ( attr_name == SDX_POINT_UNITS )
                {
                    result.unit_of_measure = data;
                }
                else if ( attr_name == SDX_POINT_TIME )
                {
                    result.timestamp = data;
                }
                else
                    throw XMLError("Undefined attribute while parsing PointData");
            }
            result.value = point_element.get_value<string>();

            return result;
        }

        PointData point_from_xml(const std::string &point_record)
        {
            // Verify point_record isn't empty
            if ( point_record.empty() )
                throw XMLError("Point record is empty");

            // Read the string and convert to a ptree
            std::istringstream iss(point_record);
            ptree xml;
            xml_parser::read_xml<ptree>(iss, xml);

            try
            {
                // Get the child node which represents the point
                xml = xml.get_child(SDX_POINT);
            } catch (boost::property_tree::ptree_bad_path e)
            {
                throw XMLError("XML Point Record does not contain the <p> tag.");
            }

            // Build PointData
            PointData result = point_from_xml(xml);

            return result;
        }

        ModelData model_from_xml(const ptree &model_element)
        {
            // Verify that there is data
            std::string data = model_element.data();
            if ( data.empty() )
                throw XMLError("Empty value for ModelData.");

            // Get attributes of the subtree model_element
            ptree attr = model_element.get_child("<xmlattr>");
            if ( attr.empty() )
                throw XMLError("Empty attributes for model. At least the id is mandatory.");

            // Declare model
            ModelData result;

            // Get attributes and set them to the result
            for (const node& n : attr)
            {
                string attr_name = n.first;
                string data = n.second.data();

                if ( attr_name == SDX_MODEL_ID )
                {
                    result.id = data;
                } else if ( attr_name == SDX_MODEL_NAMESPACE )
                {
                    result.ns = data;
                } else if ( attr_name == SDX_MODEL_INDEX )
                {
                    result.record_index = data;
                }

            }

            // Get points
            ptree point_elements;
            for ( const node& pe : model_element)
            {
                if ( pe.first == SDX_POINT )
                {
                    PointData p = point_from_xml(pe.second);
                    result.add_point(p);
                }
            }

            return result;
        }
    }
}
#endif //SERVER_UTIL_IMPLEMENTATION_HPP
