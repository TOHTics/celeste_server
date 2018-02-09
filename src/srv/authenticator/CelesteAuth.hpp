#ifndef CELESTE_AUTHENTICATOR_HPP
#define CELESTE_AUTHENTICATOR_HPP

#include <soci.h>
#include <json.hpp>
#include <object_pool.hpp>
#include <unordered_map>

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
        using permission_map_type = std::unordered_map<std::string, nlohmann::json>;

        CelesteAuth(const db_settings_type& dbSettings, const std::string& filename = "config.json");
        
        void auth(const BasicAuthRequest& auth, AuthStatus& status) const;

    private:
        bool authorize(const std::string& group,
                       const std::string& resource,
                       const std::string& method) const;

        bool authorize_method(const nlohmann::json& resources,
                              const std::string& method) const;

        mutable sql_type        m_sql;
        db_settings_type        m_db_settings;
        nlohmann::json          m_permissions;
    };
}

#endif