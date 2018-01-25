/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#ifndef SERVER_RESOURCE_POINT_HPP
#define SERVER_RESOURCE_POINT_HPP

#include <string>
#include <memory>
#include <json.hpp>
#include <restbed>
#include <soci.h>
#include <object_pool.hpp>
#include <boost/optional.hpp>


namespace celeste
{   
namespace resource
{
    /**
     * @brief      Represents a Point on the DB.
     */
    struct Point
    {
        typedef std::string id_type;

        Point() = default;

        id_type                         PointId;
        std::string                     ModelId;
        int                             type;
        boost::optional<std::string>    u;
        boost::optional<std::string>    d;
    };

    /**
     * @brief      Points resource class.
     *
     * @tparam     Json  Json type to use
     */
    template <class Json>
    class Points : public restbed::Resource
    {
    };

    /**
     * @brief      Specialization for `nlohmann::json`.
     */
    template <>
    class Points<nlohmann::json> : public restbed::Resource
    {
    public:
        // --- Typedefs --------------
        typedef Point               value_type;
        typedef nlohmann::json      json_type;

        // --- Constructors ----------

        /**
         * @brief      Creates a resource where you can access the Points
         * on the DB.
         *
         * @param[in]  dbSettings  DB settings for connection.
         */
        Points(const std::string& dbSettings, size_t max_connections);

        // --- Public methods --------

        /**
         * @brief      Gets a Point from the DB.
         *
         * @param[in]  PointId  The Point identifier.
         *
         * @return     Fetched Point.
         */
        Point get(const std::string& pointId, const std::string& modelId);

        /**
         * @brief      Inserts a Point into the DB.
         *
         * @param[in]  Point   Point to be inserted.
         */
        void insert(const value_type& Point);

        /**
         * @brief      Removes a Point from the DB.
         *
         * @param[in]  PointId  The Point identifier
         */
        void remove(const std::string& pointId, const std::string& modelId);

    private:
        // --- Private methods -------
        void GET(const std::shared_ptr<restbed::Session> session);
        void POST(const std::shared_ptr<restbed::Session> session);
        void DELETE(const std::shared_ptr<restbed::Session> session);

        // --- Member attributes -----
        carlosb::object_pool<soci::session> sqlPool;
    };
}
}

// --- JSON SERIALIZATION ------------
namespace nlohmann
{
    template <>
    struct adl_serializer<celeste::resource::Point>
    {
        static void to_json(json& j, const celeste::resource::Point& obj);
        static void from_json(const json& j, celeste::resource::Point& obj);
    }; 
}

// ---- SQL MAPPING ------------------
namespace soci
{
    template <>
    struct type_conversion<celeste::resource::Point>
    {
        typedef values base_type;

        static void from_base(values const& v,
                              indicator,
                              celeste::resource::Point& p);

        static void to_base(const celeste::resource::Point& p,
                            values& v,
                            indicator& ind);
    };
}

#endif
