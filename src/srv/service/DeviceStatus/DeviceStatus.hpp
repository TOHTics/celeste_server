#ifndef CELESTE_SERVICE_DEVICE_STATUS_HPP
#define CELESTE_SERVICE_DEVICE_STATUS_HPP

#include <string>
#include <memory>
#include <soci.h>
#include <json.hpp>
#include <restbed>
#include <object_pool.hpp>

namespace celeste
{
namespace resource
{

    template <class Status>
    struct BasicDeviceStatus;

    template <class Status>
    struct BasicDeviceStatus
    { 
        std::string DeviceId;
        Status      status;
    };

    using DeviceStatus = BasicDeviceStatus<nlohmann::json>;

    template <class Json>
    class DeviceStatusService : public restbed::Resource
    {

    };

    template <>
    class DeviceStatusService<nlohmann::json> : public restbed::Resource
    {
    public:
        // ==== Typedefs =================================
        typedef nlohmann::json      json_type;

        // ==== Constructors =============================
        DeviceStatusService(const std::string& dbSettings);

        /**
         * @brief      Retrieves the status of a Device.
         *
         * @param[in]  deviceId  The device identifier
         *
         * @return     The status.
         */
        DeviceStatus get_status(const std::string& deviceId);

        /**
         * @brief      Updates the status of a Device.
         *
         * @param[in]  deviceId    The device identifier
         * @param[in]  new_status  The new status
         */
        void update_status(const std::string& deviceId, const json_type& new_status);


    private:
        // ==== Private methods ==========================
        void GET(const std::shared_ptr<restbed::Session> session);
        void PUT(const std::shared_ptr<restbed::Session> session);

        // ==== Private members ==========================
        std::string m_dbSettings;
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