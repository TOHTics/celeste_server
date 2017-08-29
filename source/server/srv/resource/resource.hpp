//<editor-fold desc="Preamble">
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 8/29/17 Carlos Brito
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
//</editor-fold>

//<editor-fold desc="Description">
/**
 * @file resource.hpp
 * @author Carlos Brito (carlos.brito524@gmail.com)
 * @date 8/24/17.
 *
 * @brief Contains the function declarations for obtaining a resource.
 *
 * # Description
 * Including this header file will grant access to the `resource::make_$$`
 * functions which return resources that a HTTP client could access.
 * Each of the returned resources has one associated handler which implies
 * that `N` instances of resource `R` have only one handler `H`. In a diagram:
 *
 * ````
 *    is handled by
 * R @--------------\
 * R @---------------\
 * .                  \
 * .                  [H]
 * .                  /
 * R @---------------/
 * R @--------------/
 * ````
 *
 * TODO
 * ----
 * Nothing for the moment.
 */
//</editor-fold>
#ifndef SERVER_RESOURCE_HPP
#define SERVER_RESOURCE_HPP

#endif //SERVER_RESOURCE_HPP
