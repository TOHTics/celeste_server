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
         * No full length description available.
         */
        struct SunSpecDataResponse
        {
            int status;
            std::string code;
            std::string reason;
            std::string message;

            static std::string to_xml(const SunSpecDataResponse &response);
        };
    }
}

#endif //SERVER_SUNSPECDATARESPONSE_HPP
