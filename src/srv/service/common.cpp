#include "common.hpp"
#include <iostream>

using namespace std;
using json = nlohmann::json;

namespace celeste
{
namespace resource
{
    string quote(const string& str, string q)
    {
        return q + str + q;
    }

    string as_comma_list(string s)
    {
        return s;
    }

    string as_column_list(string str)
    {
        return quote(str, "`");
    }

    string as_value_list(string str)
    {
        return quote(str, "\'");
    }

    void handle_error(const int code,
                      std::string message,
                      const std::shared_ptr<restbed::Session> session)
    {
        if (session->is_open())
        {
            session->close(code,
                           message,
                           {
                               { "Content-Length", std::to_string(message.size()) },
                               { "Connection",     "close" }
                           });
        }
        else
        {
            // TODO
            // The session closed before we could send back the error
            // must explicitly log that on the server
        }
    }

    template<>
    nlohmann::json
    get_json<nlohmann::json, restbed::Request>(const restbed::Request& request)
    {
        // get bytes
        restbed::Bytes bytes = request.get_body();
        
        // convert to string
        string body;
        bytes2string(bytes, body);

        // convert to json
        nlohmann::json j = nlohmann::json::parse(body);

        return j;
    }
}
}

namespace nlohmann
{
    using namespace boost::posix_time;
    void adl_serializer<boost::posix_time::ptime>::to_json(json& j, const boost::posix_time::ptime& obj)
    {
        j = to_iso_extended_string(obj);
    }

    void adl_serializer<boost::posix_time::ptime>::from_json(const json& j, boost::posix_time::ptime& obj)
    {
        obj = time_from_string(j.get<std::string>());
    }
}

