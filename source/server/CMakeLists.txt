cmake_minimum_required(VERSION 3.0)
project(server)

include_directories(.)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O2")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -- Sunspec ----------------
file(GLOB_RECURSE SUNSPEC_SRC
    "sunspec/*.hpp"
    "sunspec/*.cpp"
    )
set(sunspec ${SUNSPEC_SRC})

# -- Server -----------------
file(GLOB_RECURSE SRV_SRC
    "srv/*.hpp"
    "srv/*.cpp"
    )
set(srv ${SRV_SRC})

# -- Database ---------------
file(GLOB_RECURSE DB_SRC
    "db/*.hpp"
    "db/*.cpp"
    )
set(db ${DB_SRC})
message("DB SRC: " ${db})

# -- Tests ------------------
file(GLOB_RECURSE TEST_SRC
    "test/*.hpp"
    "test/*.cpp"
    )
set(tests ${TEST_SRC})

# -- Mix all source files --
set(SOURCE_FILES 
        main.cpp

        ${sunspec}
        ${srv}
        ${db}
        ${tests}
    )

# Find packages
find_package(Boost COMPONENTS date_time REQUIRED)
find_library(Restbed_Library librestbed.dylib)
find_library(MySQL_Library libmysqlcppconn.dylib)

# Link executable
add_executable(server ${SOURCE_FILES})
target_link_libraries(server ${Boost_LIBRARIES})
target_link_libraries(server ${Restbed_Library})
target_link_libraries(server ${MySQL_Library})

# Add our own Sunspec library
add_library(sunspec SHARED ${sunspec})
set_target_properties(sunspec PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(sunspec ${Boost_LIBRARIES})
