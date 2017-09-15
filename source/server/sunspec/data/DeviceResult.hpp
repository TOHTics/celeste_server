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
 * @brief Contains class declarations for `DeviceResult`
 */
//</editor-fold>
#ifndef SUNSPEC_DEVICEFAILURE_HPP
#define SUNSPEC_DEVICEFAILURE_HPP


#include <string>
#include <boost/property_tree/ptree.hpp>
#include "DeviceData.hpp"

namespace sunspec
{
namespace data
{
/**
 * @class DeviceResult
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/29/17.
 *
 * @brief Describes the result of processing a device record
 */
struct DeviceResult
{
    std::string reason;     ///< Reason as to why processing the device failed
    std::string message;    ///< User friendly message to describe error
    std::string code;       ///< Status code

    DeviceData devData;     ///< Associated device

    /**
     * Empty constructor.
     */
    DeviceResult() = default;

    /**
     * Copy constructor.
     * @param other Other instance to copy
     */
    DeviceResult( const DeviceResult &other ) = default;

    /**
     * Method to output a `std::string` containing the XML representation of a `DeviceResult`
     * @param dresult Instance of `DeviceResult`
     * @param ptOut Output parameter that if set to something different than `nullptr` it will
     * contain the element tree (`ptree`) representation of the XML.
     * @return `std::string` containing the XML representation of the `dresult`
     */
    static std::string to_xml( const DeviceResult &dresult,
                               std::shared_ptr<boost::property_tree::ptree> ptOut = nullptr );

    /**
     * Converts the `std::string` XML specification to a `DeviceResult` object.
     * @param data_response The string containing the XML specification.
     * @return The instance of `DeviceResult` containing the information.
     */
    static DeviceResult from_xml( const std::string &dresult );

    /**
     * Converts the `ptree` XML specification to a `DeviceResult` object.
     * @param data_response The property tree containing the XML specification.
     * @return The instance of `DeviceResult` containing the information.
     */
    static DeviceResult from_xml( const boost::property_tree::ptree &dresult_tr );
};
}
}


#endif //SERVER_DEVICEFAILURE_HPP
