#include "CelesteServer.hpp"
#include "service/status.hpp"
#include "logging/Logger.hpp"

#include <iostream>
#include <memory>

using namespace std;

namespace celeste
{
    CelesteServer::CelesteServer(int port, string dbSettings,  int worker_limit, std::string root)
    {
        auto devices = make_shared<DeviceResource>(dbSettings);
        auto models = make_shared<ModelResource>(dbSettings);
        auto points = make_shared<PointResource>(dbSettings);
        auto upload = make_shared<LoggerResource>(dbSettings, worker_limit);
        auto reading = make_shared<ReadingResource>(dbSettings, worker_limit);
        auto device_model = make_shared<DeviceModelAssocResource>(dbSettings);
        auto device_status = make_shared<DeviceStatusResource>(dbSettings);

        m_srv_settings = make_shared<restbed::Settings>();
        m_srv_settings->set_status_messages(status::STATUS_MAP);
        m_srv_settings->set_port(port);
        m_srv_settings->set_root(root);
        m_srv_settings->set_worker_limit(worker_limit);
        m_srv_settings->set_connection_timeout(std::chrono::seconds(10));
        m_srv_settings->set_default_headers({
            {"Access-Control-Allow-Origin", "*"}
        });

        m_api.publish(devices);
        m_api.publish(models);
        m_api.publish(points);
        m_api.publish(upload);
        m_api.publish(reading);
        m_api.publish(device_model);
        m_api.publish(device_status);

        m_api.set_logger(make_shared<CelesteLogger>());
    }

    void CelesteServer::start()
    {
        m_api.start(m_srv_settings);
    }

    void CelesteServer::stop()
    {
        m_api.stop();
    }
}