//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/29/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief Contains HTTP status codes
 */
//</editor-fold>
#ifndef SUNSPEC_STATUS_CODES_HPP
#define SUNSPEC_STATUS_CODES_HPP

namespace sunspec
{
namespace sdx
{
static const int SUCCESS = 200;
static const int FAILURE = 400;
static const int D_FAILURE = 400;

static const int ACCESS_DENIED = 403; ///< Access granted to system but denied for specific resource
static const int INVALID_MESSAGE = 400; ///< Message is either badly formed or has invalid content
static const int INVALID_CREDENTIALS = 401; ///< Access to system denied
static const int LIMIT_EXCEEDED = 400; ///< Limit for an amount within a single operation exceeded
static const int PROCESSING_EXCEPTION = 500; ///< Error occurred during processing with a known reason
static const int QUOTA_EXCEEDED = 400; ///< Limit for requests within a time range exceeded
static const int SYSTEM_MAINTENANCE = 503; ///< System is currently unavailable
static const int UNEXPECTED_EXCEPTION = 500; ///< Error occurred during processing for unknown reason
static const int UNKNOWN_LOGGER = 400; ///< Device id is not valid
static const int UNKNOWN_DEVICE = 400; ///< Logger id is not valid
}
}
#endif //SERVER_STATUS_CODES_HPP
