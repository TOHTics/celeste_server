/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains class declarations.
 */
#ifndef SERVER_DB_DEVICERECORD_HPP
#define SERVER_DB_DEVICERECORD_HPP

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace solarplant 
{
namespace db
{
namespace entity
{
/**
 * @class   DeviceRecord
 * @author  Carlos Brito (carlos.brito524@gmail.com)
 * @brief   No description available.
 *
 * No full length description available.
 */
struct DeviceRecord
{
    typedef boost::posix_time::ptime timestamp_type;
    typedef std::string              string_type;

    enum Column
    {
        idx_col,
        device_id_col,
        t_col,
        cid_col,
        if_col,
        lid_col
    };

    int device_id;
    int idx;
    timestamp_type t;
    string_type cid;
    string_type lif;
    string_type lid;

};
}
}
}
#endif
