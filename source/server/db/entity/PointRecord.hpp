//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief Contains ModelRecord definitions.
 */
//</editor-fold>
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
 * @class PointRecord
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 20/Sep/17.
 *
 * @brief No description available.
 *
 * # Description
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
