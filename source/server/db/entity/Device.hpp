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
    typedef std::string string_type;

    enum Column
    {
        id_col,
        client_id_col,
        man_col,
        mod_col,
        sn_col
    }; ///< Positions of the columns of the table Device

    int id;                 ///< Id of device
    string_type client_id;  ///< Id of the client which owns this device
    string_type man;        ///< Manufacturer of the device
    string_type mod;        ///< Model of device
    string_type sn;         ///< Serial number
};
}
}
}
#endif
