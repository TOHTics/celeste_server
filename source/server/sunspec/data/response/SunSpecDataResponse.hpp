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
 * @brief Contains class declarations for `SunSpecDataResponse`
 */
//</editor-fold>
#ifndef SERVER_SUNSPECDATARESPONSE_HPP
#define SERVER_SUNSPECDATARESPONSE_HPP

#include <string>
#include "DeviceResult.hpp"


namespace sunspec
{
    namespace data
    {
        /**
         * @class SunSpecDataResponse
         * @author Carlos Brito (carlos.brito524@gmail.com)
         * @date 8/29/17.
         *
         * @brief Describes the contents of a `SunSpecData` response
         *
         * # Description
         * Describes the response sent by a Host following the SunSpec interface.
         */
        struct SunSpecDataResponse
        {

            typedef std::vector<DeviceResult> device_result_list;

            int         status = 0;     ///< Mandatory HTTP status
            std::string code;           ///< Status code as per SDX spec
            std::string reason;         ///< Information about where the error was detected.
            std::string message;        ///< User-friendly error information

            device_result_list device_results; ///< Results of device processing

            /**
             * Empty constructor
             */
            SunSpecDataResponse() = default;

            /**
             * Copy constructor.
             * @param other Other instance to copy
             */
            SunSpecDataResponse(const SunSpecDataResponse& other) = default;

            /**
             * Returns the XML representation of the `SunSpecDataResponse` instance.
             * @param response The instance to convert to XML.
             * @param ptOut An optional output parameter which is the element tree (`ptree`) representation of the XML
             * @return The XML representation of the instance as a `std::string`
             */
            static std::string to_xml(const SunSpecDataResponse &response,
                                      std::shared_ptr<boost::property_tree::ptree> ptOut = nullptr);
        };
    }
}

#endif //SERVER_SUNSPECDATARESPONSE_HPP
