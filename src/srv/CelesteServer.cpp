#include <iostream>
#include <memory>
#include <termios.h>
#include <unistd.h>
#include <soci/mysql/soci-mysql.h>

#include "error.hpp"
#include "logging/Logger.hpp"

#include "crypt/CelesteEncrypter.hpp"
#include "CelesteServer.hpp"


using namespace std;
using namespace soci;

namespace celeste
{
    using DeviceResource    = resource::Devices<nlohmann::json>;
    using ModelResource     = resource::Models<nlohmann::json>;
    using PointResource     = resource::Points<nlohmann::json>;
    using DeviceModelAssocResource = resource::DeviceModelAssocs<nlohmann::json>;
    using LoggerResource    = resource::LoggerUpload;
    using ReadingResource   = resource::ReadingDispatcher<nlohmann::json>;
    using DeviceStatusResource = resource::DeviceStatusService<nlohmann::json>;
    using APIUserResource = resource::APIUsers<nlohmann::json>;

    void
    CelesteSettings::set_db_settings(const string& settings)
    {
        m_db_settings = settings;
    }

    string
    CelesteSettings::get_db_settings() const
    {
        return m_db_settings;
    }

    CelesteServer::CelesteServer()
        : restbed::Service()
    {
        set_logger(make_shared<CelesteLogger>());
        set_authentication_handler(
                [this] (const shared_ptr<restbed::Session> s,
                        const function<void(const shared_ptr<restbed::Session>)>& c) {AUTH(s, c); });
    }

    void
    CelesteServer
    ::start(const shared_ptr<const CelesteSettings> settings)
    {
        // get parameters
        string db_settings = settings->get_db_settings();
        uint16_t worker_limit = settings->get_worker_limit();

        // Create god account if it doesnt exist
        create_god_if_not_exists(db_settings);

        // Create resources
        publish(make_shared<DeviceResource>(db_settings));
        publish(make_shared<ModelResource>(db_settings));
        publish(make_shared<PointResource>(db_settings));
        publish(make_shared<LoggerResource>(db_settings, worker_limit));
        publish(make_shared<ReadingResource>(db_settings, worker_limit));
        publish(make_shared<DeviceModelAssocResource>(db_settings));
        publish(make_shared<DeviceStatusResource>(db_settings));
        publish(make_shared<APIUserResource>(db_settings));

        // fill the auth pool
        for (uint16_t i = 0; i < worker_limit; ++i)
            m_auth_pool.emplace(db_settings);

        restbed::Service::start(shared_ptr<const restbed::Settings>(settings));
    }

    void 
    CelesteServer
    ::create_god_if_not_exists(const std::string& dbSettings)
    {
        session sql(mysql, dbSettings);

        int count;
        sql << "select count(*) from APIUser where ugroup = \'God\'",
            into(count);

        if (count > 0)
            return;

        std::string id;
        std::string pwd;

        cout 
            << "\e[33mA God account does not exist!\n"
            << "You must create a God account to insert the first users\n"
            << "of the API into the Database.\n";

        cout << "Type in your id o'almighty god: \e[39m";
        getline(cin, id);

        cout << "\e[33mNow type in your password (at least 6 characters and less than 127): \n";
        int retries = 3;
        while (true)
        {
            termios oldt;
            tcgetattr(STDIN_FILENO, &oldt);
            termios newt = oldt;
            newt.c_lflag &= ~ECHO;
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);

            getline(cin, pwd);

            tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 

            if (pwd.size() < 6 || pwd.size() > 127)
            {
                switch (retries)
                {
                    case 3:
                    {
                        cout << "Remember, at least 6 characters and less than 32: ";
                        break;
                    }
                    case 2:
                    {
                        cout << "Are you daft? At least 6 characters and less than 32: ";
                        break;
                    }
                    case 1:
                    {
                        cout << "... :";
                        break;
                    }
                    case 0:
                    {
                        cout << "I'm outta here\n\e[39m";
                        throw runtime_error("You are an asshole.");
                        break;
                    }
                }
                cout << "\n(Remaining retries: " << retries << ")\n";
                retries--;
            }
            else
                break;
        }
        
        // Generate salt
        cout << "Generating salt...\n";
        string salt;
        srand(time(nullptr));
        time_t seconds_past_epoch = time(0);
        stringstream ss;
        ss << seconds_past_epoch << rand();

        CelesteEncrypter crypt;
        crypt.encrypt(ss.str(), salt);

        cout << "Encrypting password...\n";
        std::string encrypted_pwd;
        crypt.encrypt(salt + pwd, encrypted_pwd);

        cout << "Registering " << id << " as a God...\n"; 
        transaction tr(sql);
        sql << "insert into APIUser(id, ugroup, salt, pwd) values(:UserId, \'God\', :salt, :pwd)",
            use(id),
            use(salt),
            use(encrypted_pwd);
        tr.commit();
        cout << "\e[32mSuccess!\n";
        cout << "\e[39m";
    }

    void
    CelesteServer
    ::AUTH(const shared_ptr<restbed::Session> session,
           const function<void(const shared_ptr<restbed::Session>)>& callback)
    {
        auto request = session->get_request();
        BasicAuthRequest auth;
        auth.authorisation  = request->get_header("Authorization", "");
        auth.method         = request->get_method();
        auth.resource       = request->get_path();

        if (auth.authorisation.empty())
            throw AuthError("No authorisation credentials specified.");

        auto authenticator = m_auth_pool.acquire_wait();
        AuthStatus status;
        authenticator->auth(auth, status); // performs authorization and authenticaton

        if (status == SUCCESS)
            callback(session);
        else
            session->close(restbed::FORBIDDEN);
    }
}