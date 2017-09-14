//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/13/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief No description available.
 */
//</editor-fold>
#ifndef SERVER_ERROR_HPP
#define SERVER_ERROR_HPP

#include <string>
#include <exception>

namespace solarplant
{
namespace db
{
namespace dao
{
/**
 * @class DAOException
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/17/17.
 *
 * @brief General class for a DAOException.
 */
class DAOException : std::exception
{
    std::string message;

public:
    DAOException() = default;

    DAOException( const char *message ) : message(message)
    { }

    DAOException( std::string message ) : message(message)
    { }

    virtual const char *what() const throw()
    {
        return message.c_str();
    }
};
}
}
}
#endif //SERVER_ERROR_HPP
