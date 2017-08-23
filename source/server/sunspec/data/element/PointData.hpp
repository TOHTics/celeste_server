//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/17/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file PointData.hpp
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/17/17.
 * 
 * @brief Contains class definitions for DataPoint.
 *
 */
//</editor-fold>
#ifndef SERVER_DATAPOINT_HPP
#define SERVER_DATAPOINT_HPP

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace sunspec {
    namespace data {

        using std::string;
        using boost::posix_time::ptime;

        /**
         * @class DataPoint
         * @author Carlos Brito (carlos.brito524@gmail.com)
         * @date 8/17/17.
         *
         * @brief A record of some measurement.
         *
         * # Description
         * A `DataPoint` is a record of data that contains information
         * about some measurement (or sample). The measurement is
         * identified with and id and it takes any type of value. We
         * store all fields of data, like the `id` and `description`
         * as `std::strings`. This is because we are only interested in
         * getting the data instead of parsing it. To know what the types
         * of data are then we would have to look at the SDMX spec and assign
         * the types accordingly.
         */
        struct PointData {
            string id;                  /// Name of the data point record (to know what it represents).
            string scale_factor = "0";  /// Exponent of multiplier. I.e. $ 10^(scale_factor) $. Defaults to 0.
            string description;         /// User friendly description of the data point.
            string unit_of_measure;     /// Unit of measure of the value.
            string record_index;        /// Index of record. If we have multiple records of same type we need to differentiate them.
            string timestamp;           /// Time at which the point was obtained in RFC3339 format.
            string value;               /// Value of the data point.

            /**
             * Empty constructor.
             */
            PointData() = default;

            /**
             * Constructs a DataPoint object.
             * @param id Name of the data point.
             * @param value Value of data point of type `T`.
             */
            PointData(string id, string value) : id(id), value(value) {}

            /**
             * Copy constructor.
             */
            PointData(const PointData &) = default;
        };
    }
}

#endif //SERVER_DATAPOINT_HPP
