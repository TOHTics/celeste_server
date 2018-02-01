/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#ifndef SERVER_RESOURCE_DEVICE_HPP
#define SERVER_RESOURCE_DEVICE_HPP

#include <string>
#include <memory>
#include <json.hpp>
#include <restbed>
#include <soci.h>
#include <object_pool.hpp>

#include "../DeviceModel/DeviceModel.hpp"

namespace celeste
{   
namespace resource
{
    /**
     * @brief      Represents a Device on the DB.
     */
    struct Device
    {
        std::string             DeviceId;
        std::string             man;
        std::string             mod;
        std::string             sn;
        std::string             password;
    };

    template <class Json>
    class Devices : public restbed::Resource
    {
    };

    /**
     * @brief      Devices resource class.
     *
     * @tparam     Json  Json type to use
     */
    template <>
    class Devices<nlohmann::json> : public restbed::Resource
    {
    public:
        // ==== Typedefs =================================
        typedef Device              value_type;
        typedef nlohmann::json      json_type;

        // ==== Constructors =============================

        /**
         * @brief      Creates a resource where you can access the Devices
         * on the DB.
         *
         * @param[in]  dbSettings  DB settings for connection.
         * @param[in]  max_connections  max DB connections.
         */
        Devices(const std::string& dbSettings);

        // ==== Public methods =============================

        /**
         * @brief      Gets a Device from the DB.
         *
         * @param[in]  deviceId  The device identifier.
         *
         * @return     Fetched Device.
         */
        Device get(const std::string& deviceId);

        /**
         * @brief      Inserts a Device into the DB.
         *
         * @param[in]  device   Device to be inserted.
         */
        void insert(const value_type& device, const std::string& pwd);

        /**
         * @brief      Inserts a device into the DB with its associated models.
         * The models are, however, not inserted.
         *
         * @param[in]  device  Device to be inserted
         * @param[in]  models  Vector of the ids of the Models
         */
        void insert(const value_type& device, const std::string& pwd, std::vector<std::string>models);

        /**
         * @brief      Removes a model from the DB.
         *
         * @param[in]  deviceId  The device identifier
         */
        void remove(const std::string& deviceId);

    private:
        // ==== Private methods ==========================
        void GET(const std::shared_ptr<restbed::Session> session);
        void POST(const std::shared_ptr<restbed::Session> session);
        void DELETE(const std::shared_ptr<restbed::Session> session);

        // ==== Member attributes =========================
        std::string                     m_dbSettings;
    };
}
}

// ==== SQL MAPPING ======================================
namespace soci
{
    template <>
    struct type_conversion<celeste::resource::Device>
    {
        typedef values base_type;

        static void from_base(values const& v,
                              indicator,
                              celeste::resource::Device& p);

        static void to_base(const celeste::resource::Device& p,
                            values& v,
                            indicator& ind);
    };
}

// ==== JSON SERIALIZATION ===============================
namespace nlohmann
{
    template <>
    struct adl_serializer<celeste::resource::Device>
    {
        static void to_json(json& j, const celeste::resource::Device& device);
        static void from_json(const json& j, celeste::resource::Device& device);
    }; 
}

#endif