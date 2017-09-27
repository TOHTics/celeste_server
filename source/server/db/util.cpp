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

std::string quote(const std::string& str, char q) {
    return q + str + q;
}

std::string build_insert_statement( const std::vector<std::string> & value_vector,
                                    const std::vector<std::string> & column_vector,
                                    const std::string & table_name )
{
    assert(! table_name.empty());
    assert(! value_vector.empty() && ! column_vector.empty());
    assert(value_vector.size() == column_vector.size());

    std::string result;
    result += "INSERT INTO " + table_name + " ";

    result += "(" + as_comma_list(as_quote_vector(column_vector, '`')) + ")";
    result += " VALUES (" + as_comma_list(as_quote_vector(value_vector)) + ");";

    return result;
}

std::string build_select_statement( const std::vector<std::string> & column_vector,
                                    const std::string & table_name)
{

    assert(! table_name.empty());
    assert(! column_vector.empty());

    std::string result;
    result += "SELECT (" + as_comma_list(as_quote_vector(column_vector)) + ") ";
    result += "FROM " + table_name + ";";
    return result;

}

std::string build_select_all_statement (const std::string & table_name)
{

    assert(! table_name.empty());

    std::string result;
    result += "SELECT * ";
    result += "FROM " + table_name + ";";
    return result;

}

}
}
}