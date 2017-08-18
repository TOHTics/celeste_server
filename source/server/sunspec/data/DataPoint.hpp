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
 * @brief No description available.
 *
 */
//</editor-fold>
#ifndef SERVER_DATAPOINT_HPP
#define SERVER_DATAPOINT_HPP

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
/**
 * @class DataPoint
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/17/17.
 * 
 * @brief A record of some measurement.
 * 
 * # Description
 * A DataPoint is a record of data that contains information
 * about some measurement (or sample). The measurement is identified with
 * and id and it takes any type of value.
 * 
 * # TODO
 * Nothing for the moment.
 */
template <typename T>
class PointData {
    using namespace boost::posix_time;

    std::string id; /// Name of the data point record (to know what it represents).
    std::string description; /// User friendly description of the data point.
    std::string scale_factor; ///
    std::string timestamp; /// Time at which the point was obtained in RFC3339 format.

    ptime time;

};


#endif //SERVER_DATAPOINT_HPP
