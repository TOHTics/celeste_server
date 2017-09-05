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
 * @brief Contains class declarations for OpenEnergyMonitor.
 */
//</editor-fold>
#ifndef SERVER_OPENENERGYMONITOR_HPP
#define SERVER_OPENENERGYMONITOR_HPP

#include "CommonModel.hpp"

namespace sunspec
{
    namespace model
    {
        /**
         * @class OpenEnergyMonitor
         * @author Carlos Brito (carlos.brito524@gmail.com)
         * @date 8/30/17.
         *
         * @brief No description available.
         *
         * # Description
         * No full length description available.
         */
        struct OpenEnergyMonitor : public CommonModel
        {
            double voltage;
            double amperage;

            ~OpenEnergyMonitor() {};
        };
    }
}


#endif //SERVER_OPENENERGYMONITOR_HPP
