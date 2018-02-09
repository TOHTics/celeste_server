/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 7/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#ifndef SERVER_RESOURCE_APIUSER_HPP
#define SERVER_RESOURCE_APIUSER_HPP

#include <string>
#include <memory>
#include <json.hpp>
#include <restbed>
#include <soci.h>
#include <object_pool.hpp>


namespace celeste
{   
namespace resource
{
    /**
     * @brief      Represents a APIUser on the DB.
     */
    struct APIUser
    {
        std::string     UserId;
        std::string     ugroup;   
    };

    template <class Json>
    class APIUsers : public restbed::Resource
    {
    };

    /**
     * @brief      APIUsers resource class.
     *
     * @tparam     Json  Json type to use
     */
    template <>
    class APIUsers<nlohmann::json> : public restbed::Resource
    {
    public:
        // ==== Typedefs =================================
        typedef APIUser             value_type;
        typedef nlohmann::json      json_type;

        // ==== Constructors =============================

        /**
         * @brief      Creates a resource where you can access the APIUsers
         * on the DB.
         *
         * @param[in]  dbSettings  DB settings for connection.
         * @param[in]  max_connections  max DB connections.
         */
        APIUsers(const std::string& dbSettings);

        // ==== Public methods =============================

        /**
         * @brief      Gets a APIUser from the DB.
         *
         * @param[in]  id  The APIUser identifier.
         *
         * @return     Fetched APIUser.
         */
        APIUser get(const std::string& id);

        /**
         * @brief      Inserts a APIUser into the DB.
         *
         * @param[in]  APIUser      APIUser to be inserted.
         * @param[in]  pwd          Password of the APIUser.
         */
        void insert(const value_type& user, const std::string& pwd);

        /**
         * @brief      Updates the user on the db
         *
         * @param[in]  new_user  The new user
         */
        void update(const value_type& new_user);

        /**
         * @brief      Removes a model from the DB.
         *
         * @param[in]  APIUserId  The APIUser identifier
         */
        void remove(const std::string& id);

    private:
        // ==== Private methods ==========================
        void GET(const std::shared_ptr<restbed::Session> session);
        void POST(const std::shared_ptr<restbed::Session> session);
        void PUT(const std::shared_ptr<restbed::Session> session);
        void DELETE(const std::shared_ptr<restbed::Session> session);

        // ==== Member attributes =========================
        std::string     m_db_settings;
    };
}
}

// ==== SQL MAPPING ======================================
namespace soci
{
    template <>
    struct type_conversion<celeste::resource::APIUser>
    {
        typedef values base_type;

        static void from_base(values const& v,
                              indicator,
                              celeste::resource::APIUser& p);

        static void to_base(const celeste::resource::APIUser& p,
                            values& v,
                            indicator& ind);
    };
}

// ==== JSON SERIALIZATION ===============================
namespace nlohmann
{
    template <>
    struct adl_serializer<celeste::resource::APIUser>
    {
        static void to_json(json& j, const celeste::resource::APIUser& APIUser);
        static void from_json(const json& j, celeste::resource::APIUser& APIUser);
    }; 
}

#endif