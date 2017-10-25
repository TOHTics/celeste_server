cmake_minimum_required(VERSION 3.8)
project(client)

include_directories(".")
set(CMAKE_CXX_STANDARD 14)

include_directories(/Users/carlos/Documents/Trabajo/solarpanels/source/server)
link_directories(.)

set(SOURCE_FILES main.cpp)

find_library(Restbed_Library librestbed.dylib)
set(SunSpec_Library libsunspec.dylib)
# Add the executable
add_executable(client ${SOURCE_FILES})
target_link_libraries(client ${Restbed_Library})
target_link_libraries(client ${SunSpec_Library})