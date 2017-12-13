#ifndef CELESTE_DB_TRANSLATOR_IMPL_HPP
#define CELESTE_DB_TRANSLATOR_IMPL_HPP

#include <boost/optional.hpp>
#include <mysql_devapi.h>
#include "db.hpp"

namespace mysqlx
{
    // --- OPTIONAL TRANSLATION ------
    template <class T>
    boost::optional<T> value_translator<boost::optional<T>>::get(const Value& in)
    {
        if (in.isNull())
            return boost::none;
        else
            return boost::optional<T>(value_translator<T>::get(in));
    }

    template <class T>
    Value value_translator<boost::optional<T>>::put(const boost::optional<T>& in)
    {
        if (in)
            return value_translator<T>::put(*in);
        else
            return static_cast<Value>(nullptr);
    }
}


#endif