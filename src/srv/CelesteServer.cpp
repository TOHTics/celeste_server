#include <memory>

#include "error.hpp"
#include "logging/Logger.hpp"

#include "CelesteServer.hpp"


using namespace std;

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