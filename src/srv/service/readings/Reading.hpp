#ifndef CELESTE_RESOURCE_READING_HPP
#define CELESTE_RESOURCE_READING_HPP

#include <soci.h>
#include <json.hpp>
#include <boost/variant.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace celeste
{
namespace resource
{   
    /**
     * @brief      Represents the type that a point might take
     */
    enum point_type_index : int
    {
        INTEGER = 0,
        FLOAT,
        STRING
    };

    /**
     * @brief      Represents a reading.
     */
    struct Reading
    {
        using point_type = boost::variant<int, float, std::string>;

        boost::optional<double>                     sf;         ///< Scale factor
        boost::optional<boost::posix_time::ptime>   t;          ///< timestamp
        point_type                                  value;      ///< value
    };
}
}

// ==== JSON SERIALIZATION ===============================
namespace nlohmann
{
    template <>
    struct adl_serializer<celeste::resource::Reading>
    {
        static
        void to_json(json& j, const celeste::resource::Reading& obj);
        
        static
        void from_json(const json& j, celeste::resource::Reading& obj);
    };

    template <>
    struct adl_serializer<celeste::resource::Reading::point_type>
    {
        static
        void to_json(json& j, const celeste::resource::Reading::point_type& obj);
        
        static
        void from_json(const json& j, celeste::resource::Reading::point_type& obj);
    };
}

// ==== SQL MAPPING ======================================
namespace soci
{
    template <>
    struct type_conversion<celeste::resource::Reading>
    {
        typedef values base_type;

        static void from_base(values const& v,
                              indicator,
                              celeste::resource::Reading& p);

        static void to_base(const celeste::resource::Reading& p,
                            values& v,
                            indicator& ind);
    };

    template <>
    struct type_conversion<celeste::resource::Reading::point_type>
    {
        typedef std::string base_type;

        static void from_base(std::string const& v,
                              indicator,
                              celeste::resource::Reading::point_type& p);

        static void to_base(const celeste::resource::Reading::point_type& p,
                            std::string& v,
                            indicator& ind);
    };
}
#endif
