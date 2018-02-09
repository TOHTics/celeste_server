#ifndef CELESTE_SERVER_OBJECT_HPP
#define CELESTE_SERVER_OBJECT_HPP

#include <restbed>
#include <json.hpp>
#include "service/services.hpp"

namespace celeste
{

    class CelesteSettings : public restbed::Settings
    {
    public:
        CelesteSettings() = default;

        void set_db_settings(const std::string& settings);
        std::string get_db_settings() const;
    private:
        std::string m_db_settings;
    };

    class CelesteServer : public restbed::Service
    {
    public:
        CelesteServer();
        void start(const std::shared_ptr<const CelesteSettings> settings);

    private:
        void create_god_if_not_exists(const std::string& dbSettings);

        void AUTH(const std::shared_ptr<restbed::Session> session,
                  const std::function<void(const std::shared_ptr<restbed::Session>)>& callback);

        carlosb::object_pool<CelesteAuth>       m_auth_pool;
    };
}

#endif