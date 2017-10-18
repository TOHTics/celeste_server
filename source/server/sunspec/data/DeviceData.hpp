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
 * @brief Contains the class declarations for `DeviceData`
 */
//</editor-fold>
#ifndef SUNSPEC_DEVICEDATA_HPP
#define SUNSPEC_DEVICEDATA_HPP

#include <string>
#include <boost/property_tree/ptree.hpp>
#include "ModelData.hpp"

namespace sunspec
{
namespace data
{
/**
 * @class DeviceData
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/17/17.
 *
 * @brief Represents the data sent in a device record.
 *
 * # Description
 * This class is useful for containing the data of the
 * data received in an SDX-format representation.
 *
 * TODO
 * - Add constructor parameter to reserve space so when we know the number of models
 */
struct DeviceData
{

    typedef std::vector<ModelData> model_list_type;          ///< Type of list of models
    typedef model_list_type::iterator iterator;              ///< Iterator wrapper
    typedef model_list_type::const_iterator const_iterator;  ///< Const iterator wrapper

    std::string cid;    ///< Result Correlation ID
    std::string id;     ///< Optional Device Id which overrides the `man`, `mod` and `sn` if present
    std::string ifc;    ///< Optional Interface Id string
    std::string lid;    ///< Logger Id string
    std::string man;    ///< Device manufacturer
    std::string mod;    ///< Device model
    std::string sn;     ///< Device serial number
    std::string ns;     ///< Domain namespace for `lid`
    std::string t;      ///< Timestamp in RFC3339 format

    model_list_type models;

    bool lid_enabled = true;
    bool common_block_enabled = true;

    /**
     * Empty constructor.
     */
    DeviceData() = default;

    /**
     * Copy constructor.
     * @param other Other `DeviceData` object to copy
     */
    DeviceData( const DeviceData &other ) = default;

    /**
     * Initializes instance with a model list
     * @param models List of models
     */
    DeviceData( const model_list_type &models ) : models(models)
    { }

    /**
     * Initializes by reserving `n` spaces for `n` models.
     * Calling this before hand will increase performance when adding new
     * models.
     * @param n Number of spaces to reserve
     */
    DeviceData( size_t n );

    /**
     * Adds the `ModelData` instance to the list of
     * models contained by this object.
     * @param model `ModelData` object to be added.
     */
    void add_model( const ModelData &model );

    /**
     * Returns an iterator to the first element of the container.
     * If the container is empty, the returned iterator will be equal to `end()`.
     * @return An iterator to the first `ModelData`
     */
    iterator begin();

    /**
     * Returns an iterator to the element following the last element of the container.
     * This element acts as a placeholder; attempting to access it results in undefined
     * behavior.
     * @return Iterator to the element following the last `ModelData`.
     */
    iterator end();

    /**
     * Returns a const iterator to the first element of the container.
     * If the container is empty, the returned iterator will be equal to `end()`.
     * @return An iterator to the first `ModelData`
     */
    const_iterator cbegin() const;

    /**
     * Returns a const iterator to the element following the last element of the container.
     * This element acts as a placeholder; attempting to access it results in undefined
     * behavior.
     * @return Iterator to the element following the last `ModelData`.
     */
    const_iterator cend() const;

    /**
     * Returns the number of models contained by this object
     * @return Number of models contained
     */
    size_t size();

    /**
     * Builds a `DeviceData` object from the XML representation contained
     * in a `ptree`.
     * @param device_element The `ptree` object that contains exactly one
     * device element.
     * @return A `DeviceData` object containing the data
     * @throws XMLException Throws if there was an error parsing the XML. This will mean
     * that the data contained in the `device_element` is empty or is malformed.
     */
    static DeviceData from_xml( const boost::property_tree::ptree &device_element );

    /**
     * Builds a `DeviceData` object from he XML representation contained
     * in a `ptree`.
     * @param device_record The `std::string` record that contains the device
     * XML representation.
     * @return A `DeviceData` object containing the data
     * @throws XMLException Throws if there was an error parsing the XML. This will mean
     * that the data contained in the `device_record` is empty or is malformed.
     */
    static DeviceData from_xml( const std::string &device_record );

    inline DeviceData &operator=( const DeviceData &other ) = default;
};

}
}


#endif //SERVER_DEVICEDATA_HPP
