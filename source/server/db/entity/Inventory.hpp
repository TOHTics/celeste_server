/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains class declarations.
 */
#ifndef SERVER_DB_CLIENT_HPP
#define SERVER_DB_CLIENT_HPP

#include <string>

namespace solarplant
{
namespace db
{
namespace entity
{
/**
 * @class   Inventory
 * @author  Carlos Brito (carlos.brito524@gmail.com)
 * @brief   No description available.
 *
 * No full length description available.
 */
struct Inventory
{
    int id;
    std::string description;
};
}
}
}
#endif //SERVER_CLIENT_HPP
