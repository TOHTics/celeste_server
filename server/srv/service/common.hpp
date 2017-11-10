#ifndef SERVER_RESOURCE_QUERY_COMMON_HPP
#define SERVER_RESOURCE_QUERY_COMMON_HPP

#include <memory>
#include <string>
#include <mysql_devapi.h>
#include <json.hpp>

namespace celeste
{
    using dbsession_t = std::shared_ptr<mysqlx::Session>;
    using ValueList = std::vector<mysqlx::Value>;
    using ValueMap = std::map<std::string, mysqlx::Value>;

    enum PointType
    {
        STRING = 0,
        INT,
        DOUBLE,
        FLOAT
    };
}

#endif