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
    namespace data
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
    }
}

#endif //SERVER_SDX_TAGS_HPP
