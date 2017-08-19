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
 */
//</editor-fold>
#ifndef SERVER_UTIL_HPP
#define SERVER_UTIL_HPP

#include <boost/property_tree/xml_parser.hpp>

#include "../data.hpp"

namespace sunspec {
    namespace data {
        using boost::property_tree::ptree;

        /**
         * Builds a DataPoint out of a SDX specification. This specification
         * can be found in the SunSpec Data Model Exchange Specification and
         * its syntax is XML.
         * @tparam T Value type of the DataPoint. It can be many things such
         * as `float`, `uint16`, etc.
         * @param point_element The `ptree` (XML) element of the point record.
         * @return Returns a DataPoint object with the same data as the element.
         */
        template<typename T>
        DataPoint<T> from_xml(const ptree &point_element);

        /**
         * Builds a DataPoint out of a SDX specification. This specification
         * can be found in the SunSpec Data Model Exchange Specification and
         * its syntax is XML.
         * @tparam T Value type of the DataPoint. It can be many things such
         * as `float`, `uint16`, etc.
         * @param point_record A `std::string` containing the XML representat-
         * ion of the point record.
         * @return Returns a DataPoint object with the same data as the XML
         * representation passed.
         */
        template<typename T>
        DataPoint<T> from_xml(const string &point_record);

        /**
         * Converts a RFC3339 formatted string to ptime.
         * @param rfc3339 RFC3339 formatted timestamp.
         * @return Returns the `ptime` value.
         */
        ptime RFC3339_TO_PTIME(const std::string& rfc3339);
    }
}

#include "util_implementation.hpp"

#endif //SERVER_UTIL_HPP
