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
#ifndef SERVER_DB_UTIL_HPP
#define SERVER_DB_UTIL_HPP

#include <iostream>

#include <string>
#include <unordered_set>
#include <vector>
#include <map>
#include <iterator>

namespace solarplant
{
namespace db
{
namespace util
{

typedef std::map<std::string, std::string> column_map;

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
 */
std::string build_select_statement( const std::vector<std::string> & column_vector,
                                    const std::string & table_name);

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
 * vector<string> QuoteList;
 * 
 * QuoteList = quote(S.begin(), S.end());
 * 
 * for (auto it = QuoteList.begin(); it != QuoteList.end(); ++it)
 *     cout << *it << endl;
 * 
 * - $:
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

template < typename Container >
std::vector<std::string> as_quote_vector(const Container& container, char q = '\'')
{
    return as_quote_vector(std::begin(container), std::end(container), q);
}

}   
}
}

#endif