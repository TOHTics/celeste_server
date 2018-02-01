#ifndef CELESTE_AUTHENTICATOR_HPP
#define CELESTE_AUTHENTICATOR_HPP

#include <soci.h>
#include <object_pool.hpp>

namespace celeste
{

    struct DeviceAccount
    {

    };

    enum AuthStatus : int
    {
        INVALID,
        FORBIDDEN = 403,
        SUCCESS = 200
    };

    class CelesteAuthenticator
    {
    public:
        using size_type         = std::size_t;
        using db_settings_type  = std::string;
        using sql_type          = soci::session;

        CelesteAuthenticator(const db_settings_type& dbSettings);
        
        void auth_user(const std::string& id, const std::string& pwd, AuthStatus& status);
        void auth_device(const std::string& id, const std::string& pwd, AuthStatus& status);

    private:
        sql_type            m_sql;
        db_settings_type    m_db_settings;
    };
}

#endif