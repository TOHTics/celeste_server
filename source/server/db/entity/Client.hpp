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
#include <boost/date_time/gregorian/gregorian.hpp>

namespace solarplant
{
namespace db
{
namespace entity
{
/**
 * @class   Client
 * @author  Carlos Brito (carlos.brito524@gmail.com)
 * @brief   No description available.
 * 
 * A client is usually a physical person which is composed
 * of many attributes such as:
 * - First and last name
 * - Date of Birth
 * - Home address
 * For the purposes of persisting data to the database and
 * logging Points we only require the id of the client,
 * preferably an `int`.
 */
struct Client
{
    typedef boost::gregorian::date date_type;

    std::string id;
    std::string first_name;
    std::string last_name;
    date_type dob;
};
}
}
}
#endif //SERVER_CLIENT_HPP
