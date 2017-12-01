/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 3/Nov/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#ifndef CELESTE_SERVICES_READINGS_HPP
#define CELESTE_SERVICES_READINGS_HPP

#include <restbed>
#include <memory>
#include <mysql_devapi.h>
#include <json.hpp>

namespace celeste
{
namespace resource
{
    class Reading
    {
    public:
        Reading() = default;
        
        nlohmann::json
        operator()(nlohmann::json&& j)
        {
            return get(std::forward<nlohmann::json>(j));
        }

        virtual
        nlohmann::json
        get(nlohmann::json&&) const =0;

        virtual
        nlohmann::json
        get(const nlohmann::json&) const =0;

        virtual
        std::vector<std::string>
        required_fields() const =0;
    };

    class LastReading : public Reading
    {
        const std::shared_ptr<mysqlx::Session> dbSession;

    public:
        LastReading(const std::shared_ptr<mysqlx::Session>);

        std::vector<std::string>
        required_fields() const override;

        nlohmann::json
        get(nlohmann::json&&) const override;


        nlohmann::json
        get(const nlohmann::json&) const override;
        

        nlohmann::json
        get_data(const std::string& point_id,
                 const std::string& model_id,
                 int device_id) const;
    };

    class RangeReading : public Reading
    {
        const std::shared_ptr<mysqlx::Session> dbSession;

    public:
        RangeReading(const std::shared_ptr<mysqlx::Session>);

        constexpr
        std::vector<std::string>
        required_fields() const override;

        nlohmann::json
        get(nlohmann::json&&) const override;


        nlohmann::json
        get(const nlohmann::json&) const override;
        

        nlohmann::json
        get_data(const std::string& point_id,
                 const std::string& model_id,
                 int device_id,
                 const std::string& start,
                 const std::string& end) const;
    };

}
}

#endif