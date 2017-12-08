#include "db.hpp"

using namespace std;

// --- SerializableRow DEFINITIONS --------------
namespace mysqlx
{
    SerializableRow::SerializableRow()
        : Row()
    {}

    SerializableRow::SerializableRow (Row&& row)
    {
        std::cout << static_cast<int>(*(row.get(1).getRawBytes().first)) << "\n";
        for (int i = 0; i < row.colCount(); ++i)
        {
            Value val = row.get(i);
            if (val.getType() != Value::RAW)
            {
                this->set(i, row.get(i));
            }
            else
            {
                auto other_bytes = val.getRawBytes();
                long diff = other_bytes.size();
                bytes new_bytes(std::get_temporary_buffer<mysqlx::byte>(diff));
                std::copy(other_bytes.begin(), other_bytes.end(), new_bytes.begin());
                this->set(i, new_bytes);
            }
        }
    }

    EnhancedValue SerializableRow::get(col_count_t pos)
    {
        return EnhancedValue(this->Row::get(pos));
    }

    void SerializableRow::set(col_count_t pos, EnhancedValue&& value)
    {
        this->Row::set(pos, std::move(value));
    }

    void SerializableRow::set(col_count_t pos, const EnhancedValue& value)
    {
        this->Row::set(pos, value);
    }
}

// --- EnhancedValue DEFINITIONS ----------------

namespace mysqlx
{
    EnhancedValue::EnhancedValue()
        : Value(nullptr)
    {}

    EnhancedValue::EnhancedValue (Value&& value)
        : Value (std::move(value))
    {}

    EnhancedValue::EnhancedValue (const Value& value)
        : Value (value)
    {}

    EnhancedValue::EnhancedValue (const boost::posix_time::ptime& value)
        : Value(boost::posix_time::to_iso_extended_string(value).c_str())
    {}

    EnhancedValue::EnhancedValue (const none_type& value)
        : Value (nullptr)
    {}

    EnhancedValue::EnhancedValue (const std::string& value)
        : Value (value.c_str())
    {}
}


// --- TRANSLATOR SPECIALIZATIONS ---------------
namespace mysqlx
{

}

// --- JSON SERIALIZATION -----------------------
namespace nlohmann {

    void adl_serializer<mysqlx::Value>::to_json(json& j, const mysqlx::Value& value)
    {   
        // if null just convert from nullptr and return
        if (value.isNull())
        {
            j = nullptr;
        }
        else
        {
            // get type information
            auto type = value.getType();
            using ValueType = mysqlx::Value::Type;

            // check which type is correct
            switch (type)
            {
                case ValueType::FLOAT:
                    j = value.get<float>();
                    break;
                case ValueType::DOUBLE:
                    j = value.get<double>();
                    break;
                case ValueType::INT64:
                    j = value.get<int>();
                    break;
                case ValueType::STRING:
                    j = static_cast<std::string>(value.get<mysqlx::string>());
                    break;
                case ValueType::BOOL:
                    j = value.get<bool>();
                    break;
                default:
                    throw std::invalid_argument("Critical Error while parsing from mysqlx::Value to JSON."
                                                "Unhandled data type.");
            }
        }
    }

    void adl_serializer<mysqlx::Value>::from_json(const json& j, mysqlx::Value& value)
    {
        auto type = j.type();
        using JType = json::value_t;
        switch(type)
        {
            case JType::number_float:
                value = j.get<float>();
                break;
            case JType::number_integer:
                value = j.get<int>();
                break; 
            case JType::boolean:
                value = j.get<bool>();
                break;
            case JType::string:
                value = j.get<std::string>().c_str();
                break;
            case JType::null:
                value = mysqlx::nullvalue;
                break;
            default:
                throw std::invalid_argument("Critical Error while parsing from JSON to mysqlx::Value."
                                            "Unhandled data type.");
        }
    }

    void adl_serializer<mysqlx::EnhancedValue>::to_json(json& j, const mysqlx::EnhancedValue& value)
    {   
        adl_serializer<mysqlx::Value>::to_json(j, value);
    }

    void adl_serializer<mysqlx::EnhancedValue>::from_json(const json& j, mysqlx::EnhancedValue& value)
    {
        adl_serializer<mysqlx::Value>::from_json(j, value);
    }

    /**
     * @brief      Converts a SqlResult to a JSON array.
     *
     * @param[out]      j     Out JSON object.
     * @param[in]       res   The SqlResult object
     */
    void adl_serializer<mysqlx::SqlResult>::to_json(json& j, mysqlx::SqlResult res)
    {
        std::vector<json> vec(res.count());
        int i = 0;
        for (const mysqlx::Row& row : res.fetchAll())
        {
            int j = 0;
            for (const mysqlx::Column& column : res.getColumns())
            {
                vec[i][column.getColumnName()] = row[j];
                j++;
            }
            i++;
        }
        j = vec;
    }

    /**
     * @brief      Converts a RowResult to a JSON array.
     *
     * @param[out]      j     Out JSON object.
     * @param[in]       res   The RowResult object
     */
    void adl_serializer<mysqlx::RowResult>::to_json(json& j, mysqlx::RowResult res)
    {
        std::vector<json> vec(res.count());
        int i = 0;
        for (const mysqlx::Row& row : res.fetchAll())
        {
            int j = 0;
            for (const mysqlx::Column& column : res.getColumns())
            {
                vec[i][column.getColumnName()] = row[j];
                j++;
            }
            i++;
        }
        j = vec; 
    }
}