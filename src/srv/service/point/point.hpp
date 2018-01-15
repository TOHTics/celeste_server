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
#include <mysql_devapi.h>
#include <json.hpp>
#include <restbed>
#include <functional>
#include <boost/optional.hpp>

#include "srv/db/db.hpp"

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
        Points(const celeste::SessionSettings& dbSettings);

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
        celeste::SessionSettings    dbSettings;

        mysqlx::Session             dbSession;
        mysqlx::Schema              celesteDB;
        mysqlx::Table               pointTable;

        std::mutex                  point_mutex;
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

// --- SQL SERIALIZATION -------------
namespace mysqlx
{
    template <>
    struct row_serializer<celeste::resource::Point>
    {
        static void to_row (SerializableRow& row, const celeste::resource::Point& obj);
        static void from_row (const SerializableRow& row, celeste::resource::Point& obj);

    };
}

#endif
