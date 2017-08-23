//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/17/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file DeviceData.hpp
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/17/17.
 * 
 * @brief No description available.
 *
 */
//</editor-fold>
#ifndef SERVER_DEVICEDATA_HPP
#define SERVER_DEVICEDATA_HPP

#include <string>
#include <boost/property_tree/ptree.hpp>
#include "ModelData.hpp"

namespace sunspec
{
    namespace data
    {
        /**
         * @class
         * @author Carlos Brito (carlos.brito524@gmail.com)
         * @date 8/17/17.
         *
         * @brief Represents the data sent in a device record.
         *
         * # Description
         * This class is useful for containing the data of the
         * data received in an SDX-format representation.
         *
         * # TODO
         * Nothing for the moment.
         */
        struct DeviceData {

            typedef std::vector<ModelData> model_list_type;

            std::string cid;    /// Result Correlation ID
            std::string id;     /// Optional Device Id which overrides the `man`, `mod` and `sn` if present
            std::string ifc;    /// Optional Interface Id string
            std::string lid;    /// Logger Id string
            std::string man;    /// Device manufacturer
            std::string mod;    /// Device model
            std::string sn;     /// Device serial number
            std::string ns;     /// Domain namespace for `lid`
            std::string t;      /// Timestamp in RFC3339 format

            model_list_type model_list;

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
            DeviceData(const DeviceData& other) = default;

            /**
             * Adds the `ModelData` instance to the list of
             * models contained by this object.
             * @param model `ModelData` object to be added.
             */
            void add_model(const ModelData& model);

            /**
             * Builds a `DeviceData` object from the XML representation contained
             * in a `ptree`.
             * @param device_element The `ptree` object that contains exactly one
             * device element.
             * @return A `DeviceData` object containing the data
             * @throws XMLError Throws if there was an error parsing the XML. This will mean
             * that the data contained in the `device_element` is empty or is malformed.
             */
            static DeviceData from_xml(const boost::property_tree::ptree& device_element);

            /**
             * Builds a `DeviceData` object from he XML representation contained
             * in a `ptree`.
             * @param device_record The `std::string` record that contains the device
             * XML representation.
             * @return A `DeviceData` object containing the data
             * @throws XMLError Throws if there was an error parsing the XML. This will mean
             * that the data contained in the `device_record` is empty or is malformed.
             */
            static DeviceData from_xml(const std::string& device_record);
        };

    }
}



#endif //SERVER_DEVICEDATA_HPP
