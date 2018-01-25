#include <cstdlib>
#include <soci/mysql/soci-mysql.h>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "DeviceStatus.hpp"
#include "srv/service/common.hpp"

using namespace std;
using namespace soci;

namespace celeste
{
namespace resource
{
    DeviceStatusService<nlohmann::json>::DeviceStatusService(const string& dbSettings, size_t max_connections)
    {
        set_paths({"/device/status", "/device/status/arduino"});
        set_method_handler("GET", [this] (const shared_ptr<restbed::Session> session) {GET(session);});
        // set_method_handler("POST",   [this] (const shared_ptr<restbed::Session> session) {POST(session);});
        
        for (int i = 0; i < max_connections; ++i)
            sqlPool.emplace(mysql, dbSettings);
    }


    DeviceStatus DeviceStatusService<nlohmann::json>::get_status(const string& deviceId)
    {
        auto sql = sqlPool.acquire_wait();
        std::string blob;
        *sql << "select status from DeviceStatus where Device_id = :DeviceId",
            soci::into(blob), soci::use(deviceId);
        json_type status = json_type::parse(blob);
        return static_cast<DeviceStatus>(status);
    }

    void DeviceStatusService<nlohmann::json>::GET(const shared_ptr<restbed::Session> session)
    {
        // get request
        const auto request = session->get_request();

        // get json from parameters
        json_type data = request->get_query_parameters();

        // validate data
        if (data["DeviceId"].is_null())
            throw status::MISSING_FIELD_DEVICEID;

        // get device from db
        json_type response;
        response = get_status(data["DeviceId"]);

        // close
        session->close(restbed::OK,
                       response.dump(),
                       {
                            { "Content-Length", to_string(response.dump().size()) },
                            { "Connection",     "close" }
                       });
    }
}
}

namespace nlohmann
{
    using namespace celeste::resource;

    void adl_serializer<DeviceStatus>::to_json(json& j, const DeviceStatus& obj)
    {
        j["DeviceId"] = obj.DeviceId;
        j = obj.status;
    }

    void adl_serializer<DeviceStatus>::from_json(const json& j, DeviceStatus& obj)
    {
        obj.DeviceId = j.at("DeviceId");
        obj.status = j.at("status");
    }
}