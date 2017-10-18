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
 * @brief Contains class declarations for `SunSpecDataResponse`
 */
//</editor-fold>
#ifndef SUNSPEC_SUNSPECDATARESPONSE_HPP
#define SUNSPEC_SUNSPECDATARESPONSE_HPP

#include <string>
#include "DeviceResult.hpp"


namespace sunspec
{
namespace data
{
/**
 * @class SunSpecDataResponse
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/29/17.
 *
 * @brief Describes the contents of a `SunSpecData` response
 *
 * # Description
 * Describes the response sent by a Host following the SunSpec interface.
 */
struct SunSpecDataResponse
{

    typedef std::vector<DeviceResult> device_result_list; ///< Device result type
    typedef device_result_list::iterator iterator;           ///< Iterator wrapper
    typedef device_result_list::const_iterator const_iterator;     ///< Const iterator wrapper


    int status = 0;     ///< Mandatory HTTP status
    std::string code;           ///< Status code as per SDX spec
    std::string reason;         ///< Information about where the error was detected.
    std::string message;        ///< User-friendly error information

    device_result_list devResults; ///< Results of device processing

    /**
     * Empty constructor
     */
    SunSpecDataResponse() = default;

    /**
     * Copy constructor.
     * @param other Other instance to copy
     */
    SunSpecDataResponse( const SunSpecDataResponse &other ) = default;

    /**
     * Initializes with a device resuls list.
     * @param dresults List of device results
     */
    SunSpecDataResponse( const device_result_list &dresults ) : devResults(dresults)
    { }

    /**
     * Initializes instance with `n` spaces for `n` device results.
     * Calling beforehand will increase performance when adding new device results
     * @param n Number of spaces to reserve
     */
    SunSpecDataResponse( size_t n );

    /**
     * Returns an iterator to the first element of the container.
     * If the container is empty, the returned iterator will be equal to `end()`.
     * @return An iterator to the first `DeviceResult`
     */
    iterator begin();

    /**
     * Returns an iterator to the element following the last element of the container.
     * This element acts as a placeholder; attempting to access it results in undefined
     * behavior.
     * @return Iterator to the element following the last `DeviceResult`.
     */
    iterator end();

    /**
     * Returns a const iterator to the first element of the container.
     * If the container is empty, the returned iterator will be equal to `end()`.
     * @return An iterator to the first `DeviceResult`
     */
    const_iterator cbegin();

    /**
     * Returns a const iterator to the element following the last element of the container.
     * This element acts as a placeholder; attempting to access it results in undefined
     * behavior.
     * @return Iterator to the element following the last `DeviceResult`.
     */
    const_iterator cend();

    /**
     * Returns the number of device results contained by this object
     * @return Number of device results contained
     */
    size_t size();

    /**
     * Adds an instance of a `DeviceResult` to the `SunSpecDataResponse`.
     * @param dresult Instance of `DeviceResult` containing information about
     * the error of the processing of the device record.
     */
    void add_device_result( const DeviceResult &dresult );

    /**
     * Returns the XML representation of the `SunSpecDataResponse` instance.
     * @param response The instance to convert to XML.
     * @param ptOut An optional output parameter which is the element tree (`ptree`) representation of the XML
     * @return The XML representation of the instance as a `std::string`
     */
    static std::string to_xml( const SunSpecDataResponse &response,
                               std::shared_ptr<boost::property_tree::ptree> ptOut = nullptr );

    /**
     * Converts the `std::string` XML specification to a `SunSpecDataResponse` object.
     * @param data_response The string containing the XML spec.
     * @return The instance of `SunSpecDataResponse` containing the information.
     */
    static SunSpecDataResponse from_xml( const std::string &data_response );

    /**
     * Converts the `ptree` XML specification to a `SunSpecDataResponse` object.
     * @param data_response The property tree containing the XML spec.
     * @return The instance of `SunSpecDataResponse` containing the information.
     */
    static SunSpecDataResponse from_xml( const boost::property_tree::ptree &data_response_tree );

    /**
     * Overload stream injection operator.
     * @param os `ostream` object
     * @param obj Object to inject
     * @return Reference to the modified `os` instance
     */
    friend std::ostream &operator<<( std::ostream &os, const SunSpecDataResponse &obj );
};
}
}

#endif //SERVER_SUNSPECDATARESPONSE_HPP
