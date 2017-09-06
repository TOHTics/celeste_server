cmake_minimum_required(VERSION 3.8)
project(server)

include_directories(.)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp
        sunspec/data/SunSpecData.hpp sunspec/data/DeviceData.hpp
        sunspec/data/ModelData.hpp sunspec/data/PointData.hpp
        sunspec/util/error.hpp sunspec/data/data.hpp sunspec/util/sdx_tags.hpp
        sunspec/translator/IntegerTranslator.hpp sunspec/translator/FloatTranslator.hpp
        sunspec/data/PointData.cpp sunspec/data/ModelData.cpp sunspec/data/DeviceData.cpp
        sunspec/data/SunSpecData.cpp
        srv/resource/logger_upload.cpp srv/resource/logger_upload.hpp
        srv/resource/resource.hpp sunspec/data/SunSpecDataResponse.cpp sunspec/data/SunSpecDataResponse.hpp
        sunspec/util/status_code.hpp test/tests.cpp test/tests.hpp sunspec/data/DeviceResult.cpp sunspec/data/DeviceResult.hpp sunspec/model/GPSReading.hpp sunspec/model/OpenEnergyMonitor.cpp sunspec/model/OpenEnergyMonitor.hpp sunspec/model/Thermometer.cpp sunspec/model/Thermometer.hpp sunspec/model/CommonModel.hpp sunspec/point/Altitude.hpp sunspec/sunspec.hpp sunspec/util/util.hpp)



find_package(Boost REQUIRED)
find_library(Restbed_Library librestbed.dylib)

# Find packages
add_executable(server ${SOURCE_FILES})
target_link_libraries(server ${Boost_LIBRARIES})
target_link_libraries(server ${Restbed_Library})


file(GLOB_RECURSE sunspec_SRC
        "sunspec/*.hpp"
        "sunspec/*.cpp")
set(sunspec ${sunspec_SRC})
add_library(sunspec SHARED ${sunspec})
set_target_properties(sunspec PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(sunspec ${Boost_LIBRARIES})