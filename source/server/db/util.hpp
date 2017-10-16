/**
 * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 9/8/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains function declarations and template function definitions.
 */
#ifndef SERVER_DB_UTIL_HPP
#define SERVER_DB_UTIL_HPP

#include <iostream>

#include <string>
#include <vector>
#include <iterator>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace solarplant
{
namespace db
{
namespace util
{

typedef boost::gregorian::date date_type; ///< `date_type` is typedefed with `boost::gregorian::date`
typedef boost::posix_time::ptime timestamp_type;

inline std::string to_string(const std::string &str)
{
    return str;
}

std::string to_string(timestamp_type t);
std::string to_string(date_type date);
timestamp_type get_universal_time();

/**
 * @brief      Returns an object of `date_type` with the given
 * date in the string. It expects the format of the date to be in
 * ISO format: `YYYYMMDD`
 *
 * @param[in]  str   The date formatted as `YYYYMMDD`.
 *
 * @return     { description_of_the_return_value }
 */
date_type string_to_date(std::string str);

/**
 * @brief      Returns a string in ISO format: `YYYYMMDD`.
 *
 * @param[in]  date  A `date_type` object.
 *
 * @return     A string in ISO date format: `YYYMMDD`.
 */
std::string date_to_string(date_type date);

/**
 * @brief      Returns the string but with quotes around it.
 *
 * @param[in]  str   The string.
 * @param[in]  q     The quote character.
 *
 * @return     A string but with quotes around it.
 */
std::string quote(const std::string& str, char q = '\'');

/**
 * @brief      Builds a SQL insert statement string.
 *
 * @param[in]  value_vector   Vector containing the values to insert. The values must be in
 * the order of their respective columns. I.e. If `column_vector` has `"age"` as its first entry,
 * then `value_vector` should contain the value for `"age"` in its first entry. 
 * @param[in]  column_vector  Vector of columns.
 * @param[in]  table_name     Name of table to insert to.
 *
 * @return     A string containing the insert statement.
 * 
 * In general it will return a string that looks like:
 * ````
 * 
 * ````
 */
std::string build_insert_statement( const std::vector<std::string> & value_vector,
                                    const std::vector<std::string> & column_vector,
                                    const std::string & table_name );

/**
 * @brief      Builds a SQL select statement string.
 *
 * @param[in]  column_vector  Vector containing the columns to be retrieved from the db.
 * @param[in]  table_name     Name of table to select from.
 *
 * @return     A string containing the select statement.
 * 
 * In general it will return a string that looks like:
 * ````sql
 * SELECT column1, column2, ... FROM table_name;
 * ````
 */
std::string build_select_statement( const std::vector<std::string> & column_vector,
                                    const std::string & table_name);

/**
 * @brief      Builds a SQL select statement for selecting all the columns from the table.
 * 
 * @param[in]  table_name  The table name
 *
 * @return     A string containing the select statement
 * 
 * In general it will return a string that looks like:
 * ````sql
 * SELECT * FROM table_name; 
 * ````
 */
std::string build_select_all_statement (const std::string & table_name);

/**
 * @brief      Creates a comma list from elements in a container. The container must contain strings.
 *
 * @param[in]  begin  Iterator of template type `It` pointing to the beginning of the container.
 * @param[in]  end    Iterator of template type `It` pointing to the end of the container.
 *
 * @tparam     It     Type of the iterator
 *
 * @return     A `std::string` comma list of all the elements between `begin` and `end`.
 */
template < typename It > 
std::string as_comma_list(const It &begin, const It &end)
{
    /* We have to assert that the container contains strings */
    static_assert(std::is_same<std::string,
                    typename std::iterator_traits<It>::value_type>::value,
                "Iterator value type must be std::string");

    /* Build comma list */
    std::string result;
    for (auto it = begin; it != end; )
    {
        result += *it;
        if (++it != end)
            result += ",";
    }
    return result;
}

/**
 * @brief      Quotes all the elements of a container, *which contains* `std::string`.
 *
 * @param[in]  container  Instance of Container
 *
 * @tparam     Container  Container type which should contain `std::string`.
 *
 * @return     A string of comma separated list of the elements from `container.`
 */
template < typename Container >
std::string as_comma_list(const Container& container)
{
    return as_comma_list(std::begin(container), std::end(container));
}

/**
 * @brief      Returns a string of each individual element of the container
 * concatenated but each element has a prefix or suffix. The container must
 * contain strings.
 *
 * @param[in]  begin   Iterator pointing to beginning of the container.
 * @param[in]  end     Iterator pointing to the end of the container.
 * @param[in]  prefix  Prefix to add to each element.
 * @param[in]  suffix  Suffix to add to each element.
 *
 * @tparam     It      Type of iterator
 *
 * @return     String of all elements concatenated 
 */
template < typename It > 
std::string with_prefix_suffix(const It &begin, const It &end, std::string prefix, std::string suffix)
{
    /* Assert container contains strings */
    static_assert(std::is_same<std::string,
                    typename std::iterator_traits<It>::value_type>::value,
                "Iterator value type must be std::string");
    std::string result;
    for (auto it = begin; it != end; )
    {
        if (++it != end)
        {   
            result += prefix;
            result += *it;
            result += suffix;
        }
    }
    return result;
}

/**
 * @brief      Returns a vector of strings such that each element is
 * the quoted element from the container the iterators belong to. The
 * container's `value_type` must be string.
 *
 * @param[in]  begin  Iterator pointing to beginning of container.
 * @param[in]  end    Iterator pointing to end of container.
 * @param[in]  q      Quote character.
 *
 * @tparam     It     Iterator type
 * 
 * # Example
 * Using this function should give us a vector with quoted elements.
 *     
 * ````
 * using namespace std;
 * 
 * set<string> S{"Hello", "World", "This", "Is", "The", "Example"};
 * vector<string> QuoteVec;
 * 
 * QuoteVec = as_quote_vector(S.begin(), S.end());
 * 
 * for (auto it = QuoteVec.begin(); it != QuoteVec.end(); ++it)
 *     cout << *it << endl;
 * 
 * user $:
 * Output:
 * 'Hello'
 * 'World'
 * 'This'
 * 'Is'
 * 'The'
 * 'Example'
 * ````
 * 
 */
template < typename It >
std::vector<std::string> as_quote_vector(const It& begin, const It& end, char q = '\'')
{
    /* Assert container contains strings */
    static_assert(std::is_same<std::string,
                    typename std::iterator_traits<It>::value_type>::value,
                "Iterator value type must be std::string");

    std::vector<std::string> result(std::distance(begin, end));
    int idx = 0;
    for (auto it = begin; it != end; ++it, ++idx)
        result[idx] = (quote(*it, q));

    return result;
}

/**
 * @brief      Returns a vector of quoted string elements.
 *
 * @param[in]  container  A container of strings. I.e. their `value_type` is a string.
 * @param[in]  q          The quote character.
 *
 * @tparam     Container  Template type of a container.
 *
 * @return     A vector of quoted elements from the container.
 */
template < typename Container >
std::vector<std::string> as_quote_vector(const Container& container, char q = '\'')
{
    return as_quote_vector(std::begin(container), std::end(container), q);
}

/**
 * @brief      Static for function template. Requires C++1z compiler.
 *
 * @param      f       Lambda function to execute at each iteration.
 *
 * @tparam     First   Starting integer value.
 * @tparam     Last    End value, not inclusive.
 * @tparam     Lambda  A Lambda template parameter.
 */
template <int First, int Last, typename Lambda>
inline void static_for(Lambda const& f)
{
    if constexpr (First < Last)
      {
         f(std::integral_constant<int, First>{});
         static_for<First + 1, Last>(f);
      }
}
}   
}
}

#endif