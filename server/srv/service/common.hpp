#ifndef CELESTE_SERVICE_COMMON_HPP
#define CELESTE_SERVICE_COMMON_HPP

#include <memory>
#include <string>
#include <mysql_devapi.h>
#include <json.hpp>
#include <restbed>
#include <utility>

namespace celeste
{
namespace resource
{
    using ValueList = std::vector<mysqlx::Value>;
    using ValueMap = std::map<std::string, mysqlx::Value>;

    enum PointType
    {
        STRING = 0,
        INT,
        DOUBLE,
        FLOAT
    };

    template <
        class CharT,
        class Traits = std::char_traits<CharT>, 
        class Allocator = std::allocator<CharT>
    > void bytes2string(const restbed::Bytes& bytes,
                        std::basic_string<CharT, Traits, Allocator>& out)
    {
        // Read n bytes of data into a char* 
        size_t size = bytes.size() + 1;

        char *char_arr = (char *) malloc(size);
        snprintf(char_arr, (int) size, "%s", bytes.data());

        // Convert char* to string
        std::basic_string<CharT, Traits, Allocator> str = char_arr;

        out = str;
    }

    /**
     * @brief      Returns the string but with quotes around it.
     *
     * @param[in]  str   The string.
     * @param[in]  q     The quote character.
     *
     * @return     A string but with quotes around it.
     */
    std::string quote(const std::string& str, std::string q = "\'");

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

    std::string as_comma_list(std::string s);

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
    std::string as_column_list(std::string str);
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

    std::string as_value_list(std::string str);

    template <typename... Args>
    std::string as_value_list(std::string str, Args... args)
    {   
        if (str[0] == '@' || str == "NULL")
            return str + "," + as_value_list(args...);
        else
            return quote(str, "\'") + "," + as_value_list(args...);
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

    void handle_error(const int code,
                      std::string message,
                      const std::shared_ptr<restbed::Session> session);
}
}

#endif