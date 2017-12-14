#ifndef CELESTE_RESOURCE_READING_HPP
#define CELESTE_RESOURCE_READING_HPP

#include <type_traits>

#include "srv/service/common.hpp"
#include "srv/db/db.hpp"

namespace celeste
{
namespace resource
{
    template <class T>
    struct Reading
    {
        static_assert(std::is_default_constructible<T>::value,
                      "Type T for Reading<T> must be default constructible");

        typedef T value_type;

        boost::optional<double>                     sf;
        boost::optional<boost::posix_time::ptime>   t;
        T                                           value;
    };
}
}

// --- JSON SERIALIZATION ------------
namespace nlohmann
{
    template <class T>
    struct adl_serializer<celeste::resource::Reading<T>>
    {
        static
        void to_json(json& j, const celeste::resource::Reading<T>& obj)
        {
            j = json {
                {"sf",      obj.sf},
                {"t",       obj.t},
                {"value",   obj.value}
            };
        }

        
        static
        void from_json(const json& j, celeste::resource::Reading<T>& obj)
        {
            obj.sf     = j.at("sf");
            obj.t      = j.at("t");
            obj.value  = mysqlx::EnhancedValue{j.at("value")};
        }
    };
}

namespace mysqlx
{
    template <class T>
    struct row_serializer<celeste::resource::Reading<T>>
    {
        static
        void to_row (SerializableRow& row, const celeste::resource::Reading<T>& obj)
        {
            row.set(0, EnhancedValue{obj.sf});
            row.set(1, EnhancedValue{obj.t});
            row.set(2, EnhancedValue{obj.value});
        }
        
        static
        void from_row (const SerializableRow& row, celeste::resource::Reading<T>& obj)
        {
            SerializableRow tmp = row; // row.get() is not marked const, hence we need this tmp
            
            obj.sf         = tmp.get(0).get<boost::optional<double>>();
            obj.t          = tmp.get(1).get<boost::optional<boost::posix_time::ptime>>();
            obj.value      = tmp.get(2);
        }
    };
}
#endif
