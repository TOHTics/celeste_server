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
 * @brief Contains the class declarations for `SunSpecData`
 */
//</editor-fold>
#ifndef SUNSPEC_SUNSPECDATA_HPP
#define SUNSPEC_SUNSPECDATA_HPP

#include <vector>
#include <string>
#include "DeviceData.hpp"

namespace sunspec
{
namespace data
{
/**
 * @class SunSpecData
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/17/17.
 *
 * @brief Represents a SunSpecData common block element.
 *
 * # Description
 * No full length description available.
 *
 * TODO
 * Nothing.
 */
struct SunSpecData
{
    typedef std::vector<DeviceData> device_list_type;       ///< List of devices which is just `std::vector<DeviceData>`
    typedef device_list_type::iterator iterator;               ///< Iterator wrapper
    typedef device_list_type::const_iterator const_iterator;         ///< Const iterator wrapper


    std::string v;              ///< Version number
    device_list_type devices;   ///< List of device records

    /**
     * Empty constructor
     */
    SunSpecData() = default;

    /**
     * Copy constructor.
     * @param other Other `SunSpecData` instance to be copied
     */
    SunSpecData( const SunSpecData &other ) = default;

    /**
     * Constructs a `SunSpecData` instance.
     * @param d_list Device list
     */
    SunSpecData( const device_list_type &d_list ) : devices(d_list)
    { }

    /**
     * Reserves `n` spaces for `n` devices. Calling beforehand will
     * increase performance when adding new devices.
     * @param n Number of spaces to reserve
     */
    SunSpecData( size_t n );

    /**
     * Adds a device to the list of device records
     * @param device `DeviceData` instance to add
     */
    void add_device( const DeviceData &device );

    /**
     * Returns an iterator to the first element of the container.
     * If the container is empty, the returned iterator will be equal to `end()`.
     * @return An iterator to the first `DeviceData`
     */
    iterator begin();

    /**
     * Returns an iterator to the element following the last element of the container.
     * This element acts as a placeholder; attempting to access it results in undefined
     * behavior.
     * @return Iterator to the element following the last `DeviceData`.
     */
    iterator end();

    /**
     * Returns a const iterator to the first element of the container.
     * If the container is empty, the returned iterator will be equal to `end()`.
     * @return An iterator to the first `DeviceData`
     */
    const_iterator cbegin() const;

    /**
    * Returns a const iterator to the element following the last element of the container.
    * This element acts as a placeholder; attempting to access it results in undefined
    * behavior.
    * @return Iterator to the element following the last `DeviceData`.
    */
    const_iterator cend() const;


    /**
     * Returns the number of devices contained by this object
     * @return Number of devices contained
     */
    size_t size() const;

    /**
     * Builds a `SunSpecData` instance from a element tree (`ptree`) representation of the
     * SDX spec. This specification can be found in the SunSpec Data Model Exchange
     * Specification and its syntax is XML.
     * @param ss_element The `ptree` object (in XML) element of the model
     * record.
     * @return Returns a `ModelData` object with the same data as the element.
     * @throws XMLException Throws if there was an error parsing the XML. This will mean
     * that the data contained in the `model_element` is empty or is malformed.
     */
    static SunSpecData from_xml( const boost::property_tree::ptree &ss_element );

    /**
     * Builds the `SunSpecData` instance out of a SDX specification. This specification
     * can be found in the SunSpec Data Model Exchange Specification and
     * its syntax is XML.
     * @param model_record A `std::string` containing the XML representation
     * of the model record.
     * @return Returns a `ModelData` object with the same data as the XML
     * representation that was passed.
     * @throws XMLException Throws if there was an error parsing the XML. This will mean
     * that the data contained in the `model_record` is empty or is malformed.
     */
    static SunSpecData from_xml( const std::string &ss_record );
};
}
}


#endif //SERVER_SUNSPECDATA_HPP
