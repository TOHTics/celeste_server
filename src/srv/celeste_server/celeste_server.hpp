#ifndef CELESTE_SERVER_OBJECT_HPP
#define CELESTE_SERVER_OBJECT_HPP

#include <restbed>
#include <json.hpp>
#include "srv/service/services.hpp"
#include "srv/service/status.hpp"

namespace celeste
{
    class celeste_server
    {
        using DeviceResource = resource::Devices<nlohmann::json>;
        using ModelResource = resource::Models<nlohmann::json>;
        using PointResource = resource::Points<nlohmann::json>;
        using DeviceModelAssocResource = resource::DeviceModelAssocs<nlohmann::json>;
        using LoggerResource = resource::LoggerUpload;
        using ReadingResource = resource::ReadingDispatcher<nlohmann::json>;
        using DeviceStatusResource = resource::DeviceStatusService<nlohmann::json>;

    public:
        celeste_server(int port, std::string dbSettings, int worker_limit, std::string root = "celeste");
        void start();
        void stop();
    private:
        restbed::Service    m_api;
        std::shared_ptr<restbed::Settings>   m_srv_settings;
        int                 m_port;
        std::string         m_root;

    };
}

#endif