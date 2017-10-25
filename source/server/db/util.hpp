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
#include "sunspec/data/data.hpp"

namespace solarplant
{
namespace db
{
namespace util
{

typedef boost::gregorian::date date_type;           ///< `date_type` is typedefed with `boost::gregorian::date`
typedef boost::posix_time::ptime timestamp_type;    ///< typedef of `boost::posix_time::ptime`

/**
 * @brief      Converts to string a timestamp
 *
 * @param[in]  t     Timestamp to convert to string
 *
 * @return     String representation of the object.
 */
std::string to_string(timestamp_type t);

/**
 * @brief      Returns a string representation of the object.
 *
 * @param[in]  date  The date
 *
 * @return     String representation of the object.
 */
std::string to_string(date_type date);

/**
 * @brief      Gets the universal time.
 *
 * @return     The universal time.
 */
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
std::string quote(const std::string& str, std::string q = "\'")
{
    return q + str + q;
}

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
template <typename It> 
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

std::string as_comma_list(std::string s)
{
    return s;
}

template <typename... Args>
std::string as_comma_list(std::string s, Args... args)
{   
    return s + "," + as_comma_list(args...);
}

/**
 * @brief      Base case for `n=1` of `as_column_list`
 *
 * @param[in]  str   Value.
 *
 * @return     Returns the quoted string.
 */
std::string as_column_list(std::string str)
{
    return quote(str, "`");
}

/**
 * @brief      Returns a comma separated, quoted string with the values being separated
 * being those passed in the container.
 *
 * @param[in]  str   Value.
 * @param[in]  args  The values to be separated.
 *
 * @tparam     Args  Pack of values.
 *
 * @return     String of concatenated values separated by commas and quoted by `'`.
 */
template <typename... Args>
std::string as_column_list(std::string str, Args... args)
{   
    return quote(str, "`") + ", " + as_column_list(args...);
}

/**
 * @brief      Returns a comma separated, quoted string with the values being separated
 * being those passed in the container.
 *
 * @param[in]  begin  Beginning of container.
 * @param[in]  end    End of container.
 *
 * @tparam     It     Iterator type.
 *
 * @return     String of concatenated values separated by commas and quoted by `'`.
 */
template <typename It>
std::string as_column_list(const It &begin, const It &end)
{
    static_assert(std::is_same<std::string,
                    typename std::iterator_traits<It>::value_type>::value,
                "Iterator value type must be a std::string type");
    std::string result;
    for (auto it = begin; it != end; )
    {
        result += quote(*it, "`");
        if (++it != end)
            result += ",";
    }
}

std::string as_value_list(std::string str)
{
    return quote(str, "\'");
}

template <typename... Args>
std::string as_value_list(std::string str, Args... args)
{   
    if (str[0] == '@' || str == "NULL")
        return str + "," + as_value_list(args...);
    else
        return quote(str, "\'") + "," + as_value_list(args...);
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
template <typename It>
std::vector<std::string> as_quote_vector(const It& begin, const It& end, std::string q = "\'")
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
template <typename Container>
std::vector<std::string> as_quote_vector(const Container& container, std::string q = "\'")
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

/**
 * @brief      Compares two pairs of same types. This method assumes
 * that `T` has the operator `==` defined/overloaded.
 *
 * @param[in]  a     First element to be compared.
 * @param[in]  b     Second element to be compared.
 *
 * @tparam     T     Type of both elements
 *
 * @return     `true` if they are the same. 
 */
template<typename T> 
bool pair_equal(T a, T b) {
  return a == b;
}

/**
 * @brief      { function_description }
 *
 * @param[in]  a     { parameter_description }
 * @param[in]  b     { parameter_description }
 * @param[in]  args  The arguments
 *
 * @tparam     T     { description }
 * @tparam     Args  { description }
 *
 * @return     { description_of_the_return_value }
 */
template<
    typename T,
    typename... Args
> bool pair_equal(T a, T b, Args... args) {
    return pair_equal(a, b) && pair_equal(args...);
}

}   
}
}

#endif