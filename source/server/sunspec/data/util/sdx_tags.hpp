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
        using std::string;

        string SDX_POINT            = "p";      /// Point record
        string SDX_POINT_ID         = "id";     /// ID of point
        string SDX_POINT_INDEX      = "index";  /// Index of sample
        string SDX_POINT_SF         = "sf";     /// Scale factor of point record
        string SDX_POINT_UNITS      = "u";      /// Measurement units
        string SDX_POINT_DESC       = "d";      /// Description of point record
        string SDX_POINT_TIME       = "t";      /// Timestamp of point

        string SDX_MODEL            = "m";      /// Model record
        string SDX_MODEL_ID         = "id";     /// Device Model Identifier
        string SDX_MODEL_NAMESPACE  = "ns";     /// Model id namespace
        string SDX_MODEL_INDEX      = "x";      /// Model index record
    }
}

#endif //SERVER_SDX_TAGS_HPP
