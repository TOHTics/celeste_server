//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/18/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file sdx_tags.hpp
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/18/17.
 * 
 * @brief Contains the definitions for SunSpec Data Exchange (SDX) tags.
 *
 */
//</editor-fold>
#ifndef SERVER_SDX_TAGS_HPP
#define SERVER_SDX_TAGS_HPP

#include <string>

namespace sunspec
{
    namespace sdx
    {
        static const std::string SDX_POINT            = "p";      /// Point record
        static const std::string SDX_POINT_ID         = "id";     /// ID of point
        static const std::string SDX_POINT_INDEX      = "index";  /// Index of sample
        static const std::string SDX_POINT_SF         = "sf";     /// Scale factor of point record
        static const std::string SDX_POINT_UNITS      = "u";      /// Measurement units
        static const std::string SDX_POINT_DESC       = "d";      /// Description of point record
        static const std::string SDX_POINT_TIME       = "t";      /// Timestamp of point

        static const std::string SDX_MODEL            = "m";      /// Model record
        static const std::string SDX_MODEL_ID         = "id";     /// Device Model Identifier
        static const std::string SDX_MODEL_NAMESPACE  = "ns";     /// Model id namespace
        static const std::string SDX_MODEL_INDEX      = "x";      /// Model index record

        static const std::string SDX_DEVICE = "d";
        static const std::string SDX_DEVICE_LOGGER_ID = "lid";
        static const std::string SDX_DEVICE_NAMESPACE = "ns";
        static const std::string SDX_DEVICE_ID = "id";
        static const std::string SDX_DEVICE_IFC = "if";
        static const std::string SDX_DEVICE_MAN = "man";
        static const std::string SDX_DEVICE_MOD = "mod";
        static const std::string SDX_DEVICE_SN = "sn";
        static const std::string SDX_DEVICE_TIME = "t";
        static const std::string SDX_DEVICE_CORRELATION_ID = "cid";

        static const std::string SDX_VERSION = "1";
        static const std::string SDX_SUNSPEC_DATA = "SunSpecData";
        static const std::string SDX_SUNSPEC_DATA_VERSION = "v";
    }
}

#endif //SERVER_SDX_TAGS_HPP
