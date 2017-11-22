#ifndef CELESTE_SERVICES_READING
#define CELESTE_SERVICES_READING

#include <memory>
#include <mysql_devapi.h>
#include <restbed>


namespace celeste
{
namespace resource
{
    void reading_dispatcher(const std::shared_ptr<restbed::Session> session,
                            const std::shared_ptr<mysqlx::Session> dbSession);

    std::shared_ptr<restbed::Resource> make_reading(const std::shared_ptr<mysqlx::Session> dbSession);
}
}
#endif