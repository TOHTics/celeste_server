//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/29/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file SunSpecDataResponse.hpp
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/29/17.
 * 
 * @brief No description available.
 *
 * TODO
 * ----
 * Nothing for the moment.
 */
//</editor-fold>
#ifndef SERVER_SUNSPECDATARESPONSE_HPP
#define SERVER_SUNSPECDATARESPONSE_HPP

#include <string>


namespace sunspec
{
    namespace data
    {
        /**
         * @class
         * @author Carlos Brito (carlos.brito524@gmail.com)
         * @date 8/29/17.
         *
         * @brief No description available.
         *
         * # Description
         * Describes the response sent by a Host following the SunSpec interface.
         */
        struct SunSpecDataResponse
        {
            int         status = 0;     ///< Mandatory HTTP status
            std::string code;           ///< Status code as per SDX spec
            std::string reason;         ///< Information about where the error was detected.
            std::string message;        ///< User-friendly error information

            /**
             * Empty constructor
             */
            SunSpecDataResponse() = default;

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
