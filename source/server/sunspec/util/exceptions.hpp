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
#ifndef SUNSPEC_EXCEPTIONS_HPP
#define SUNSPEC_EXCEPTIONS_HPP

#include <exception>
#include <string>

namespace sunspec
{
namespace data
{
/**
 * @class DataException
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/17/17.
 *
 * @brief General class for a DataException.
 */
class DataException : std::exception
{
    std::string message;

public:
    DataException() = default;

    DataException( const char *message ) : message(message)
    { }

    DataException( std::string message ) : message(message)
    { }

    virtual const char *what() const throw()
    {
        return message.c_str();
    }
};

/**
 * @class XMLException
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/17/17.
 *
 * @brief Represents an action that went wrong while
 * parsing XML.
 */
class XMLException : public DataException
{
public:
    XMLException( std::string message ) : DataException(message)
    { }
};

/**
 * @class PointDataException
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/22/17.
 *
 * @brief Represents an error with respect to some PointData.
 * Usually this will indicate that the PointData is not formed
 * correctly.
 */
class PointDataException : public DataException
{
public:
    PointDataException( std::string message ) : DataException(message)
    { }
};

/**
 * @class ModelDataException
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/23/17.
 *
 * @brief Represents an error with respect to some ModelDataError.
 * Usually this will indicate that the ModelDataError is not formed
 * correctly.
 */
class ModelDataException : public DataException
{
public:
    ModelDataException( std::string message ) : DataException(message)
    { }
};
}
}


#endif //SERVER_ERROR_HPP
