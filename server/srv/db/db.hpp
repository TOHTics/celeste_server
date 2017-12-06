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

namespace mysqlx
{
    class EnhancedValue;
    class SerializableRow;

    /**
     * @brief      This class aids in the serialization of rows. By specializing
     * the methods in the class we can write our own serialization functions.
     *
     * @tparam     T     Type of object to serialize.
     */
    template <class T>
    struct row_serializer
    {
        static void to_row (SerializableRow& row, const T& value);
        static void from_row(const SerializableRow& row, T& value);
    };

    /**
     * @brief      Adds functionality to the Value class. This is a key
     * component used in serializing rows. You may add conversions for a
     * type `T` by writing the cast operator and the copy constructor:
     * 
     * `operator T() {...}`
     * `EnhancedValue(const T& value) : Value (...) {...}`
     * 
     */
    class EnhancedValue : public Value
    {
        template <class T>
        using optional_type = boost::optional<T>; /// Optional type

        using none_type = boost::none_t; /// None type.

    public:
        /**
         * @brief      Copy constructor for `mysqlx::Value`.
         * This is more of a compatibility constructor to ensure
         * we can copy content from `Value`.
         *
         * @param[in]  value  Value.
         */
        EnhancedValue (Value&& value)
            : Value (std::forward<Value>(value))
        {}

        /**
         * @brief      Copy constructor for base class.
         *
         * @param[in]  value  Base class instance.
         */
        EnhancedValue (const Value& value)
            : Value (value)
        {}

        /**
         * @brief      Copy constructor for any conversion.
         *
         * @param[in]  value  Value of type `T`. 
         *
         * @tparam     T      Must be convertible to `mysqlx::Value`.
         */
        template <class T>
        EnhancedValue (const T& value)
            : Value(value)
        {
            static_assert(std::is_convertible<T, Value>::value, "Cannot convert type T to mysqlx::Value.");
        }

        /**
         * @brief      Construct `EnhancedValue` from an optional value
         * Namely, the type is `boost::optional`.
         *
         * @param[in]  value  Optional value.
         *
         * @tparam     T      Type contained in the optional value.
         */
        template <class T>
        EnhancedValue (const optional_type<T>& value)
        {
            if (value)
                EnhancedValue(*value);
            else
                EnhancedValue(nullptr);
        }

        /**
         * @brief      Construct a null value from a `none_type` object.
         *
         * @param[in]  value  Null value.
         */
        EnhancedValue (const none_type value)
            : Value (nullptr)
        {}

        /**
         * @brief      Enables compatibility with `std::string`.
         *
         * @param[in]  value  String value.
         */
        explicit // prevent ambiguity with mysqlx::string
        EnhancedValue (const std::string& value)
            : Value (value.c_str())
        {}

        /**
         * @brief      Converts a `std::string` to `mysqlx::Value`.
         */
        explicit // prevent ambiguity with mysqlx::string
        operator std::string()
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
            return converter.to_bytes(this->get<string>());
        }

        /**
         * @brief      Converts to `optional_type`.
         *
         * @tparam     T     { description }
         */
        template <class T>
        operator optional_type<T>()
        {
            if (this->isNull())
                return boost::none;
            else
                return boost::optional<T>(this->get<T>());
        }

        /**
         * @brief      Gets the desired value in its proper type representation.
         *
         * @tparam     T     Type contained in value.
         *
         * @return     Value of type `T`.
         */
        template <class T>
        T get()
        {   
            static_assert(std::is_convertible<T, EnhancedValue>::value, "Type is not convertible to mysqlx::EnhancedValue.");
            return T(*this);
        }
    };

    /**
     * @brief      `SerializableRow` permits us to serialize Rows
     * while retaining the functionality from `mysqlx::Row`.
     */
    class SerializableRow : public Row
    {
    public:

        /**
         * @brief      Empty constructor.
         */
        SerializableRow()
            : Row()
        {}

        /**
         * @brief      Copies a Row to a SerializableRow
         *
         * @param[in]  row Row to be serialized.
         */
        SerializableRow (Row&& row)
        {
            for (int i = 0; i < row.colCount(); ++i)
                this->set(i, row.get(i));
        }

        /**
         * @brief      Serializes an object of type `T` to
         * this row. 
         * You must specialize `row_serializer` to `row_serializer<T>`
         * and implement its methods.
         * @param[in]  value  Object of type `T`.
         *
         * @tparam     T    Type of value to serialize.
         */
        template <class T>
        void from(const T& value)
        {
            SerializableRow row;
            row_serializer<T>::to_row(row, value);
            *this = row;
        }

        /**
         * @brief      Returns the serialized row as an object of type `T`.
         * You must specialize `row_serializer` to `row_serializer<T>`
         * and implement its methods.
         * @tparam     T     Type of object.
         *
         * @return     Serialized object.
         */
        template <class T>
        T as()
        {
            T value;
            row_serializer<T>::from_row(*this, value);
            return value;
        }

        /**
         * @brief      Gets an `EnhancedValue` from the row.
         *
         * @param[in]  pos   Column position of value.
         *
         * @return     Value in column.
         */
        EnhancedValue get(col_count_t pos)
        {
            return EnhancedValue(this->Row::get(pos));
        }
    };
}

// --- SERIALIZATION FOR SOME MYSQLX OBJECTS ------
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
#endif