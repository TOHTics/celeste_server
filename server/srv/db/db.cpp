#include "db.hpp"

using namespace std;

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