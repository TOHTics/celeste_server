#include <json.hpp>
#include "point.hpp"
#include "srv/db/db.hpp"
#include "srv/service/common.hpp"

using namespace std;
using json = nlohmann::json;

namespace celeste
{
namespace resource
{
    void get_point(const std::shared_ptr<restbed::Session> session,
                   const std::shared_ptr<mysqlx::Session> dbSession)
    {
        auto request = session->get_request();
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        session->fetch(content_length,
            [ request, dbSession ] (const shared_ptr<restbed::Session> session, const restbed::Bytes &bytes)
            {
                string body;
                bytes2string(bytes, body);

                json data = json::parse(body);

                auto sch = dbSession->getSchema("Celeste");
                auto table = sch.getTable("Point");

                if (data["PointId"].is_null())
                {
                    auto res = table.
                        select("*").
                        where("Model_id = :ModelId").
                        bind(ValueMap{{"ModelId", data["ModelId"].get<string>().c_str()}}).
                        execute();

                    string rb = json(move(res)).dump();
                    session->close(restbed::OK,
                               rb,
                               {
                                   { "Content-Length", to_string(rb.size()) },
                                   { "Connection",     "close" }
                               });
                } else
                {
                    try
                    {
                        auto res = table.
                            select("*").
                            where("Model_id = :ModelId AND id = :PointId").
                            bind(ValueMap{
                                {"ModelId", data["ModelId"].get<string>().c_str()},
                                {"PointId", data["PointId"].get<string>().c_str()}
                            }).
                            execute();

                        string rb = json(move(res)).dump();
                        session->close(restbed::OK,
                                   rb,
                                   {
                                       { "Content-Length", to_string(rb.size()) },
                                       { "Connection",     "close" }
                                   });
                    } catch (const mysqlx::Error& e)
                    {
                        handle_error(restbed::INTERNAL_SERVER_ERROR,
                                     "Could not get Point " + data["Point_id"].get<string>(),
                                     session);
                    }
                }
            });
    }

    void save_point(const std::shared_ptr<restbed::Session> session,
                    const std::shared_ptr<mysqlx::Session> dbSession)
    {
        auto request = session->get_request();
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        session->fetch(content_length,
            [ request, dbSession ] (const shared_ptr<restbed::Session> session, const restbed::Bytes &bytes)
            {
                string body;
                bytes2string(bytes, body);

                json data = json::parse(body);

                auto sch = dbSession->getSchema("Celeste");
                auto table = sch.getTable("Point");

                try
                {
                    table.
                    insert("id", "Model_id", "type", "u", "d").
                    values(data["PointId"].get<string>().c_str(),
                           data["ModelId"].get<string>().c_str(),
                           data["type"].get<int>(),
                           data["u"].get<string>().c_str(),
                           data["d"].get<string>().c_str()
                           ).
                    execute();

                    session->close(restbed::OK);
                } catch (const mysqlx::Error& e)
                {
                    handle_error(restbed::INTERNAL_SERVER_ERROR,
                                 "Could not add Point " + data["id"].get<string>() + " on Model " + data["Model_id"].get<string>(),
                                 session);
                }
            });
    }

    void delete_point(const std::shared_ptr<restbed::Session> session,
                      const std::shared_ptr<mysqlx::Session> dbSession)
    {
        auto request = session->get_request();
        size_t content_length = (size_t) request->get_header("Content-Length", 0);

        auto sch = dbSession->getSchema("Celeste");
        auto table = sch.getTable("Point");

        json data;
        session->fetch(content_length,
        [ request, dbSession, &data] (const shared_ptr<restbed::Session> session, const restbed::Bytes &bytes)
        {
            // Get body
            string body;
            bytes2string(bytes, body);

            data = json::parse(body);
        });

        string point_id = data["PointId"];
        string model_id = data["ModelId"];

        try
        {
            table.
            remove().
            where("Point_id = :PointId AND Model_id = :ModelId").
            bind(ValueMap{
                {"PointId", point_id.c_str()},
                {"ModelId", model_id.c_str()}
            }).
            execute();

            session->close(restbed::OK);
        } catch (const mysqlx::Error& e)
        {
            handle_error(restbed::INTERNAL_SERVER_ERROR,
                         "Could not delete Point " + point_id + " on Model " + model_id,
                         session);
        }
    }

    std::shared_ptr<restbed::Resource> make_point(const std::shared_ptr<mysqlx::Session> dbSession)
    {
        auto resource = make_shared<restbed::Resource>();
        resource->set_paths({
            "/points"
        });

        resource->set_method_handler("GET", bind(get_point, placeholders::_1, dbSession));
        resource->set_method_handler("POST", bind(save_point, placeholders::_1, dbSession));
        resource->set_method_handler("DELETE", bind(delete_point, placeholders::_1, dbSession));

        return move(resource);
    }
}
}