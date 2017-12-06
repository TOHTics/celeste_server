/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 5/Dec/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 */

#include <vector>
#include <list>
#include <mysql_devapi.h>
#include <boost/variant.hpp>
#include "ReadingFetcher.hpp"
#include "ReadRequest.hpp"

using namespace std;

namespace celeste
{
namespace resource
{   
    typedef boost::variant<
        int,
        float,
        double,
        std::string
    > atomic_type;
    using atomic_type = ReadingFetcher::atomic_type;

    // template <>
    // vector<int> ReadingFetcher::fetch_impl(const mysqlx::Session& dbSession,
    //                                        const LastReadRequest& req) const
    // {
    //     return {1, 2, 3};
    // }

    template <>
    atomic_type ReadingFetcher::fetch_impl(const mysqlx::Session& dbSession,
                                           const LastReadRequest& req) const
    {
        return 99;
    }

    // template <>
    // int ReadingFetcher::fetch_impl(const mysqlx::Session& dbSession,
    //                                        const LastReadRequest& req) const
    // {
    //     return 2;
    // }
}
}