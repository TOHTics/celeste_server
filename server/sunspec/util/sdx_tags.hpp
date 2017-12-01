//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/18/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @file
 * @brief Contains the definitions for SunSpec Data Exchange (SDX) tags.
 */
#ifndef SUNSPEC_SDX_TAGS_HPP
#define SUNSPEC_SDX_TAGS_HPP

#include <string>

namespace sunspec
{
namespace sdx
{
static const std::string XML_ATTR = "<xmlattr>";  ///< Meta tag to obtain attributes

static const std::string SDX_POINT = "p";      ///< Point record
static const std::string SDX_POINT_ID = "id";     ///< ID of point
static const std::string SDX_POINT_INDEX = "x";      ///< Index of sample
static const std::string SDX_POINT_SF = "sf";     ///< Scale factor of point record
static const std::string SDX_POINT_UNITS = "u";      ///< Measurement units
static const std::string SDX_POINT_DESC = "d";      ///< Description of point record
static const std::string SDX_POINT_TIME = "t";      ///< Timestamp of point

static const std::string SDX_MODEL = "m";      ///< Model record
static const std::string SDX_MODEL_ID = "id";     ///< Device Model Identifier
static const std::string SDX_MODEL_NAMESPACE = "ns";     ///< Model id namespace
static const std::string SDX_MODEL_INDEX = "x";      ///< Model index record

static const std::string SDX_DEVICE = "d";      ///< Device record
static const std::string SDX_DEVICE_LID = "lid";    ///< Logger id
static const std::string SDX_DEVICE_NS = "ns";     ///< Namespace
static const std::string SDX_DEVICE_ID = "id";     ///< Device id
static const std::string SDX_DEVICE_IFC = "if";     ///< Interface id
static const std::string SDX_DEVICE_MAN = "man";    ///< Manufacturer
static const std::string SDX_DEVICE_MOD = "mod";    ///< Model
static const std::string SDX_DEVICE_SN = "sn";     ///< Serial number
static const std::string SDX_DEVICE_TIME = "t";      ///< Timestamp of devic
static const std::string SDX_DEVICE_CID = "cid";    ///< Result correlation id

static const std::string SDX_VERSION = "1";            ///< Version of SDX spec
static const std::string SDX_SUNSPEC_DATA = "SunSpecData";  ///< SunSpecData record
static const std::string SDX_SUNSPEC_DATA_VERSION = "v";            ///< Version of SunSpecData

static const std::string SDX_SUNSPEC_DATA_RESPONSE = "SunSpecDataResponse";
static const std::string SDX_RESPONSE_STATUS = "status";
static const std::string SDX_RESPONSE_CODE = "code";
static const std::string SDX_RESPONSE_REASON = "reason";
static const std::string SDX_RESPONSE_MESSAGE = "message";

static const std::string SDX_DRESULT = "dResult";    ///< Device failure
static const std::string SDX_DRESULT_CODE = "code";       ///< Status code of error
static const std::string SDX_DRESULT_REASON = "reason";     ///< Reason why the device failed processing
static const std::string SDX_DRESULT_MESSAGE = "message";    ///< User friendly message describing the error
}
}

#endif //SERVER_SDX_TAGS_HPP
