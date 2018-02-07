#ifndef CELESTE_AUTHENTICATOR_HPP
#define CELESTE_AUTHENTICATOR_HPP

#include <soci.h>
#include <object_pool.hpp>
#include <json.hpp>

namespace celeste
{
    enum AuthStatus : int
    {
        INVALID,
        FORBIDDEN = 403,
        SUCCESS = 200
    };

    struct BasicAuthRequest
    {
        std::string authorisation;
        std::string resource;
        std::string method;
    };

    class CelesteAuth
    {
    public:
        using size_type         = std::size_t;
        using db_settings_type  = std::string;
        using sql_type          = soci::session;

        CelesteAuth(const db_settings_type& dbSettings, const std::string& filename = "permissions.json");
        
        void auth(const BasicAuthRequest& auth, AuthStatus& status);

    private:
        bool authorize(const std::string& group,
                       const std::string& resource,
                       const std::string& method);

        bool authorize_method(const nlohmann::json& resources,
                              const std::string& method);

        sql_type            m_sql;
        db_settings_type    m_db_settings;
        nlohmann::json      m_permissions;


    };
}

#endif