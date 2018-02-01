#include <exception>
#include <soci/mysql/soci-mysql.h>

#include "CelesteAuthenticator.hpp"
#include "srv/crypt/CelesteEncrypter.hpp"

namespace celeste
{
    using namespace soci;
    using namespace std;

    CelesteAuthenticator
    ::CelesteAuthenticator(const db_settings_type& dbSettings)
        :   m_sql(mysql, dbSettings),
            m_db_settings(dbSettings)
    {}

    void
    CelesteAuthenticator
    ::auth_user(const std::string& id, const std::string& pwd, AuthStatus& status)
    {
        try
        {
            m_sql << "select 1"; // "ping" the server
        } catch (mysql_soci_error&)
        {
            m_sql.reconnect(); // attempt reconnect
        } catch (exception&)
        {
            m_sql.open(mysql, m_db_settings); // if everything went bad attempt to open connection again
        }

        std::string db_pwd;
        std::string salt;
        m_sql << "select salt, pwd from APIUsers where username = :id",
                into(salt),
                into(db_pwd),
                use(id);

        std::string encrypted;
        CelesteEncrypter{}.encrypt(salt + pwd, encrypted);

        if (encrypted == db_pwd)
            status = SUCCESS;
        else
            status = FORBIDDEN;
    }

    void
    CelesteAuthenticator
    ::auth_device(const std::string& id, const std::string& pwd, AuthStatus& status)
    {
        try
        {
            m_sql << "select 1"; // "ping" the server
        } catch (mysql_soci_error&)
        {
            m_sql.reconnect(); // attempt reconnect
        } catch (exception&)
        {
            m_sql.open(mysql, m_db_settings); // if everything went bad attempt to open connection again
        }

        std::string db_pwd;
        std::string salt;
        m_sql << "select salt, pwd from DevicePasswords where Device_id = :id",
                into(salt),
                into(db_pwd),
                use(id);

        std::string encrypted;
        CelesteEncrypter{}.encrypt(salt + pwd, encrypted);

        if (encrypted == db_pwd)
            status = SUCCESS;
        else
            status = FORBIDDEN;
    }
}

