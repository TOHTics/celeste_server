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
#include <mysql_devapi.h>
#include <json.hpp>
#include <restbed>
#include <functional>
#include <boost/optional.hpp>

#include "srv/db/db.hpp"
#include "../device/device.hpp"
#include "../model/model.hpp"

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
        int                             DeviceId;
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
        DeviceModelAssocs(const mysqlx::SessionSettings& dbSettings);

        // --- Public methods --------

        /**
         * @brief      Gets the models of the Device.
         *
         * @param[in]  deviceId  The device identifier.
         *
         * @return     An `std::vector<Model>`.
         */
        std::vector<DeviceModelAssoc> get_all(int deviceId);
        std::vector<DeviceModelAssoc> get_all(int deviceId, const std::string& modelId);

        // TODO
        DeviceModelAssoc get(int deviceId, const std::string& modelId, int idx);

        // TODO
        int associate(const DeviceModelAssoc& dm);

        // TODO
        void dissasociate(int deviceId, const std::string& modelId, int idx);

    private:
        // --- Private methods -------
        void GET(const std::shared_ptr<restbed::Session> session);
        void POST(const std::shared_ptr<restbed::Session> session);
        void DELETE(const std::shared_ptr<restbed::Session> session);

        // --- Member attributes -----
        mysqlx::Session     dbSession;
        mysqlx::Schema      celesteDB;
        mysqlx::Table       associationTable;
    };
}
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

// --- SQL SERIALIZATION -------------
namespace mysqlx
{
    template <>
    struct row_serializer<celeste::resource::DeviceModelAssoc>
    {
        static void to_row (SerializableRow& row, const celeste::resource::DeviceModelAssoc& obj);
        static void from_row (const SerializableRow& row, celeste::resource::DeviceModelAssoc& obj);

    };
}

#endif