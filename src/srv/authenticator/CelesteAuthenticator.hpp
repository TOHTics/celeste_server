#ifndef CELESTE_AUTHENTICATOR_HPP
#define CELESTE_AUTHENTICATOR_HPP

#include <soci.h>
#include <object_pool.hpp>

namespace celeste
{
    enum AuthStatus : int
    {
        INVALID,
        FORBIDDEN,
        SUCCESS
    };

    class CelesteAuthenticator
    {
    public:
        using size_type         = std::size_t;
        using db_settings_type  = std::string;
        using sql_type          = soci::session;
        using sql_pool_type     = carlosb::object_pool<sql_type>;

        CelesteAuthenticator(const db_settings_type& dbSettings, size_type connections);
        
        void operator()(const std::string& id, const std::string& pwd, AuthStatus& status);

    private:
        sql_pool_type       m_sql_pool;
        db_settings_type    m_db_settings;
    };
}

#endif