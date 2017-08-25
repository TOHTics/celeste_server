cmake_minimum_required(VERSION 3.8)
project(server)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp
        sunspec/data/element/SunSpecData.hpp sunspec/data/element/DeviceData.hpp
        sunspec/data/element/ModelData.hpp sunspec/data/element/PointData.hpp
        sunspec/data/util/error.hpp sunspec/data/data.hpp sunspec/data/util/sdx_tags.hpp
        sunspec/data/translator/IntegerTranslator.hpp sunspec/data/translator/FloatTranslator.hpp
        sunspec/data/element/PointData.cpp sunspec/data/element/ModelData.cpp sunspec/data/element/DeviceData.cpp
        sunspec/data/element/SunSpecData.cpp
        srv/resources/resource.cpp srv/resources/resource.hpp srv/services/service.cpp srv/services/service.hpp)


find_package(Boost COMPONENTS date_time REQUIRED)

find_library(Restbed_Library librestbed.dylib)

# Find packages
add_executable(server ${SOURCE_FILES})
target_link_libraries(server ${Boost_LIBRARIES})
target_link_libraries(server ${Restbed_Library})
