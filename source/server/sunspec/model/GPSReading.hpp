//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/30/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief Contains class declarations for GPS
 */
//</editor-fold>
#ifndef SERVER_GPS_HPP
#define SERVER_GPS_HPP

#include <sunspec/point/Altitude.hpp>
#include <sunspec/data/ModelData.hpp>

namespace sunspec
{
    namespace model
    {
        /**
         * @class GPS
         * @author Carlos Brito (carlos.brito524@gmail.com)
         * @date 8/30/17.
         *
         * @brief Describes a GPS sensor
         *
         * # Description
         * No full length description available.
         */
        struct GPSReading : public data::ModelData
        {
            point::Altitude alt;

            GPSReading(double altitude)
            {
                alt = altitude;

                points.push_back(alt);
            }
        };

    }
}


#endif //SERVER_GPS_HPP
