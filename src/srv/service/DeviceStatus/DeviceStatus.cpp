#include <cstdlib>
#include <soci/mysql/soci-mysql.h>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "srv/error.hpp"
#include "srv/service/common.hpp"

#include "DeviceStatus.hpp"


using namespace std;
using namespace soci;

namespace celeste
{
namespace resource
{
    DeviceStatusService<nlohmann::json>::DeviceStatusService(const string& dbSettings)
        : m_dbSettings(dbSettings)
    {
        set_paths({"/device/status"});
        set_method_handler("GET", [this] (const shared_ptr<restbed::Session> session) {GET(session);});
        set_method_handler("PUT", [this] (const shared_ptr<restbed::Session> session) {PUT(session);});
    }


    DeviceStatus DeviceStatusService<nlohmann::json>::get_status(const string& deviceId)
    {
        session sql(mysql, m_dbSettings);
        indicator ind;
        std::string blob;
        sql << "select status from DeviceStatus where Device_id = :DeviceId",
            soci::into(blob, ind), soci::use(deviceId);
        
        if (ind != i_ok)
            throw std::runtime_error("There is no status posted for Device: " + deviceId +
                                     " or the document stored is too long.");

        json_type status;
        try
        {
            status = json_type::parse(blob);
        } catch (std::exception& e)
        {
            throw std::runtime_error("Malformed status in the DB.");
        }
        return DeviceStatus{.DeviceId = deviceId, .status = std::move(status)};
    }

    void DeviceStatusService<nlohmann::json>::update_status(const std::string& deviceId, const json_type& new_status)
    {
        session sql(mysql, m_dbSettings);
        statement stmt = (sql.prepare
                          << "insert into DeviceStatus (Device_id, status) values(:DeviceId, :status) "
                          << "on duplicate key "
                          << "update status=:status",
                          use(deviceId, "DeviceId"), use(std::move(new_status.dump()), "status")
                          );
        stmt.execute();
    }

    void DeviceStatusService<nlohmann::json>::PUT(const std::shared_ptr<restbed::Session> session)
    {
        // get request
        const auto request = session->get_request();

        // get headers
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        // fetch data to access later
        session->fetch(content_length, [] (const shared_ptr<restbed::Session> session, const restbed::Bytes &bytes) {});

        // get json from request
        json_type data = get_json<json_type>(*request);

        if (data["DeviceId"].is_null())
            throw MissingFieldError("DeviceId");

        if (data["DeviceId"].is_null())
            throw runtime_error("status field cannot be null");

        try
        {
            update_status(data["DeviceId"], data["status"]);
            session->close(restbed::OK);
        }
        catch (mysql_soci_error& e)
        {
            throw DatabaseError("Could not update Device status.");
        }
    }

    void DeviceStatusService<nlohmann::json>::GET(const shared_ptr<restbed::Session> session)
    {
        // get request
        const auto request = session->get_request();

        // get json from parameters
        json_type data = request->get_query_parameters();

        // validate data
        if (data["DeviceId"].is_null())
            throw MissingFieldError("DeviceId");

        try
        {
            // get device from db
            json_type response;
            response = get_status(data["DeviceId"]);
            string body = response.dump();

            // close
            session->close(restbed::OK,
                           body,
                           {
                                { "Content-Length", to_string(body.size()) },
                                { "Connection",     "close" }
                           });
        }
        catch (mysql_soci_error& e)
        {
            throw DatabaseError("Could not fetch Device status");
        }
    }
}
}

namespace nlohmann
{
    using namespace celeste::resource;

    void adl_serializer<DeviceStatus>::to_json(json& j, const DeviceStatus& obj)
    {
        j = obj.status;
    }

    void adl_serializer<DeviceStatus>::from_json(const json& j, DeviceStatus& obj)
    {
        obj.status = j.at("status");
    }
}