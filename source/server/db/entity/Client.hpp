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
#ifndef SERVER_DB_CLIENT_HPP
#define SERVER_DB_CLIENT_HPP

#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

namespace solarplant
{
namespace db
{
namespace entity
{
/**
* @class Client
* @author Carlos Brito (carlos.brito524@gmail.com)
* @date 9/8/17.
*
* @brief No description available.
*
* # Description
* No full length description available.
*/
struct Client
{
    typedef boost::gregorian::date date_type;
    int id;
    std::string first_name;
    std::string last_name;
    uint32_t age;
    date_type dob;
};
}
}
}
#endif //SERVER_CLIENT_HPP
