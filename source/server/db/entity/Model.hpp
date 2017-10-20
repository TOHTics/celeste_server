/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains class declarations.
 */
#ifndef SERVER_DB_MODEL_HPP
#define SERVER_DB_MODEL_HPP

#include <string>

namespace solarplant
{
namespace db
{
namespace entity
{
/**
 * @class   Model
 * @author  Carlos Brito (carlos.brito524@gmail.com)
 * @brief   No description available.
 *
 * No full length description available.
 */
struct Model
{   
    typedef std::string string_type;

    enum Column
    {
        id_col,
        ns_col
    };

    string_type id;
    string_type ns;
};
}
}
}
#endif
