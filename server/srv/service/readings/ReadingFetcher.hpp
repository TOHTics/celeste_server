/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 5/Dec/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */
#ifndef CELESTE_SERVICES_READING_FETCHER
#define CELESTE_SERVICES_READING_FETCHER

#include <memory>
#include <mysql_devapi.h>
#include <restbed>
#include <type_traits>
#include <boost/variant.hpp>

#include "ReadRequest.hpp"

namespace celeste
{
namespace resource
{
    class ReadingFetcher
    {
    public:
        // --- TYPEDEFS --------------
        typedef boost::variant<
            int,
            float,
            double,
            std::string
        > atomic_type;

        // --- CONSTRUCTORS ----------
        ReadingFetcher() = default;

        // --- PUBLIC METHODS --------
        // template <class Response, class Request>
        // Response fetch(mysqlx::Session&& dbSession, const Request& req) const
        // {
        //     return fetch<Response>(std::forward<mysqlx::Session>(dbSession), req);
        // }

        // template <class Response, class Request>
        // Response fetch(const mysqlx::Session& dbSession, const Request& req) const
        // {
        //     return fetch_impl<Response>(dbSession, req);
        // }

        template <class Response, class Request>
        typename std::enable_if<
            std::is_same<Response, atomic_type>::value,
            Response
        >::type
        fetch(const mysqlx::Session& dbSession, const Request& req) const
        {   
            return fetch_impl<atomic_type>(dbSession, req);
        }

        template <class Response, class Request>
        typename std::enable_if<
            !std::is_same<Response, atomic_type>::value &&
            std::is_convertible<Response, atomic_type>::value &&
            std::is_convertible<
                decltype(boost::get<Response>(std::declval<atomic_type>())),
                Response
            >::value,
            Response
        >::type
        fetch(const mysqlx::Session& dbSession, const Request& req) const
        {   
            return boost::get<Response>(fetch_impl<atomic_type>(dbSession, req));
        }

        template <class Response, class Request>
        typename std::enable_if<
            !std::is_convertible<Response, atomic_type>::value &&
            std::is_convertible<typename Response::value_type, atomic_type>::value,
            Response
        >::type
        fetch(const mysqlx::Session& dbSession, const Request& req) const
        {
            return fetch_impl<Response>(dbSession, req);
        }

        template <class Response, class Request>
        typename std::enable_if<
            !std::is_convertible<Response, atomic_type>::value &&
            std::is_convertible<typename Response::mapped_type, atomic_type>::value,
            Response
        >::type
        fetch(const mysqlx::Session& dbSession, const Request& req) const
        {
            return fetch_impl<Response>(dbSession, req);
        }
    private:
        // --- PRIVATE METHODS -------
        template <class Response, class Request>
        Response fetch_impl(const mysqlx::Session& dbSession,
                            const Request& req) const;
    };


    // --- DEFAULT SPECIALIZATIONS ---
    template <>
    std::vector<ReadingFetcher::atomic_type>
    ReadingFetcher::fetch_impl(const mysqlx::Session& dbSession,
                               const LastReadRequest& req)
    const;

    template <>
    ReadingFetcher::atomic_type
    ReadingFetcher::fetch_impl(const mysqlx::Session& dbSession,
                               const LastReadRequest& req)
    const;
}
}

#endif
