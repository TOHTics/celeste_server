#include "db.hpp"

using namespace std;

namespace celeste
{
    SessionSettings::operator mysqlx::SessionSettings() const
    {
        return mysqlx::SessionSettings(host.c_str(), port, user.c_str(), pwd.c_str(), db.c_str());
    }
}


// --- SerializableRow DEFINITIONS --------------
namespace mysqlx
{
    SerializableRow::SerializableRow()
        : Row()
    {}

    // SerializableRow::SerializableRow(const Row& row)
    // {
    //     for (int i = 0; i < row.colCount(); ++i)
    //     {
    //         auto value = const_cast<Row&>(row).get(i);
    //         if (value.getType() == Value::RAW)
    //             this->set(i, value);
    //     }
    // }

    SerializableRow::SerializableRow (Row&& row)
    {
        for (int i = 0; i < row.colCount(); ++i)
        {
            auto value = row.get(i);
            if (value.getType() == Value::RAW)
                this->set(i, std::move(value));
            else
                this->set(i, value);
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


    EnhancedValue::EnhancedValue (EnhancedValue&& value)
    {
        if (value.getType() ==  Value::RAW)
            this->m_raw = std::move(value.m_raw);
        else
            *this = std::move(value);
    }

    EnhancedValue::EnhancedValue (const Value& value)
        : Value (value) 
    {
        if (value.getType() ==  Value::RAW)
        {
            // copy bytes
            bytes old_bytes = value.getRawBytes();
            bytes new_bytes(std::get_temporary_buffer<byte>(old_bytes.size()));
            std::copy(old_bytes.begin(), old_bytes.end(), new_bytes.begin());
            std::swap(this->m_raw, new_bytes);
        }
    }

    EnhancedValue::EnhancedValue (const EnhancedValue& value)
    {
        *this = EnhancedValue(static_cast<Value>(value));
    }

}


// --- TRANSLATOR SPECIALIZATIONS ---------------
// We use the fact that an `EnhancedValue` is constructible
// from the base class `Value`. For types such as `ptime`
// we rely on the fact that the data needed to convert to
// the type is stored as `mysqlx::bytes`. Even more, to convert
// from, for example, `ptime` we store it as `mysqlx::bytes`.
namespace mysqlx
{
    // --- STRING TRANSLATION --------
    std::string value_translator<std::string>::get(const Value& in)
    {
        assert(in.getType() == Value::STRING);
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.to_bytes(in.get<mysqlx::string>());
    }

    Value value_translator<std::string>::put(const std::string& in)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;    
        return static_cast<Value>(converter.from_bytes(in));
    }
    
    // --- CONST CHAR* TRANSLATION
    const char* value_translator<const char*>::get(const Value& in)
    {
        assert(in.getType() == Value::STRING);
        return value_translator<std::string>::get(in).c_str();
    }

    Value value_translator<const char*>::put(const char* in)
    {
        return static_cast<Value>(in);
    }

    // --- NONE TRANSLATION ----------
    boost::none_t value_translator<boost::none_t>::get(const Value& in)
    {
        assert(in.getType() == Value::VNULL);
        return boost::none;
    }

    Value value_translator<boost::none_t>::put(const boost::none_t& in)
    {
        return static_cast<Value>(nullptr);
    }

    // --- POSIX TIME TRANSLATION ----
    boost::posix_time::ptime value_translator<boost::posix_time::ptime>::get(const Value& in)
    {
        assert(in.getType() == Value::RAW);
        /*
         * The following calculation of the year was derived
         * based on how mysqlx stores timestamps from the DB.
         * Basically, you get a response consisting of at most
         * 7 bytes but at least 4 bytes. The structure of the bytes
         * are desglossed below:
         * 
         * [x] [y] [month] [day] [hours] [minutes] [seconds]
         * 
         * If SS contains the value 0, you get 6 bytes instead of 7.
         * If SS and MM contain the value 0, you get 5 bytes instead of 7.
         * If SS, MM and HH contain the value 0, you get 4 bytes of 7.
        */
        using date = boost::gregorian::date;
        using namespace boost::posix_time;
        auto _bytes = static_cast<Value>(in).get<mysqlx::bytes>();

        int x = *(_bytes.begin());
        int y = *(_bytes.begin() + 1);
        int year = 128 * (y - 1) + x;
        int month = *(_bytes.begin() + 2);
        int day = *(_bytes.begin() + 3);

        int _hours = 0;
        int _minutes = 0;
        int _seconds = 0;

        if (_bytes.size() >= 4)
        {
            _hours = *(_bytes.begin() + 4);
            if (_bytes.size() >= 5)
            {
                _minutes = *(_bytes.begin() + 5);

                if (_bytes.size() >= 6)
                {
                    _seconds = *(_bytes.begin() + 6);
                }
            }
        }
        return ptime(date(year, month, day), hours(_hours) + minutes(_minutes) + seconds(_seconds));
    }

    Value value_translator<boost::posix_time::ptime>::put(const boost::posix_time::ptime& in)
    {   
        using date = boost::gregorian::date;
        using namespace boost::posix_time;

        long bytes_needed = 7;  // see value_translator::get() for ptime
        mysqlx::bytes byte_buffer(std::get_temporary_buffer<mysqlx::byte>(bytes_needed));
        mysqlx::byte *byte_array = byte_buffer.begin();

        date d = in.date();

        byte_array[0] = d.year() % 256;
        byte_array[1] = int(d.year() / 128);
        byte_array[2] = d.month();
        byte_array[3] = d.day_number();
        byte_array[4] = in.time_of_day().hours();
        byte_array[5] = in.time_of_day().minutes();
        byte_array[6] = in.time_of_day().seconds();
        return static_cast<Value>(byte_buffer);
    }

    // --- SELF TRANSLATION ----------
    Value value_translator<Value>::get(const Value& in)
    {
        return in;
    }

    Value value_translator<Value>::put(const Value& in)
    {
        return in;
    }

    // --- INT TRANSLATION -----------
    int value_translator<int>::get(const Value& in)
    {
        assert(in.getType() == Value::INT64);
        return in.get<int>();
    }

    Value value_translator<int>::put(int in)
    {
        return static_cast<Value>(in);
    }

    // --- FLOAT TRANSLATION ---------
    float value_translator<float>::get(const Value& in)
    {
        assert(in.getType() == Value::FLOAT);
        return in.get<float>();
    }

    Value value_translator<float>::put(float in)
    {
        return static_cast<Value>(in);
    }

    // --- DOUBLE TRANSLATION --------
    double value_translator<double>::get(const Value& in)
    {
        assert(in.getType() == Value::DOUBLE);
        return in.get<double>();
    }

    Value value_translator<double>::put(double in)
    {
        return static_cast<Value>(in);
    }

    // --- BOOL TRANSLATION ----------
    bool value_translator<bool>::get(const Value& in)
    {
        assert(in.getType() == Value::BOOL);
        return in.get<bool>();
    }

    Value value_translator<bool>::put(bool in)
    {
        return static_cast<Value>(in);
    }

    // --- BYTES TRANSLATION ---------
    bytes value_translator<bytes>::get(const Value& in)
    {
        assert(in.getType() == Value::RAW);
        bytes old_bytes = in.getRawBytes(); // get old bytes
        bytes new_bytes(std::get_temporary_buffer<byte>(old_bytes.size())); // get new bytes 
        std::copy(old_bytes.begin(), old_bytes.end(), new_bytes.begin());
        return new_bytes;
    }

    Value value_translator<bytes>::put(const bytes& in)
    {
        bytes old_bytes = in;
        bytes new_bytes(std::get_temporary_buffer<byte>(old_bytes.size())); // get new bytes 
        std::copy(old_bytes.begin(), old_bytes.end(), new_bytes.begin());
        return static_cast<Value>(new_bytes);
    }
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