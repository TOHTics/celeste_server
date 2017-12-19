#ifndef CELESTE_SERVICE_COMMON_HPP
#define CELESTE_SERVICE_COMMON_HPP

#include <memory>
#include <string>
#include <mysql_devapi.h>
#include <json.hpp>
#include <restbed>
#include <utility>
#include <boost/optional.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "srv/db/db.hpp"

namespace celeste
{
namespace resource
{
    // --- USEFUL TYPES --------------
    using ValueList = std::vector<mysqlx::EnhancedValue>;
    using ValueMap = std::map<std::string, mysqlx::EnhancedValue>;

    enum PointType
    {
        STRING = 0,
        INT,
        DOUBLE,
        FLOAT
    };

    // --- METHODS -------------------
    
    /**
     * @brief      Converts bytes to a string.
     *
     * @param[in]  bytes      Bytes.
     * @param      out        String.
     *
     * @tparam     CharT      Type of each string element.
     * @tparam     Traits     Traits of string.
     * @tparam     Allocator  String allocator.
     */
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

    /**
     * @brief      Handles an error that occurred in the service.
     *
     * @param[in]  code     The code
     * @param[in]  message  The message
     * @param[in]  session  The session
     */
    void
    handle_error(const int code,
                 std::string message,
                 const std::shared_ptr<restbed::Session> session);

    /**
     * @brief      Gets the body of a request as a JSON object.
     *
     * @param[in]  request  Request.
     *
     * @tparam     Json     JSON type.
     * @tparam     Request  Request type.
     *
     * @return     Body of request as JSON object.
     */
    template <class Json, class Request>
    Json get_json(const Request& request);

    /**
     * @brief      Function template specialization. It specializes for
     * `Json = nlohmann::json` and `Request = restbed::Request`
     *
     * @param[in]  request  The request
     *
     * @return     { description_of_the_return_value }
     */
    template<>
    nlohmann::json
    get_json<nlohmann::json, restbed::Request>(const restbed::Request& request);
}
}


// --- JSON SERIALIZERS --------------
namespace nlohmann 
{
    template <typename T>
    struct adl_serializer<boost::optional<T>> 
    {
        static void to_json(json& j, const boost::optional<T>& opt) {
            if (opt == boost::none) {
                j = nullptr;
            } else {
              j = *opt; // this will call adl_serializer<T>::to_json which will
                        // find the free function to_json in T's namespace!
            }
        }

        static void from_json(const json& j, boost::optional<T>& opt) 
        {
            if (j.is_null()) {
                opt = boost::none;
            } else {
                opt = j.get<T>(); // same as above, but with 
                                  // adl_serializer<T>::from_json
            }
        }
    };


    template <>
    struct adl_serializer<boost::posix_time::ptime>
    {
        static void to_json(json& j, const boost::posix_time::ptime& time);
        static void from_json(const json& j, boost::posix_time::ptime& time);
    };
}

#endif