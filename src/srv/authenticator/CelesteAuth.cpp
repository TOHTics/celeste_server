#include <exception>
#include <soci/mysql/soci-mysql.h>

#include <fstream>

#include "CelesteAuth.hpp"

#include "srv/crypt/CelesteEncrypter.hpp"
#include "srv/service/base64.h"

namespace celeste
{
    using namespace soci;
    using namespace std;
    using namespace nlohmann;

    CelesteAuth
    ::CelesteAuth(const db_settings_type& dbSettings, const std::string& filename)
        :   m_sql(mysql, dbSettings),
            m_db_settings(dbSettings)
    {
        ifstream i(filename);
        if (!i.is_open())
            throw runtime_error("Could not open permissions file: " + string(filename));

        try
        {
            i >> m_permissions;
        } catch (exception& e)
        {
            throw runtime_error("Malformed permissions file: " + string(e.what()));
        }
    }

    void
    CelesteAuth
    ::auth(const BasicAuthRequest& auth, AuthStatus& status)
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

        // Extract id and password
        std::string decoded = base64_decode(auth.authorisation.substr(auth.authorisation.find(' ') + 1));        
        auto pos = decoded.find(':');
        std::string id = decoded.substr(0, pos);
        std::string pwd = decoded.substr(pos + 1);

        if (id.empty() || pwd.empty())
            throw runtime_error("Empty authentication field!");

        // Authenticate
        std::string db_pwd;
        std::string salt;
        std::string group;
        m_sql << "select salt, pwd, ugroup from APIUsers where id = :userid",
                into(salt),
                into(db_pwd),
                into(group),
                use(id, "userid");

        std::string encrypted;
        CelesteEncrypter{}.encrypt(salt + pwd, encrypted);

        if (encrypted == db_pwd)
        {
            if (authorize(group, auth.resource, auth.method))
                status = SUCCESS;
            else
                status = FORBIDDEN;
        }
        else
        {
            status = FORBIDDEN;
        }
    }

    bool
    CelesteAuth
    ::authorize(const std::string& group,
                const std::string& resource,
                const std::string& method)
    {
        cout << resource << endl;
        cout << method << endl;
        auto allowed_resources = m_permissions.find(group); 
        cout << *allowed_resources << endl;
        if (allowed_resources == m_permissions.end())
        {
            return false;
        }
        else if (allowed_resources->is_string())
        {
            return (*allowed_resources == "all");
        }
        else if (allowed_resources->is_object())
        {
            auto allowed_methods = allowed_resources->find(resource);
            cout << *allowed_methods << endl;
            if (allowed_methods == allowed_resources->end())
            {
                return false;
            }
            else if (allowed_methods->is_string())
            {
                return (*allowed_methods == "all");
            }
            else if (allowed_methods->is_array())
            {
                vector<string> tmp = *allowed_methods;
                return (find(tmp.begin(), tmp.end(), method) != tmp.end());
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
}

