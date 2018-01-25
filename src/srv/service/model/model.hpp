

/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#ifndef SERVER_RESOURCE_MODEL_HPP
#define SERVER_RESOURCE_MODEL_HPP

#include <string>
#include <memory>
#include <json.hpp>
#include <restbed>
#include <soci.h>
#include <object_pool.hpp>
#include <boost/optional.hpp>
#include <mutex>

namespace celeste
{   
namespace resource
{
    /**
     * @brief      Represents a Model on the DB.
     */
    struct Model
    {
        typedef std::string id_type;

        Model() = default;

        id_type                         ModelId;
        boost::optional<std::string>    ns;
    };

    /**
     * @brief      Models resource class.
     *
     * @tparam     Json  Json type to use
     */
    template <class Json>
    class Models : public restbed::Resource
    {
    };

    /**
     * @brief      Specialization for `nlohmann::json`.
     */
    template <>
    class Models<nlohmann::json> : public restbed::Resource
    {
    public:
        // --- Typedefs --------------
        typedef Model               value_type;
        typedef nlohmann::json      json_type;

        // --- Constructors ----------

        /**
         * @brief      Creates a resource where you can access the Models
         * on the DB.
         *
         * @param[in]  dbSettings  DB settings for connection.
         */
        Models(const std::string& dbSettings, size_t max_connections);

        // --- Public methods --------

        /**
         * @brief      Gets a Model from the DB.
         *
         * @param[in]  modelId  The Model identifier.
         *
         * @return     Fetched Model.
         */
        Model get(const std::string& modelId);

        /**
         * @brief      Inserts a Model into the DB.
         *
         * @param[in]  model   Model to be inserted.
         */
        void insert(const value_type& model);

        /**
         * @brief      Removes a Model from the DB.
         *
         * @param[in]  modelId  The Model identifier
         */
        void remove(const std::string& modelId);

    private:
        // --- Private methods -------
        void GET(const std::shared_ptr<restbed::Session> session);
        void POST(const std::shared_ptr<restbed::Session> session);
        void DELETE(const std::shared_ptr<restbed::Session> session);

        // --- Member attributes -----
        carlosb::object_pool<soci::session>     sqlPool;
    };
}
}

// --- JSON SERIALIZATION ------------
namespace nlohmann
{
    template <>
    struct adl_serializer<celeste::resource::Model>
    {
        static void to_json(json& j, const celeste::resource::Model& obj);
        static void from_json(const json& j, celeste::resource::Model& obj);
    }; 
}

// ---- SQL MAPPING ------------------
namespace soci
{
    template <>
    struct type_conversion<celeste::resource::Model>
    {
        typedef values base_type;

        static void from_base(values const& v,
                              indicator,
                              celeste::resource::Model& p);

        static void to_base(const celeste::resource::Model& p,
                            values& v,
                            indicator& ind);
    };
}

#endif
