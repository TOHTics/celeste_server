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
    int device_id;
    std::string model_id;
    std::string point_id;

    int device_record_idx;
    int model_index;
    int index;
    double sf;

};
}
}
}
#endif //SERVER_POINTRECORD_HPP
