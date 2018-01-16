#ifndef CELESTE_SERVICE_DEVICE_STATUS_HPP
#define CELESTE_SERVICE_DEVICE_STATUS_HPP

#include <string>
#include <memory>
#include <soci.h>
#include <json.hpp>
#include <restbed>

namespace celeste
{
namespace resource
{
    struct DeviceStatus
    {
        bool            isPowerCut;
        bool            isOn;
        bool            isConnectedToServer;
        bool            hasMalfunction;

        std::string     DeviceId;
        int             pingTimeToServer;
    };

    template <class Json>
    class DeviceStatusService : public restbed::Resource
    {

    };

    template <>
    class DeviceStatusService<nlohmann::json> : public restbed::Resource
    {
    public:
        // --- Typedefs --------------
        typedef nlohmann::json      json_type;

        // --- Constructors ----------
        DeviceStatusService();

        DeviceStatus get(const std::string& deviceId);
        bool isPowerCut(const std::string& deviceId);
        [[deprecated]] int get_arduino(const std::string& deviceId);

    private:
        // --- Private methods -------
        void GET(const std::shared_ptr<restbed::Session> session);
        // void POST(const std::shared_ptr<restbed::Session> session);
    };
}
}

// --- JSON SERIALIZATION --------------------------------
namespace nlohmann
{
    template <>
    struct adl_serializer<celeste::resource::DeviceStatus>
    {
        static void to_json(json& j, const celeste::resource::DeviceStatus& obj);
        static void from_json(const json& j, celeste::resource::DeviceStatus& obj);
    }; 
}

#endif