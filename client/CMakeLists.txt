cmake_minimum_required(VERSION 3.8)
project(client)

# link_directories("/usr/local/lib")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../cmake/modules")

find_package(restbed        REQUIRED)

include_directories(${restbed_INCLUDE})
include_directories("/Users/carlos/Documents/Trabajo/celeste/solarplant/server")

link_directories(".")

set(SOURCE_FILES main.cpp)

find_library(Restbed_Library librestbed.dylib)
set(SunSpec_Library libsunspec.dylib)
# Add the executable
add_executable(client ${SOURCE_FILES})
target_link_libraries(client ${Restbed_Library})
target_link_libraries(client ${SunSpec_Library})