/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#ifndef SERVER_RESOURCE_DEVICE_MODEL_ASSOC_HPP
#define SERVER_RESOURCE_DEVICE_MODEL_ASSOC_HPP

#include <string>
#include <memory>
#include <json.hpp>
#include <restbed>
#include <soci.h>
#include <boost/optional.hpp>
#include <object_pool.hpp>

namespace celeste
{   
namespace resource
{
    /**
     * @brief      Represents an association with 
     */
    struct DeviceModelAssoc
    {
        DeviceModelAssoc() = default;

        int                             idx;
        std::string                     DeviceId;
        std::string                     ModelId;
        boost::optional<std::string>    note;
    };

    template <class Json>
    class DeviceModelAssocs : public restbed::Resource
    {
    };

    /**
     * @brief      DeviceModelAssoc resource class.
     *
     * @tparam     Json  Json type to use
     */
    template <>
    class DeviceModelAssocs<nlohmann::json> : public restbed::Resource
    {
    public:
        // --- Typedefs --------------
        typedef nlohmann::json      json_type;

        // --- Constructors ----------

        /**
         * @brief      Creates a resource where you can access the DeviceModelAssoc
         * on the DB.
         *
         * @param[in]  dbSettings  DB settings for connection.
         */
        DeviceModelAssocs(const std::string& dbSettings);

        // --- Public methods --------

        /**
         * @brief      Gets the models of the Device.
         *
         * @param[in]  deviceId  The device identifier.
         *
         * @return     An `std::vector<Model>`.
         */
        std::vector<DeviceModelAssoc> get(const std::string& deviceId);
        std::vector<DeviceModelAssoc> get(const std::string& deviceId, const std::string& modelId);

        // TODO
        DeviceModelAssoc get(const std::string& deviceId, const std::string& modelId, int idx);

        // TODO
        void associate(const DeviceModelAssoc& assoc);

        // TODO
        void dissasociate(const std::string& deviceId, const std::string& modelId, int idx);

    private:
        // --- Private methods -------
        void GET(const std::shared_ptr<restbed::Session> session);
        void POST(const std::shared_ptr<restbed::Session> session);
        void DELETE(const std::shared_ptr<restbed::Session> session);

        // --- Member attributes -----
        carlosb::object_pool<soci::session>   sqlPool;
    };
}
}

// ---- SQL MAPPING ------------------
namespace soci
{
    template <>
    struct type_conversion<celeste::resource::DeviceModelAssoc>
    {
        typedef values base_type;

        static void from_base(values const& v,
                              indicator,
                              celeste::resource::DeviceModelAssoc& p);

        static void to_base(const celeste::resource::DeviceModelAssoc& p,
                            values& v,
                            indicator& ind);
    };
}

// --- JSON SERIALIZATION ------------
namespace nlohmann
{
    template <>
    struct adl_serializer<celeste::resource::DeviceModelAssoc>
    {
        static void to_json(json& j, const celeste::resource::DeviceModelAssoc& obj);
        static void from_json(const json& j, celeste::resource::DeviceModelAssoc& dm);
    }; 
}

#endif