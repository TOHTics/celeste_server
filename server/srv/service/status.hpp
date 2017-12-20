#ifndef CELESTE_SERVICE_STATUS_HPP
#define CELESTE_SERVICE_STATUS_HPP

#include <map>
#include <string>

namespace celeste
{
namespace status
{
    enum : int
    {
        // --- NOT FOUND ERRORS -------------------
        // 4000 - 4049
        DEVICE_NOT_FOUND            = 4000,
        MODEL_NOT_FOUND             = 4001,
        POINT_NOT_FOUND             = 4002,
        READING_NOT_FOUND           = 4003,

        // --- MISC ERRORS ------------------------
        // 4050 - 4099
        UPLOAD_FAILED               = 4050,
        EMPTY_ARRAY                 = 4051,

        // --- SYNTAX ERRORS ----------------------
        // 4100 - 4199
        JSON_SYNTAX_ERROR           = 4100,
        XML_SYNTAX_ERROR            = 4101,
        CELESTERN_SYNTAX_ERROR      = 4102,

        // --- MISSING FIELD ERRORS ---------------
        
        // Device Related: 4200 - 4219
        MISSING_FIELD_DEVICEID      = 4200,
        MISSING_FIELD_MAN           = 4201,
        MISSING_FIELD_MOD           = 4202,
        MISSING_FIELD_SN            = 4203,
        MISSING_FIELD_IDX           = 4204,
        
        // Model Related: 4220 - 4239
        MISSING_FIELD_MODELID       = 4220,
        MISSING_FIELD_NS            = 4221,

        // Point Related: 4240 - 4259
        MISSING_FIELD_POINTID       = 4240,
        MISSING_FIELD_TYPE          = 4241,
        MISSING_FIELD_U             = 4242,
        MISSING_FIELD_D             = 4243,

        // Device has Model Related: 4260 - 4279
        MISSING_FIELD_NOTE          = 4260,


        // --- READING ERRORS ---------------------
        // 4300 - 5300
        
        // Missing fields: 4300 - 4499
        READING_MISSING_FIELD                   = 4300,
        MISSING_FIELD_METHOD                    = 4301,
        MISSING_FIELD_START                     = 4302,
        MISSING_FIELD_END                       = 4303,
        MISSING_FIELD_DEVICEIDS                 = 4304,

        // Methods: 4500 - 4699
        READING_METHOD_NOT_FOUND        = 4301,
        READING_METHOD_NOT_SUPPORTED    = 4302,
        READING_METHOD_INVALID_ARGS     = 4303,

        // --- SPECIAL ERRORS ---------------------
        UNHANDLED_EXCEPTION         = 8000
    };

    static std::map<int, std::string> STATUS_MAP
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

        {READING_MISSING_FIELD,     "There's a missing field that the server requires see: https://github.com/carlosb/celeste/blob/master/doc/API.md#reading"},
        {MISSING_FIELD_METHOD,      "Missing field: method"},
        {MISSING_FIELD_START,       "Missing field: start"},
        {MISSING_FIELD_END,         "Missing field: end"},
        {MISSING_FIELD_DEVICEIDS,   "Missing field: DeviceIds"},

        {READING_METHOD_NOT_FOUND,      "No such method exists for requesting a Reading. See https://github.com/carlosb/celeste/blob/master/doc/API.md#reading for a list of supported methods."},
        {READING_METHOD_NOT_SUPPORTED,  "This method is not yet supported. See https://github.com/carlosb/celeste/issues in order to open ticket regarding the availability of this method."},
        {READING_METHOD_INVALID_ARGS,   "Invalid arguments passed to the method. See https://github.com/carlosb/celeste/blob/master/doc/API.md#reading for how to use this method."},

        {UNHANDLED_EXCEPTION,       "Unhandled Exception. Please open a ticket/issue at https://github.com/carlosb/celeste/issues describing the request you made and I will personally answer back with a solution."},
    
        {EMPTY_ARRAY,               "Empty array value detected! For this particular case it is forbidden and, thus, you must specify +1 array elements."}
    };
}
}
#endif