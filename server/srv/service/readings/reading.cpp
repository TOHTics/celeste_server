#include <json.hpp>
#include <map>
#include "reading.hpp"
#include "readings.hpp"
#include "srv/service/common.hpp"
#include "srv/db/db.hpp"

using namespace std;
using json = nlohmann::json;

namespace celeste
{
namespace resource
{
    void reading_dispatcher(const shared_ptr<restbed::Session> session,
                            const shared_ptr<mysqlx::Session> dbSession)
    {   
        static map<string, const shared_ptr<Reading>> read = 
        {
            {"last", make_shared<LastReading>(dbSession)}
        };

        static string existing_methods = "last\n";

        auto request = session->get_request();
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        // Fetch json
        json data;
        session->fetch(content_length,
            [ request, &data ] (const shared_ptr<restbed::Session> session, const restbed::Bytes &bytes)
            {
                string body;
                bytes2string(bytes, body);
                data = json::parse(body);
            });

        // If no method is given, set last as default
        string method;
        if (data["method"].is_null())
        {
            method = "last";
        }
        else
        {
            method = data["method"];

            // Verify that the method passed exists
            if (read.find(method) == read.end())
            {
                handle_error(restbed::BAD_REQUEST,
                             "Missing field: " + quote("method") +
                             "Please specify a existing method from one of these:\n" + existing_methods,
                             session);
                return;
            }
        }

        // Get reading method which we shall use
        auto reading = read[method];

        for (const string& field : reading->required_fields())
        {
            if (data.find(field) == data.end())
            {
                handle_error(restbed::BAD_REQUEST,
                             "Missing field: " + quote(field),
                             session);
                return;
            }
        }

        string rb;
        try
        {
            json j = reading->get(data);
            rb = j.dump();
        } catch (const json::exception& e)
        {
            throw e;
        } catch (const mysqlx::Error& e)
        {
            handle_error(restbed::INTERNAL_SERVER_ERROR,
                         "There was an error while retrieving the readings. The information you sent us, however, was valid.",
                         session);
        }

        session->close(restbed::OK,
                               rb,
                               {
                                   { "Content-Length", to_string(rb.size()) },
                                   { "Connection",     "close" }
                               });
    }

    shared_ptr<restbed::Resource> make_reading(const shared_ptr<mysqlx::Session> dbSession)
    {
        auto resource = make_shared<restbed::Resource>();
        // resource->set_path("/device/{DeviceId: [0-9]+}/model/{ModelId: [a-zA-Z]+}/point/{PointId: [a-zA-Z]+}");
        resource->set_path("/reading");
        resource->set_method_handler("POST", bind(reading_dispatcher, placeholders::_1, dbSession));
        return move(resource);
    }

}
}