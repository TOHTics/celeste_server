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
 * @brief No description available.
 */
//</editor-fold>
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
 * @class DeviceRecord
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 20/Sep/17.
 *
 * @brief No description available.
 *
 * # Description
 * No full length description available.
 */
struct DeviceRecord
{
    typedef boost::posix_time::ptime timestamp_type;

    int device_id;
    int index;
    timestamp_type t;
    std::string cid;
    std::string lif;
    std::string lid;

};
}
}
}
#endif
