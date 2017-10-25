/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains function definitions.
 */
#include <cassert>
#include "util.hpp"

namespace solarplant
{
namespace db
{
namespace util
{
typedef std::map<std::string, std::string> value_map_type;

timestamp_type get_universal_time()
{   using boost::posix_time::second_clock;
    return timestamp_type(second_clock::universal_time());
}

std::string to_string(timestamp_type t)
{
    std::stringstream stream;
    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet();
    facet->format("%Y-%m-%d %H:%M:%S");
    stream.imbue(std::locale(std::locale::classic(), facet));
    stream << t;
    return stream.str();
}

std::string to_string(date_type date) 
{
    return date_to_string(date);
}

date_type string_to_date(std::string str)
{
    return boost::gregorian::from_string(str);
}

std::string date_to_string(date_type date)
{
    return boost::gregorian::to_iso_string(date);
}

}
}
}