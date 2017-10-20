/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains class declarations.
 */
#ifndef SERVER_DB_POINTRECORD_HPP
#define SERVER_DB_POINTRECORD_HPP

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace solarplant
{
namespace db
{
namespace entity
{
/**
 * @class   PointRecord
 * @author  Carlos Brito (carlos.brito524@gmail.com)
 * @brief   No description available.
 *
 * No full length description available.
 */
struct PointRecord
{
    typedef boost::posix_time::ptime timestamp_type;
    typedef std::string              string_type;

    int             device_id;
    int             device_record_idx;
    int             model_idx;
    int             idx;
    double          sf;
    string_type     model_id;
    string_type     point_id;
    timestamp_type  t;
};
}
}
}
#endif //SERVER_POINTRECORD_HPP
