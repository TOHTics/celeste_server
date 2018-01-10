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
        ReadingFetcher(const celeste::SessionSettings& dbSettings);
        ReadingFetcher(const ReadingFetcher& other);
        ReadingFetcher(ReadingFetcher&& other) = delete;
        
        // --- OPERATORS -------------
        ReadingFetcher& operator=(const ReadingFetcher&) = delete;
        ReadingFetcher& operator=(ReadingFetcher&&) = delete;

        // --- PUBLIC METHODS --------
        template <class Response, class Request>
        typename std::enable_if<
            std::is_arithmetic<Response>::value,
            Response
        >::type
        fetch(Request&& req) const
        {   
            return fetch_impl<Response>(std::forward<Request>(req));
        }

        template <class Response, class Request>
        typename std::enable_if<
            std::is_arithmetic<typename Response::value_type>::value,
            Response
        >::type
        fetch(Request&& req) const
        {
            return fetch_impl<Response>(std::forward<Request>(req));
        }


        template <class Response, class Request>
        typename std::enable_if<
            std::is_convertible<value_type, Response>::value,
            Response
        >::type
        fetch(Request&& req) const
        {   
            return fetch_impl<value_type>(std::forward<Request>(req));
        }

        template <class Response, class Request>
        typename std::enable_if<
            !std::is_convertible<value_type, Response>::value &&
            std::is_convertible<value_type, typename Response::value_type>::value,
            Response
        >::type
        fetch(Request&& req) const
        {
            return fetch_impl<Response>(std::forward<Request>(req));
        }

        template <class Response, class Request>
        typename std::enable_if<
            !std::is_convertible<value_type, Response>::value &&
            std::is_convertible<value_type, typename Response::mapped_type>::value,
            Response
        >::type
        fetch(Request&& req) const
        {
            return fetch_impl<value_type>(std::forward<Request>(req));
        }
    private:
        // --- PRIVATE METHODS -------
        template <class Response, class Request>
        Response fetch_impl(Request&& req) const;

        // --- PRIVATE MEMBERS -------
        celeste::SessionSettings    dbSettings;
        mutable mysqlx::Session     dbSession;
    };


    // --- DEFAULT SPECIALIZATIONS ---
    template <>
    std::vector<ReadingFetcher::value_type>
    ReadingFetcher::fetch_impl(const RangeReadRequest& req)
    const;

    template <>
    ReadingFetcher::value_type
    ReadingFetcher::fetch_impl(const LastReadRequest& req)
    const;

    template <>
    std::vector<double>
    ReadingFetcher::fetch_impl(const AccumulatedReadRequest& req)
    const;
}
}

#endif
