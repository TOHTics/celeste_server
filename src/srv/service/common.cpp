#include "common.hpp"

using namespace std;
using json = nlohmann::json;


namespace celeste
{
namespace resource
{
    void string2point(int type, const std::string& str, Reading::point_type& out)
    {
        switch (type)
        {
            case point_type_index::INTEGER:
            {
                out = stoi(str);
                return;
            }
            case point_type_index::FLOAT:
            {
                out = stof(str);
                return;
            }
            case point_type_index::STRING:
            {
                out = str;
                return;
            }
            default:
            {
                throw std::runtime_error("No type conversion");
            }
        }
    }

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
}
}

namespace soci
{
    void type_conversion<boost::posix_time::ptime>::from_base(const base_type& p, indicator, boost::posix_time::ptime& v)
    {
        v = boost::posix_time::ptime_from_tm(p);
    }

    void type_conversion<boost::posix_time::ptime>::to_base(const boost::posix_time::ptime& v, base_type& p, indicator&)
    {
        p = boost::posix_time::to_tm(v);
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

