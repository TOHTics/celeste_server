# --- Perform different tests using Catch ------

cmake_minimum_required(VERSION 3.0)

project(server_test LANGUAGES CXX)

# ---- Configure compiler ----------------------
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD TRUE)

message("${gray}Catch2 is needed to perform the tests.${reset}")

if (NOT EXISTS "${PROJECT_SOURCE_DIR}/catch/catch.hpp")
    message("${gray}We will attempt to download the latest version from the internet.${reset}")
    
    file(DOWNLOAD
         "https://github.com/catchorg/Catch2/releases/download/v2.0.1/catch.hpp"
         "${PROJECT_SOURCE_DIR}/catch/catch.hpp"
         TIMEOUT    10
         STATUS     DOWNLOAD_STATUS
         SHOW_PROGRESS
         )
    list(GET ${DOWNLOAD_STATUS} 0 IS_ERROR)
    
    if (${IS_ERROR})
        message(FATAL_ERROR "${green}Download completed succesfully!${reset}")
    endif()
endif()

include_directories("catch/")

# --- Gather all tests ---------------
file(GLOB_RECURSE test_glob "tests/*.cpp")
set(test_src ${test_glob})
add_executable(${PROJECT_NAME} ${test_src})
