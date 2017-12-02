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
}
}

