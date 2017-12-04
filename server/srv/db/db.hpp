#ifndef SERVER_DB_DB_HPP
#define SERVER_DB_DB_HPP

#include <memory>
#include <restbed>
#include <json.hpp>
#include <mysql_devapi.h>
#include <boost/optional.hpp>
#include <type_traits>
#include <locale>
#include <codecvt>

namespace nlohmann
{
    template <>
    struct adl_serializer<mysqlx::Value>
    {
        static void to_json(json& j, const mysqlx::Value& value);
        static void from_json(const json& j, mysqlx::Value& value);
    };


    template <>
    struct adl_serializer<mysqlx::RowResult>
    {
        static void to_json(json& j, mysqlx::RowResult res);
    };

    template <>
    struct adl_serializer<mysqlx::SqlResult>
    {
        static void to_json(json& j, mysqlx::SqlResult res);
    };
}

namespace mysqlx
{

    class EnhancedValue;
    class SerializableRow;

    template <class T>
    struct row_serializer
    {
        static void to_row (SerializableRow& row, const T& value);
        static void from_row(const SerializableRow& row, T& value);
    };

    class EnhancedValue : public Value
    {
        template <class T>
        using optional_t = boost::optional<T>;
    public:
        EnhancedValue (Value&& value)
            : Value (value)
        {}

        EnhancedValue (const Value& value)
            : Value (value)
        {}

        template <class T>
        EnhancedValue (const T& value)
            : Value(value)
        {}

        template <class T>
        EnhancedValue (const optional_t<T>& value)
            : EnhancedValue(*value)
        {}

        EnhancedValue (const boost::none_t value)
            : Value (nullptr)
        {}

        explicit // prevent ambiguity with mysqlx::string
        EnhancedValue (const std::string& value)
            : Value (value.c_str())
        {}

        explicit // prevent ambiguity with mysqlx::string
        operator std::string()
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
            return converter.to_bytes(this->get<string>());
        }

        template <class T>
        operator optional_t<T>()
        {
            if (this->isNull())
                return boost::none;
            else
                return boost::optional<T>(this->get<T>());
        }

        template <class T>
        T get()
        {   
            static_assert(std::is_convertible<T, EnhancedValue>::value, "Type is not convertible to mysqlx::EnhancedValue.");
            return T(*this);
        }
    };

    class SerializableRow : public Row
    {
    public:

        SerializableRow()
            : Row()
        {}

        SerializableRow (Row&& row)
        {
            for (int i = 0; i < row.colCount(); ++i)
                this->set(i, row.get(i));
        }

        template <class T>
        void from(const T& value)
        {
            SerializableRow row;
            row_serializer<T>::to_row(row, value);
            *this = row;
        }

        template <class T>
        T as()
        {
            T value;
            row_serializer<T>::from_row(*this, value);
            return value;
        }

        EnhancedValue get(col_count_t pos)
        {
            return EnhancedValue(this->Row::get(pos));
        }
    };
}

namespace celeste
{
    namespace db
    {
    /**
     * @brief      Makes a MySQL database session.
     *
     * @param[in]  args  Arguments.
     *
     * @tparam     Args  Argument pack.
     *
     * @return     MySQL database session.
     * 
     * In general, we may use this function like:
     * `auto dbSession = make_db_session("localhost", 33060, "user", "password")`
     */
    template<typename... Args>
    std::shared_ptr<mysqlx::Session> make_db_session(Args... args)
    {   
        return std::shared_ptr<mysqlx::Session>(new mysqlx::Session(args ...));
    }


    /**
     * @brief      Makes a MySQL database session.
     *
     * @param[in]  host  The host
     * @param[in]  port  The port
     * @param[in]  user  The user
     * @param[in]  pwd   The password
     *
     * @return     MySQL database session.
     */
    std::shared_ptr<mysqlx::Session> make_db_session(std::string host, int port, std::string user, std::string pwd);

    }
}
#endif