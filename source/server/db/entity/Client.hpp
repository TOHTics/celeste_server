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
 */
struct Client
{
    typedef std::string              string_type;
    typedef boost::gregorian::date   date_type;

    enum Column
    {
        id_col,
        first_name_col,
        last_name_col,
        dob_col
    }; ///< Positions of the columns of the table Client

    string_type      id;            ///< Id of the client
    string_type      first_name;    ///< First name of the client
    string_type      last_name;     ///< Last name of the client
    date_type        dob;           ///< Date of birth 
};
}
}
}
#endif //SERVER_CLIENT_HPP
