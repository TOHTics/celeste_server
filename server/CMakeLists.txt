# ---- Set cmake minimum version ---------------
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
cmake_policy(SET CMP0048 NEW)

project(celeste_server VERSION 0.0.1 LANGUAGES CXX)
message("${gray}celeste_server v${PROJECT_VERSION} ${reset}")

# ---- Set the module path ---------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../cmake/modules")

include_directories(".")

# ---- Configure compiler ----------------------
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Sunspec ---------------------------------
file(GLOB_RECURSE SUNSPEC_SRC
    "sunspec/*.hpp"
    "sunspec/*.cpp"
    )
set(sunspec ${SUNSPEC_SRC})

# ---- Server ----------------------------------
file(GLOB_RECURSE SRV_SRC
    "srv/*.hpp"
    "srv/*.cpp"
    )
set(srv ${SRV_SRC})

# ---- Tests -----------------------------------
file(GLOB_RECURSE TEST_SRC
    "test/*.hpp"
    "test/*.cpp"
    )
set(tests ${TEST_SRC})

# ---- Mix all source files --------------------
set(SERVER_SOURCE_FILES 
        main.cpp
        ${sunspec}
        ${srv}
        ${tests}
    )

# ---- Find packages ---------------------------
# find_package(restbed        REQUIRED)
    add_subdirectory("dependency/restbed")
    add_subdirectory("dependency/mysql-connector-cpp/")
# endif()

# find_package(mysqlcppconn   REQUIRED)

# find_library(restbedlib librestbed.a) 
# find_library(MySQL libmysqlcppconn8.dylib)

# ---- Add an executable -----------------------
add_executable(celeste_server ${SERVER_SOURCE_FILES})

# ---- Link executable -------------------------`
target_link_libraries(celeste_server restbed)
target_link_libraries(celeste_server libconcpp )

# Add our own Sunspec library
add_library(sunspec SHARED ${sunspec})
set_target_properties(sunspec PROPERTIES LINKER_LANGUAGE CXX)
