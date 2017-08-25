//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/17/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file SunSpecData.hpp
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/17/17.
 * 
 * @brief No description available.
 *
 */
//</editor-fold>
#ifndef SERVER_SUNSPECDATA_HPP
#define SERVER_SUNSPECDATA_HPP

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
         * # TODO
         * Nothing for the moment.
         */
        struct SunSpecData {
            typedef std::vector<DeviceData> device_list; ///< List of devices which is just `std::vector<DeviceData>`

            std::string v;         ///< Version number
            device_list devices;   ///< List of device records

            /**
             * Empty constructor
             */
            SunSpecData() = default;

            /**
             * Copy constructor.
             * @param other Other `SunSpecData` instance to be copied
             */
            SunSpecData(const SunSpecData& other) = default;

            /**
             * Constructs a `SunSpecData` instance.
             * @param v Version number of SDX spec
             * @param d_list Device list
             */
            SunSpecData(std::string v, device_list d_list) : v(v), devices(d_list) {}

            /**
             * Adds a device to the list of device records
             * @param device `DeviceData` instance to add
             */
            void add_device(const DeviceData& device);

            static SunSpecData from_xml(const boost::property_tree::ptree& ss_element );
            static SunSpecData from_xml(const std::string& ss_record);
        };
    }
}



#endif //SERVER_SUNSPECDATA_HPP
