//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/17/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief Contains the class declarations for `PointData`
 *
 */
//</editor-fold>
#ifndef SUNSPEC_POINTDATA_HPP
#define SUNSPEC_POINTDATA_HPP

#include <string>
#include <boost/property_tree/ptree.hpp>

namespace sunspec
{
namespace data
{

/**
 * @class PointData
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/17/17.
 *
 * @brief A record of some measurement.
 *
 * # Description
 * A `PointData` is a record of data that contains information
 * about some measurement (or sample). The measurement is
 * identified with and id and it takes any type of value. We
 * store all fields of data, like the `id` and `description`
 * as `std::string`s. This is because we are only interested in
 * getting the data instead of parsing it. To know what the types
 * of data are then we would have to look at the SDMX spec and assign
 * the types accordingly.
 */
struct PointData
{
    std::string id;        ///< Name of the data point record.
    std::string sf = "0";  ///< The scale factor. I.e. \f$ value \times 10^{sf} \f$. Defaults to 0.
    std::string d;         ///< User friendly description of the data point.
    std::string u;         ///< Unit of measure of the value.
    std::string x;         ///< Index of record. If we have multiple records of same type we need to differentiate them.
    std::string t;         ///< Time at which the point was obtained in RFC3339 format.
    std::string value;     ///< Value of the data point.


    /**
     * Empty constructor.
     */
    PointData() = default;

    /**
     * Constructs a DataPoint object.
     * @param id Name of the data point.
     * @param value Value of data point represented by a `std::string`
     */
    PointData( std::string id, std::string value ) : id(id), value(value)
    { }

    /**
     * Copy constructor.
     */
    PointData( const PointData & ) = default;

    /**
     * Builds a PointData out of a SDX specification. This specification
     * can be found in the SunSpec Data Model Exchange Specification and
     * its syntax is XML.
     * @param point_element The `ptree` object (in XML) element of the point record.
     * @return Returns a `PointData` object with the same data as the element.
     * @throws XMLException Throws if there was an error parsing the XML. This will mean
     * that the data contained in the `point_element` is empty or is malformed.
     */
    static PointData from_xml( const boost::property_tree::ptree &point_element );

    /**
     * Builds a PointData out of a SDX specification. This specification
     * can be found in the SunSpec Data Model Exchange Specification and
     * its syntax is XML.
     * @param point_record A `std::string` containing the XML representation
     * of the point record.
     * @return Returns a `PointData` object with the same data as the XML
     * representation that was passed.
     * @throws XMLException Throws if there was an error parsing the XML. This will mean
     * that the data contained in the `point_record` is empty or is malformed.
     */
    static PointData from_xml( const std::string &point_record );
};
}
}

#endif //SERVER_POINTDATA_HPP
