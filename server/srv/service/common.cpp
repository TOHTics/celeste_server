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

    void handle_error(int status_code,
                      string message,
                      const shared_ptr<restbed::Session> session)
    {
        json error = message;
        session->close(status_code,
                       error.dump(),
                       {
                           { "Content-Length", to_string(error.dump().size()) },
                           { "Connection",     "close" }
                       });
    }
}
}

