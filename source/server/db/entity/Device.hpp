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
 * @class Device
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 20/Sep/17.
 *
 * @brief No description available.
 *
 * # Description
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
