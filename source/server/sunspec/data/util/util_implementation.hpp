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
 * - Implement RFC3339 translator.
 */
//</editor-fold>
#ifndef SERVER_UTIL_IMPLEMENTATION_HPP
#define SERVER_UTIL_IMPLEMENTATION_HPP

#include <string>
#include <algorithm>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "../data.hpp"
#include "error.hpp"
#include "sdx_tags.hpp"

namespace sunspec {
    namespace data {
        using namespace boost::property_tree;
        using boost::posix_time::ptime;
        using boost::posix_time::time_from_string;
        namespace {
            typedef std::pair<std::string, ptree> node;
        }

        template <typename T>
        DataPoint<T> from_xml(const ptree &point_element) {
            // Verify that there is data
            std::string data = point_element.data();
            if (data.empty())
                throw XMLError("Empty data value for DataPoint");

            // Get attributes of point element
            ptree attr = point_element.get_child("<xmlattr>");
            if (attr.empty())
                throw XMLError("Empty attributes for point");

            // Build DataPoint
            DataPoint<T> result;
            for (const node &n : attr)
            {
                std::cout << n.first << std::endl;
                if (n.first == SDX_POINT_ID)
                    result.id = n.second.get_value<std::string>();
                else if (n.first == SDX_POINT_SF)
                    result.scale_factor = n.second.get_value<int>();
                else if (n.first == SDX_POINT_INDEX)
                    result.record_index = n.second.get_value<std::string>();
                else if (n.first == SDX_POINT_DESC)
                    result.description = n.second.get_value<std::string>();
                else if (n.first == SDX_POINT_UNITS)
                    result.unit_of_measure = n.second.get_value<std::string>();
                else if (n.first == SDX_POINT_TIME)
                {
                    string timestamp_s = n.second.get_value<std::string>();
                    result.timestamp = RFC3339_TO_PTIME(timestamp_s);
                }
                else
                    throw XMLError("Undefined attribute while parsing DataPoint");
            }
            result.value = point_element.get_value<T>();

            return result;
        }

        template<typename T>
        DataPoint<T> from_xml(const std::string &point_record)
        {
            // Verify point_record isn't empty
            if ( point_record.empty() )
                throw XMLError("Point record is empty");

            // Read the string and convert to a ptree
            std::istringstream iss(point_record);
            ptree xml;
            xml_parser::read_xml<ptree>(iss, xml);

            // Get the child node which represents the point
            xml = xml.get_child(SDX_POINT);

            // Verify that it is a point record
            if ( xml.empty() )
                throw XMLError("XML Point Record does not contain the <p> tag.");

            // Build DataPoint
            DataPoint<T> result = from_xml<T>(xml);

            return result;
        }

        ptime RFC3339_TO_PTIME(const std::string& rfc3339)
        {

            size_t RFC3339_T_POSITION = 10; // Position of 'T'
            std::string std_fmt = rfc3339;

            // Take the 'T' and 'Z' out of the string so
            // Boost can parse it.
            std_fmt[RFC3339_T_POSITION] = ' ';
            std_fmt.pop_back();

            return time_from_string(std_fmt);
        }
    }
}
#endif //SERVER_UTIL_IMPLEMENTATION_HPP
