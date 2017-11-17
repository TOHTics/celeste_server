#ifndef CELESTE_RESOURCES_POINT_HPP
#define CELESTE_RESOURCES_POINT_HPP

#include <memory>
#include <restbed>
#include <mysql_devapi.h>
namespace celeste
{
namespace resource
{
    void get_point(const std::shared_ptr<restbed::Session> session,
               const std::shared_ptr<mysqlx::Session> dbSession);

    void save_point(const std::shared_ptr<restbed::Session> session,
                    const std::shared_ptr<mysqlx::Session> dbSession);

    void delete_point(const std::shared_ptr<restbed::Session> session,
                      const std::shared_ptr<mysqlx::Session> dbSession);

    std::shared_ptr<restbed::Resource> make_point(const std::shared_ptr<mysqlx::Session> dbSession);
}
}

#endif