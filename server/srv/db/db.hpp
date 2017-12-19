#ifndef SERVER_DB_DB_HPP
#define SERVER_DB_DB_HPP

#include <memory>
#include <restbed>
#include <json.hpp>
#include <mysql_devapi.h>
#include <boost/optional.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <type_traits>
#include <locale>
#include <codecvt>


namespace mysqlx
{

    class EnhancedValue;

    /**
     * @brief      Translates between mysqlx::Value and some other
     * External type.
     *
     * @tparam     External  Type that we want to convert to.
     */
    template <class External> 
    struct value_translator 
    {
        typedef Value       internal_type;
        typedef External    external_type;

        /**
         * @brief      Translates the input value `in` to the External
         * data type.
         *
         * @param[in]  in    Input value.
         *
         * @return      Translated value.
         */
        static
        External get(const Value& in);

        /**
         * @brief      Translates the input value `in` to the data type
         * `mysqlx::Value`.
         *
         * @param[in]  in    Input value.
         *
         * @return     Translated value.
         */
        static
        Value put(const External& in);
    };

    // --- DEFAULT TRANSLATIONS ---------------------
    template <> 
    struct value_translator<std::string>
    {
        static
        std::string get(const Value& in);

        static
        Value put(const std::string& in);
    };

    template <class T> 
    struct value_translator<boost::optional<T>>
    {
        static
        boost::optional<T> get(const Value& in);

        static
        Value put(const boost::optional<T>& in);
    };

    template <> 
    struct value_translator<boost::none_t>
    {
        static
        boost::none_t get(const Value& in);

        static
        Value put(const boost::none_t& in);
    };

    template <> 
    struct value_translator<boost::posix_time::ptime>
    {
        static
        boost::posix_time::ptime get(const Value& in);

        static
        Value put(const boost::posix_time::ptime& in);
    };

    template <> 
    struct value_translator<Value>
    {
        static
        Value get(const Value& in);

        static
        Value put(const Value& in);
    };

    template <> 
    struct value_translator<int>
    {
        static
        int get(const Value& in);

        static
        Value put(int in);
    };

    template <> 
    struct value_translator<double>
    {
        static
        double get(const Value& in);

        static
        Value put(double in);
    };

    template <> 
    struct value_translator<float>
    {
        static
        float get(const Value& in);

        static
        Value put(float in);
    };

    template <> 
    struct value_translator<bool>
    {
        static
        bool get(const Value& in);

        static
        Value put(bool in);
    };

    template <> 
    struct value_translator<bytes>
    {
        static
        bytes get(const Value& in);

        static
        Value put(const bytes& in);
    };

    template <> 
    struct value_translator<const char*>
    {
        static
        const char* get(const Value& in);

        static
        Value put(const char*);
    };
}

// --- VALUE AND ROW CLASSES --------------------
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
        static 
        void to_row (SerializableRow& row, const T& value);

        static 
        void from_row(const SerializableRow& row, T& value);
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
    public:
        /**
         * @brief      Constructs a null value.
         */
        EnhancedValue();

        /**
         * @brief      Copy constructor for `mysqlx::Value`.
         * This is more of a compatibility constructor to ensure
         * we can copy content from `Value`.
         *
         * @param[in]  value  Value.
         */
        EnhancedValue (EnhancedValue&& value);

        /**
         * @brief      Copy constructor for base class.
         *
         * @param[in]  value  Base class instance.
         */
        EnhancedValue (const Value& value);

        /**
         * @brief      Copy constructor.
         *
         * @param[in]  value  The value
         */
        EnhancedValue (const EnhancedValue& value);

        /**
         * @brief      Copy assignment.
         *
         * @param[in]  value  The value
         *
         * @return     A reference to the value.
         */
        EnhancedValue& operator=(const EnhancedValue& value) = default;

        /**
         * @brief      Copy constructor for any conversion.
         *
         * @param[in]  value  Value of type `T`. 
         *
         * @tparam     T      Must be convertible to `mysqlx::Value`.
         */
        template <class T>
        EnhancedValue (const T& value)
            : Value(value_translator<T>::put(value))
        {}

        /**
         * @brief      Performs an implicit conversion (if available) to
         * type T.
         *
         * @tparam     T     { description }
         */
        template <class T>
        operator T()
        {
            return value_translator<T>::get(*this);
        }

        /**
         * @brief      Gets the value by converting it to type T.
         *
         * @tparam     T     Type contained in value.
         *
         * @return     Value of type `T`.
         */
        template <class T>
        T get()
        {   
            return value_translator<T>::get(*this);
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
        SerializableRow();

        // SerializableRow(const Row& row);

        /**
         * @brief      Copies a Row to a SerializableRow
         *
         * @param[in]  row Row to be serialized.
         */
        SerializableRow (Row&& row);

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
        EnhancedValue get(col_count_t pos);

        /**
         * @brief      Sets the value of the row at `pos` using
         * an EnhancedValue.
         *
         * @param[in]  pos        The position
         * @param[in]  value  The value
         */
        void set(col_count_t pos, EnhancedValue&& value);

        /**
         * @brief      Sets the value of the row at `pos` using
         * an EnhancedValue.
         *
         * @param[in]  pos    The position
         * @param[in]  value  The value
         */
        void set(col_count_t pos, const EnhancedValue& value);
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
    struct adl_serializer<mysqlx::EnhancedValue>
    {
        static void to_json(json& j, const mysqlx::EnhancedValue& value);
        static void from_json(const json& j, mysqlx::EnhancedValue& value);
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

#include "translator_impl.hpp"
#endif