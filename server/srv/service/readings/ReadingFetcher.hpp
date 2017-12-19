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
#include <restbed>
#include <type_traits>

#include "srv/db/db.hpp"

#include "Reading.hpp"
#include "ReadRequest.hpp"

#include <iostream>

namespace celeste
{
namespace resource
{
    class ReadingFetcher
    {
    public:
        // --- TYPEDEFS --------------
        typedef mysqlx::EnhancedValue   atomic_type;
        typedef Reading<atomic_type>    value_type;

        // --- CONSTRUCTORS ----------
        ReadingFetcher() = default;

        // --- PUBLIC METHODS --------
        template <class Response, class Request>
        typename std::enable_if<
            std::is_arithmetic<Response>::value,
            Response
        >::type
        fetch(mysqlx::Session&& dbSession, const Request& req) const
        {   
            return fetch_impl<Response>(std::forward<mysqlx::Session>(dbSession), req);
        }

        template <class Response, class Request>
        typename std::enable_if<
            std::is_arithmetic<typename Response::value_type>::value,
            Response
        >::type
        fetch(mysqlx::Session&& dbSession, const Request& req) const
        {
            return fetch_impl<Response>(std::forward<mysqlx::Session>(dbSession), req);
        }


        template <class Response, class Request>
        typename std::enable_if<
            std::is_convertible<value_type, Response>::value,
            Response
        >::type
        fetch(mysqlx::Session&& dbSession, const Request& req) const
        {   
            return fetch_impl<value_type>(std::forward<mysqlx::Session>(dbSession), req);
        }

        template <class Response, class Request>
        typename std::enable_if<
            !std::is_convertible<value_type, Response>::value &&
            std::is_convertible<value_type, typename Response::value_type>::value,
            Response
        >::type
        fetch(mysqlx::Session&& dbSession, const Request& req) const
        {
            return fetch_impl<Response>(std::forward<mysqlx::Session>(dbSession), req);
        }

        template <class Response, class Request>
        typename std::enable_if<
            !std::is_convertible<value_type, Response>::value &&
            std::is_convertible<value_type, typename Response::mapped_type>::value,
            Response
        >::type
        fetch(mysqlx::Session&& dbSession, const Request& req) const
        {
            return fetch_impl<value_type>(std::forward<mysqlx::Session>(dbSession), req);
        }
    private:
        // --- PRIVATE METHODS -------
        template <class Response, class Request>
        Response fetch_impl(mysqlx::Session&& dbSession,
                            const Request& req) const;
    };


    // --- DEFAULT SPECIALIZATIONS ---
    template <>
    std::vector<ReadingFetcher::value_type>
    ReadingFetcher::fetch_impl(mysqlx::Session&& dbSession,
                               const RangeReadRequest& req)
    const;

    template <>
    ReadingFetcher::value_type
    ReadingFetcher::fetch_impl(mysqlx::Session&& dbSession,
                               const LastReadRequest& req)
    const;

    template <>
    std::vector<double>
    ReadingFetcher::fetch_impl(mysqlx::Session&& dbSession,
                               const AccumulatedReadRequest& req)
    const;
}
}

#endif
