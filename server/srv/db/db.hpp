#ifndef SERVER_DB_DB_HPP
#define SERVER_DB_DB_HPP

#include <memory>
#include <restbed>
#include <json.hpp>
#include <mysql_devapi.h>

namespace nlohmann
{
    template <>
    struct adl_serializer<mysqlx::Value>
    {
        static void to_json(json& j, const mysqlx::Value& value);
        static void from_json(const json& j, mysqlx::Value& value);
    };

    template <>
    struct adl_serializer<mysqlx::RowResult>
    {
        static void to_json(json& j, mysqlx::RowResult res);
    };

    template <>
    struct adl_serializer<mysqlx::SqlResult>
    {
        static void to_json(json& j, mysqlx::SqlResult res);
    };
}

namespace celeste
{
    namespace db
    {
    /**
     * @brief      Makes a MySQL database session.
     *
     * @param[in]  args  Arguments.
     *
     * @tparam     Args  Argument pack.
     *
     * @return     MySQL database session.
     * 
     * In general, we may use this function like:
     * `auto dbSession = make_db_session("localhost", 33060, "user", "password")`
     */
    template<typename... Args>
    std::shared_ptr<mysqlx::Session> make_db_session(Args... args)
    {   
        return std::shared_ptr<mysqlx::Session>(new mysqlx::Session(args ...));
    }


    /**
     * @brief      Makes a MySQL database session.
     *
     * @param[in]  host  The host
     * @param[in]  port  The port
     * @param[in]  user  The user
     * @param[in]  pwd   The password
     *
     * @return     MySQL database session.
     */
    std::shared_ptr<mysqlx::Session> make_db_session(std::string host, int port, std::string user, std::string pwd);

    }
}
#endif