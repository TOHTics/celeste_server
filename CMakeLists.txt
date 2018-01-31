# ---- Set cmake minimum version ---------------
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
cmake_policy(SET CMP0048 NEW)

# ---- Set project details ---------------------
project(celeste_server VERSION 0.0.1 LANGUAGES CXX)

# ---- Build options ---------------------------
# option(OPTION     "Description"    ON )

# ---- Configuration ---------------------------
include("${PROJECT_SOURCE_DIR}/cmake/configuration.cmake")

# ---- Set the module path ---------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules")

# ---- Print copyright message -----------------
message("${blue} ------------------------------------------------------- ${reset}")
message("${blue} --- Copyright (C) TOH Tics 2017 All rights reserved --- ${reset}")
message("${blue} ------------------------------------------------------- ${reset}")

# ---- Display info ----------------------------
message("${gray}celeste_server v${PROJECT_VERSION} ${reset}")

message("${gray}This project uses the following dependencies:")
message("@ restbed")
message("@ MySQL C++ connector v8")
message("@ Boost property_tree")
message("@ nhlomann JSON")
message("@ object_pool")
message("${reset}")

# ---- Configure compiler ----------------------
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wextra")

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# ---- Server ---------------------------------
file(GLOB_RECURSE SERVER_SRC
    "src/*.hpp"
    "src/*.h"
    "src/*.cpp"
    "src/*.cc"
    )
set(server ${SERVER_SRC})

# ---- Mix all source files --------------------
set(SERVER_SOURCE_FILES 
        src/main.cpp
        ${sunspec}
        ${server}
    )

# ---- Find packages ---------------------------
find_package(celeste_rn     REQUIRED)
find_package(restbed        REQUIRED)
find_package(mysql          REQUIRED)
find_package(soci           REQUIRED)
find_package(json           REQUIRED)
find_package(object_pool    REQUIRED)

find_package(Boost 1.55          REQUIRED
             COMPONENTS
             date_time
             program_options)

include_directories("src/")
include_directories(".")

include_directories(${celeste_rn_INCLUDE})
include_directories(${mysql_INCLUDE})
include_directories(${soci_INCLUDE})
include_directories(${soci_CONFIG_INCLUDE})
include_directories(${restbed_INCLUDE})
include_directories(${json_INCLUDE})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${object_pool_INCLUDE})
include_directories(${soci_INCLUDE})

# ---- Build -----------------------------------
message("${blue}Generating makefiles...${reset}")

# ---- Add an executable -----------------------
add_executable(celeste_server ${SERVER_SOURCE_FILES})

# ---- Link executable -------------------------
if (NOT restbed_LIBRARY)
    add_subdirectory("dependency/restbed/")
    target_link_libraries(celeste_server restbed)
else()
    target_link_libraries(celeste_server ${restbed_LIBRARY})
endif()

if (NOT celeste_rn_LIBRARY)
    add_subdirectory("dependency/celeste_rn/")
    target_link_libraries(celeste_server celeste_rn)
else()
    target_link_libraries(celeste_server ${celeste_rn_LIBRARY})
endif()

if (NOT soci_LIBRARY)
    add_subdirectory("dependency/soci/")
    set(SOCI_TESTS OFF)
    target_link_libraries(celeste_server soci_core )
    target_link_libraries(celeste_server soci_mysql )
else()
    target_link_libraries(celeste_server ${soci_LIBRARY})
    target_link_libraries(celeste_server ${soci_mysql_LIBRARY})
endif()

target_link_libraries(celeste_server ${Boost_LIBRARIES})

configure_file("${CMAKE_SOURCE_DIR}/logger-conf.conf" "${CMAKE_BINARY_DIR}/logger-conf.conf" COPYONLY)
