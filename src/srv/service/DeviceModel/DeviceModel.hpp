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
        // ==== Typedefs =================================
        typedef nlohmann::json      json_type;

        // ==== Constructors =============================

        /**
         * @brief      Creates a resource where you can access the DeviceModelAssoc
         * on the DB.
         *
         * @param[in]  dbSettings  DB settings for connection.
         */
        DeviceModelAssocs(const std::string& dbSettings);

        // ==== Public methods ===========================

        /**
         * @brief      Gets the list of models that are associated with the Device.
         *
         * @param[in]  deviceId  The device identifier.
         *
         * @return     Vector of associations.
         */
        std::vector<DeviceModelAssoc> get(const std::string& deviceId);

        /**
         * @brief      Gets the list of models associated with the Device.
         * Since associations may be aggregated, the distinction will be
         * reflected in the `idx` field.
         *
         * @param[in]  deviceId  The device identifier
         * @param[in]  modelId   The model identifier
         *
         * @return     Vector of associations.
         */
        std::vector<DeviceModelAssoc> get(const std::string& deviceId, const std::string& modelId);

        /**
         * @brief      Gets a single association on the Device and the Model with aggregated index.
         *
         * @param[in]  deviceId  The device identifier
         * @param[in]  modelId   The model identifier
         * @param[in]  idx       The index
         *
         * @return     { description_of_the_return_value }
         */
        DeviceModelAssoc get(const std::string& deviceId, const std::string& modelId, int idx);

        /**
         * @brief      Associates a Model to a Device using a `DeviceModelAssoc` object.
         *
         * @param[in]  assoc  The association
         */
        void associate(const DeviceModelAssoc& assoc);

        /**
         * @brief      Dissasociates a Model from a Device. It is necessary to specify
         * the Device identifier, the Model identifier and which aggregated index you need
         * to dissasociate.
         *
         * @param[in]  deviceId  The device identifier
         * @param[in]  modelId   The model identifier
         * @param[in]  idx       The index
         */
        void dissasociate(const std::string& deviceId, const std::string& modelId, int idx);

    private:
        // --- Private methods -------
        void GET(const std::shared_ptr<restbed::Session> session);
        void POST(const std::shared_ptr<restbed::Session> session);
        void DELETE(const std::shared_ptr<restbed::Session> session);

        // --- Member attributes -----
        std::string   m_dbSettings;
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