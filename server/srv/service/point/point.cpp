#include <json.hpp>
#include <iostream>
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

                if (data["Point_id"] == nullptr)
                {
                    auto res = table.
                        select("*").
                        where("Model_id = :ModelId").
                        bind(ValueMap{{"ModelId", data["Model_id"].get<string>().c_str()}}).
                        execute();

                    json rb = move(res);
                    session->close(restbed::OK, rb.dump());
                } else
                {
                    auto res = table.
                        select("*").
                        where("Model_id = :ModelId AND id = :PointId").
                        bind(ValueMap{
                            {"ModelId", data["Model_id"].get<string>().c_str()},
                            {"PointId", data["Point_id"].get<string>().c_str()}
                        }).execute();

                    string rb = json(move(res));
                    session->close(restbed::OK,
                                   rb,
                                   {
                                       { "Content-Length", to_string(rb.size()) },
                                       { "Connection",     "close" }
                                   });
                }
            });
    }

    void save_point(const std::shared_ptr<restbed::Session> session,
                    const std::shared_ptr<mysqlx::Session> dbSession)
    {

    }

    void delete_point(const std::shared_ptr<restbed::Session> session,
                      const std::shared_ptr<mysqlx::Session> dbSession)
    {

    }

    std::shared_ptr<restbed::Resource> make_point(const std::shared_ptr<mysqlx::Session> dbSession)
    {
        auto resource = make_shared<restbed::Resource>();
        resource->set_path("/points");

        resource->set_method_handler("GET", bind(get_point, placeholders::_1, dbSession));
        resource->set_method_handler("POST", bind(save_point, placeholders::_1, dbSession));
        resource->set_method_handler("DELETE", bind(delete_point, placeholders::_1, dbSession));

        return move(resource);
    }
}
}