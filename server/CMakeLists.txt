cmake_minimum_required(VERSION 3.0)

project(celeste_server VERSION 0.0.1 LANGUAGES CXX)

include_directories(".")
link_directories("/usr/local/lib")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O2 -stdlib=libc++")
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
        ${tests}
    )

# Find packages
find_library(restbed librestbed.dylib)
find_library(MySQL libmysqlcppconn8.dylib)

# Link executable
add_executable(celeste_server ${SOURCE_FILES})
target_link_libraries(celeste_server ${restbed})
target_link_libraries(celeste_server ${MySQL})

# Add our own Sunspec library
add_library(sunspec SHARED ${sunspec})
set_target_properties(sunspec PROPERTIES LINKER_LANGUAGE CXX)
