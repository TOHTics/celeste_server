#include "DeviceStatus.hpp"
#include "srv/service/common.hpp"

using namespace std;

namespace celeste
{
namespace resource
{
    DeviceStatusService<nlohmann::json>::DeviceStatusService()
    {
        set_paths({"/device/status", "/device/status/arduino"});
        set_method_handler("POST", [this] (const shared_ptr<restbed::Session> session) {GET(session);});
        // set_method_handler("POST",   [this] (const shared_ptr<restbed::Session> session) {POST(session);});
    }

    bool DeviceStatusService<nlohmann::json>::isPowerCut(const std::string& deviceId)
    {
        // make a request to CRM server
        auto request = make_shared<restbed::Request>(restbed::Uri("http://work.tohtics.com:3330/api/auth/" + deviceId));
        request->set_method("POST");

        auto response = restbed::Http::sync(request);
        auto length = response->get_header("Content-Length", 0);
        restbed::Http::fetch(length, response);

        std::string body;
        bytes2string(response->get_body(), body);

        if (response->get_status_code() == restbed::NOT_FOUND)
            throw status::DEVICE_NOT_FOUND;

        json_type j = json_type::parse(body);
        
        return (j["device_status"].get<string>() == "OFF");
    }

    DeviceStatus DeviceStatusService<nlohmann::json>::get(const string& deviceId)
    {

        auto status = DeviceStatus{
            .DeviceId               = deviceId,
            .isPowerCut             = this->isPowerCut(deviceId),
            .isOn                   = true,
            .isConnectedToServer    = true,
            .hasMalfunction         = false,
            .pingTimeToServer       = 89
        };

        return status;
    }

    [[deprecated]] int DeviceStatusService<nlohmann::json>::get_arduino(const string& deviceId)
    {
        return 0;
        // return static_cast<int>(this->isPowerCut(deviceId));
    }
    void DeviceStatusService<nlohmann::json>::GET(const shared_ptr<restbed::Session> session)
    {
        // get request
        const auto request = session->get_request();

        // get headers
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        // fetch data to access later
        session->fetch(content_length, [] (const shared_ptr<restbed::Session>, const restbed::Bytes&) {});

        // get json from request
        json_type data = get_json<json_type>(*request);

        // validate data
        if (data["DeviceId"].is_null())
            throw status::MISSING_FIELD_DEVICEID;

        // get device from db
        json_type response;
        if (request->get_path() == "/celeste/device/status/arduino")
            response = this->get_arduino(data["DeviceId"]);
        else
            response = this->get(data["DeviceId"]);

        // close
        session->close(restbed::OK,
                       response.dump(),
                       {
                            { "Content-Length", to_string(response.dump().size()) },
                            { "Connection",     "close" }
                       });
    }
}
}

namespace nlohmann
{
    using namespace celeste::resource;

    void adl_serializer<DeviceStatus>::to_json(json& j, const DeviceStatus& obj)
    {
        j = json {
            // {"DeviceId",            obj.DeviceId},
            {"isPowerCut",          obj.isPowerCut},
            // {"isOn",                obj.isOn},
            // {"isConnectedToServer", obj.isConnectedToServer},
            // {"hasMalfunction",      obj.hasMalfunction},
            // {"pingTimeToServer",    obj.pingTimeToServer}
        };
    }

    void adl_serializer<DeviceStatus>::from_json(const json& j, DeviceStatus& obj)
    {
        obj = DeviceStatus {
            // .DeviceId               = j.at("DeviceId"),
            .isPowerCut             = j.at("isPowerCut"),
            // .isOn                   = j.at("isOn"),
            // .isConnectedToServer    = j.at("hasMalfunction"),
            // .pingTimeToServer       = j.at("pingTimeToServer")
        };
    }
}