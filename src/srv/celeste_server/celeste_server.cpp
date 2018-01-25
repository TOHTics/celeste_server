#include "celeste_server.hpp"

#include <iostream>
#include <memory>

using namespace std;

namespace celeste
{
    celeste_server::celeste_server(int port, string dbSettings,  int worker_limit, std::string root)
    {
        cout << "\e[33m";
        cout << "@ Making resources...\n";
        auto devices = make_shared<DeviceResource>(dbSettings, worker_limit);
        auto models = make_shared<ModelResource>(dbSettings, worker_limit);
        auto points = make_shared<PointResource>(dbSettings, worker_limit);
        auto upload = make_shared<LoggerResource>(dbSettings, worker_limit);
        auto reading = make_shared<ReadingResource>(dbSettings, worker_limit);
        auto device_model = make_shared<DeviceModelAssocResource>(dbSettings, worker_limit);
        auto device_status = make_shared<DeviceStatusResource>();

        cout << "@ Configuring server...\n";
        m_srv_settings = make_shared<restbed::Settings>();
        m_srv_settings->set_status_messages(status::STATUS_MAP);
        m_srv_settings->set_port(port);
        m_srv_settings->set_root(root);
        m_srv_settings->set_worker_limit(worker_limit);
        m_srv_settings->set_default_headers({
            {"Access-Control-Allow-Origin", "*"}
        });

        cout << "@ Publishing resources...\n";

        m_api.publish(devices);
        m_api.publish(models);
        m_api.publish(points);
        m_api.publish(upload);
        m_api.publish(reading);
        m_api.publish(device_model);
        m_api.publish(device_status);
    }

    void celeste_server::start()
    {
        m_api.start(m_srv_settings);
    }

    void celeste_server::stop()
    {
        m_api.stop();
    }
}