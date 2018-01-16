#ifndef CELESTE_SERVICE_STATUS_HPP
#define CELESTE_SERVICE_STATUS_HPP

#include <map>
#include <string>
#include <memory>
#include <restbed>

namespace celeste
{
namespace status
{
    // --- NOT FOUND ERRORS -------------------
    // 4000 - 4049
    static constexpr int DEVICE_NOT_FOUND            = 4000;
    static constexpr int MODEL_NOT_FOUND             = 4001;
    static constexpr int POINT_NOT_FOUND             = 4002;
    static constexpr int READING_NOT_FOUND           = 4003;
    static constexpr int DEVICE_MODEL_NOT_FOUND      = 4004;

    // --- MISC ERRORS ------------------------
    // 4050 - 4099
    static constexpr int UPLOAD_FAILED               = 4050;
    static constexpr int EMPTY_ARRAY                 = 4051;
    static constexpr int TYPE_MUST_BE_NUMERIC        = 4052;
    static constexpr int VALUE_IS_OUT_OF_RANGE       = 4053;
    static constexpr int TYPE_MUST_BE_ARRAY          = 4054;

    // --- SYNTAX ERRORS ----------------------
    // 4100 - 4199
    static constexpr int JSON_SYNTAX_ERROR           = 4100;
    static constexpr int XML_SYNTAX_ERROR            = 4101;
    static constexpr int CELESTERN_SYNTAX_ERROR      = 4102;

    // --- MISSING FIELD ERRORS ---------------
    
    // Device Related: 4200 - 4219
    static constexpr int MISSING_FIELD_DEVICEID      = 4200;
    static constexpr int MISSING_FIELD_MAN           = 4201;
    static constexpr int MISSING_FIELD_MOD           = 4202;
    static constexpr int MISSING_FIELD_SN            = 4203;
    static constexpr int MISSING_FIELD_IDX           = 4204;
    
    // Model Related: 4220 - 4239
    static constexpr int MISSING_FIELD_MODELID       = 4220;
    static constexpr int MISSING_FIELD_NS            = 4221;

    // Point Related: 4240 - 4259
    static constexpr int MISSING_FIELD_POINTID       = 4240;
    static constexpr int MISSING_FIELD_TYPE          = 4241;
    static constexpr int MISSING_FIELD_U             = 4242;
    static constexpr int MISSING_FIELD_D             = 4243;

    // Device has Model Related: 4260 - 4279
    static constexpr int MISSING_FIELD_NOTE          = 4260;


    // --- READING ERRORS ---------------------
    // 4300 - 5300
    
    // Missing fields: 4300 - 4499
    static constexpr int READING_MISSING_FIELD           = 4300;
    static constexpr int MISSING_FIELD_METHOD            = 4301;
    static constexpr int MISSING_FIELD_START             = 4302;
    static constexpr int MISSING_FIELD_END               = 4303;
    static constexpr int MISSING_FIELD_DEVICEIDS         = 4304;

    // Methods: 4500 - 4699
    static constexpr int READING_METHOD_NOT_FOUND        = 4301;
    static constexpr int READING_METHOD_NOT_SUPPORTED    = 4302;
    static constexpr int READING_METHOD_INVALID_ARGS     = 4303;

    // --- SPECIAL ERRORS ---------------------
    static constexpr int UNHANDLED_EXCEPTION         = 8000;

    static const std::map<int, std::string> STATUS_MAP
    {
        {DEVICE_NOT_FOUND,          "Device not found."},
        {MODEL_NOT_FOUND,           "Model not found."},
        {POINT_NOT_FOUND,           "Point not found."},
        {READING_NOT_FOUND,         "Reading not found."},

        {UPLOAD_FAILED,             "The upload of data has failed."},

        {JSON_SYNTAX_ERROR,         "JSON syntax error."},
        {XML_SYNTAX_ERROR,          "XML syntax error."},
        {CELESTERN_SYNTAX_ERROR,    "CelesteRN syntax error."},

        {MISSING_FIELD_DEVICEID,    "Missing field: DeviceId"},
        {MISSING_FIELD_MAN,         "Missing field: man"},
        {MISSING_FIELD_MOD,         "Missing field: mod"},
        {MISSING_FIELD_SN,          "Missing field: sn"},
        {MISSING_FIELD_MODELID,     "Missing field: ModelId"},
        {MISSING_FIELD_NS,          "Missing field: ns"},
        {MISSING_FIELD_POINTID,     "Missing field: PointId"},
        {MISSING_FIELD_TYPE,        "Missing field: type"},
        {MISSING_FIELD_U,           "Missing field: u"},
        {MISSING_FIELD_D,           "Missing field: d"},
        {MISSING_FIELD_NOTE,        "Missing field: note"},
        {MISSING_FIELD_IDX,         "Missing field: idx"},

        {READING_MISSING_FIELD,     "There's a missing field that the server requires."},
        {MISSING_FIELD_METHOD,      "Missing field: method"},
        {MISSING_FIELD_START,       "Missing field: start"},
        {MISSING_FIELD_END,         "Missing field: end"},
        {MISSING_FIELD_DEVICEIDS,   "Missing field: DeviceIds"},

        {READING_METHOD_NOT_FOUND,      "No such method exists for requesting a Reading."},
        {READING_METHOD_NOT_SUPPORTED,  "This method is not yet supported."},
        {READING_METHOD_INVALID_ARGS,   "Invalid arguments passed to the method."},

        {UNHANDLED_EXCEPTION,       "Unhandled Exception. Please open a ticket/issue at https://github.com/TOHTics/celeste_server/issues"},
    
        {EMPTY_ARRAY,               "Empty array value detected! For this particular case it is forbidden and, thus, you must specify +1 array elements."},
        {TYPE_MUST_BE_NUMERIC,      "The type of the readings must be numeric for this method to work."},
        {VALUE_IS_OUT_OF_RANGE,     "One of the passed values falls out of range."},
        {TYPE_MUST_BE_ARRAY,        "Field must be array of elements."},
    };
}
}
#endif