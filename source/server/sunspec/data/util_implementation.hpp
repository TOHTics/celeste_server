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

#include "data.hpp"
#include "error.hpp"
#include "sdx_tags.hpp"

namespace sunspec {
    namespace data {
        using boost::property_tree::ptree;
        using boost::posix_time::ptime;
        namespace {
            typedef std::pair<std::string, ptree> node;
        }

        template <class T>
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
                    result.timestamp = n.second.get_value<ptime>();
                else
                    throw XMLError("Undefined attribute while parsing DataPoint");
            }
            result.value = point_element.get_value<T>();

            return result;
        }
    }
}
#endif //SERVER_UTIL_IMPLEMENTATION_HPP
