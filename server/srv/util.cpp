#include "util.hpp"

namespace celeste
{
namespace srv
{
namespace util
{
    std::string quote(const std::string& str, std::string q)
    {
        return q + str + q;
    }

    std::string as_comma_list(std::string s)
    {
        return s;
    }

    std::string as_column_list(std::string str)
    {
        return quote(str, "`");
    }

    std::string as_value_list(std::string str)
    {
        return quote(str, "\'");
    }

}
}
}