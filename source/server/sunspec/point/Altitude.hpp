//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/31/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief No description available.
 */
//</editor-fold>
#ifndef SERVER_ALTITUDE_HPP
#define SERVER_ALTITUDE_HPP

#include <sunspec/data/PointData.hpp>

namespace sunspec
{
    namespace point
    {

        /**
         * @class Altitude
         * @author Carlos Brito (carlos.brito524@gmail.com)
         * @date 8/31/17.
         *
         * @brief No description available.
         *
         * # Description
         * No full length description available.
         */
        struct Altitude : public data::PointData
        {
            Altitude() : Altitude(0)
            {}

            Altitude(double alt)
            {
                id = "alt";
                sf = "0";
                d = "Altitude of device";
                u = "meters";
                value = std::to_string(alt);
            }

            inline operator double()
            {
                return std::stod(this->value);
            }

        };

    }
}

#endif //SERVER_ALTITUDE_HPP
