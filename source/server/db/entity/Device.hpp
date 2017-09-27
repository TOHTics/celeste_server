/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains class declarations.
 */
#ifndef SERVER_DB_DEVICE_HPP
#define SERVER_DB_DEVICE_HPP

#include <string>

namespace solarplant
{
namespace db
{
namespace entity
{
/**
 * @class   Device
 * @author  Carlos Brito (carlos.brito524@gmail.com)
 * @brief   No description available.
 *
 * No full length description available.
 */
struct Device
{
    int id;
    std::string client_id;
    std::string man;
    std::string mod;
    std::string sn;
};
}
}
}
#endif
