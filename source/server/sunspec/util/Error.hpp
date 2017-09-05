//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/18/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief Contains `Error` and its derived classes.
 */
//</editor-fold>
#ifndef SERVER_ERROR_HPP
#define SERVER_ERROR_HPP

#include <exception>
#include <string>

namespace sunspec
{
    namespace data
    {
        /**
         * @class Error
         * @author Carlos Brito (carlos.brito524@gmail.com)
         * @date 8/17/17.
         *
         * @brief General class for an Error.
         */
        class error : std::exception
        {
            std::string message;

        public:
            error() = default;
            error(const char* message) : message(message) {}
            error(std::string message) : message(message) {}

            virtual const char* what() const throw()
            {
                return message.c_str();
            }
        };

        /**
         * @class XMLError
         * @author Carlos Brito (carlos.brito524@gmail.com)
         * @date 8/17/17.
         *
         * @brief Represents an action that went wrong while
         * parsing XML.
         */
        class XMLError : public error
        {
        public:
            XMLError(std::string message) : error(message) {}
        };

        /**
         * @class PointDataError
         * @author Carlos Brito (carlos.brito524@gmail.com)
         * @date 8/22/17.
         *
         * @brief Represents an error with respect to some PointData.
         * Usually this will indicate that the PointData is not formed
         * correctly.
         */
        class PointDataError : public error
        {
        public:
            PointDataError(std::string message) : error(message) {}
        };

        /**
         * @class ModelDataError
         * @author Carlos Brito (carlos.brito524@gmail.com)
         * @date 8/23/17.
         *
         * @brief Represents an error with respect to some ModelDataError.
         * Usually this will indicate that the ModelDataError is not formed
         * correctly.
         */
        class ModelDataError : public error
        {
        public:
            ModelDataError(std::string message) : error(message) {}
        };
    }
}


#endif //SERVER_ERROR_HPP
