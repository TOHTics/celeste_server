/**
 * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 24/08/17 Carlos Brito *
 * * * * * * * * * * * * * * * * * * * *
 * 
 * @file
 * @brief Contains the function declarations for obtaining a resource.
 *
 * # Description
 * Including this header file will grant access to the `resource::make_resource`
 * functions which return resources that a HTTP client could access.
 * Each of the returned resources has one associated handler which implies
 * that `N`  resource **instances** `Ri` have only one handler `H`. In a diagram:
 *
 * ````
 *     is handled by
 * Ri @--------------\
 * Ri @---------------\
 * .                   \
 * .                  [H]
 * .                   /
 * Ri @---------------/
 * Ri @--------------/
 * ````
 * Also each resource instance should have on unique associated path `P`.
 *
 * ````
 *     is located at
 * Ri @-------------- [P]
 * ````
 *
 * However, each resource `R` has one unique associated handler.
 * ````
 *    is handled uniquely by
 * R @----------------------- [H]
 * ````
 * # Notes
 * ## Why aren't the resources specialized classes?
 * A `Resource` is a class with different attributes. In general, we differentiate resources from one another
 * through use of their names, their handlers and attributes.
 *
 * For example, the `make_logger_upload()` method returns a `logger_upload` resource. This resource has a unique
 * associated handler `logger_upload_handler`. The resource (like all resources) can have many instances at different
 * paths and be published by any service (as long as the path doesn't collision with any resources already on the
 * service).
 *
 * To verify what constitutes a particular resource we recommend that one looks at the `.hpp` file documentation.
 *
 * ## How do I implement my own resource?
 * We follow these steps to create a resource:
 *
 * 1. Give the resource a name. E.g. `my_resource`
 * 2. Create `.hpp` and `.cpp` files in the directory `resource`
 * 3. Declare a function `make_my_resource` under the namespace `solarplant::srv:resource` in `my_resource.hpp` file
 * 4. In the `.cpp` file implement the handler and internals under the namespace `solarplant::srv::handler`
 * 5. Implement the handler and make methods
 * 6. Publish the resource in some service!
 *
 * Important notes:
 * - The handler should be `void` and take a `std::shared_ptr<restbed::Session>` as parameter.
 * - The make function should return a `std::shared_ptr<restbed::Resource>` and take a `std::string`
 * as a parameter for setting the path
 * - Check the resources already implemented to get an idea!
 *
 * TODO
 * Nothing for the moment.
 */
#ifndef SERVER_RESOURCE_HPP
#define SERVER_RESOURCE_HPP

#include "logger_upload.hpp"

#endif //SERVER_RESOURCE_HPP
