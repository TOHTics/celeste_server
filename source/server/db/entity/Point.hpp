/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains class declarations.
 */
#ifndef SERVER_DB_POINT_HPP
#define SERVER_DB_POINT_HPP

#include <string>

namespace solarplant
{
namespace db
{
namespace entity
{
/**
 * @class   Point
 * @author  Carlos Brito (carlos.brito524@gmail.com)
 * @brief This describes a Point.
 * 
 * No full lenght description available. 
 */
struct Point
{   
    typedef std::string string_type;
    string_type id;
    string_type model_id;
    string_type type;
    string_type u;
    string_type d;
};
}
}
}
#endif //SERVER_POINT_HPP
