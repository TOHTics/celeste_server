//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/13/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file
 * @brief No description available.
 */
//</editor-fold>

#include <cassert>
#include "util.hpp"

namespace solarplant
{
namespace db
{
namespace util
{
typedef std::map<std::string, std::string> value_map_type;

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
    if (*column_vector.begin() == "*")
        result += "SELECT * FROM ";
    else
        result += "SELECT (" + as_comma_list(as_quote_vector(column_vector)) + ") ";

    result += "FROM " + table_name + ";";
    return result;

}

}
}
}