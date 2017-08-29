cmake_minimum_required(VERSION 3.8)
project(server)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp
        sunspec/data/element/SunSpecData.hpp sunspec/data/element/DeviceData.hpp
        sunspec/data/element/ModelData.hpp sunspec/data/element/PointData.hpp
        sunspec/data/util/Error.hpp sunspec/data/data.hpp sunspec/data/util/SDX_Tags.hpp
        sunspec/data/translator/IntegerTranslator.hpp sunspec/data/translator/FloatTranslator.hpp
        sunspec/data/element/PointData.cpp sunspec/data/element/ModelData.cpp sunspec/data/element/DeviceData.cpp
        sunspec/data/element/SunSpecData.cpp
        srv/resource/logger_upload.cpp srv/resource/logger_upload.hpp
        srv/resource/resource.hpp sunspec/data/response/SunSpecDataResponse.cpp sunspec/data/response/SunSpecDataResponse.hpp
        srv/status_code.hpp test/tests.cpp test/tests.hpp sunspec/data/response/DeviceResult.cpp sunspec/data/response/DeviceResult.hpp)


find_package(Boost COMPONENTS date_time REQUIRED)

find_library(Restbed_Library librestbed.dylib)

# Find packages
add_executable(server ${SOURCE_FILES})
target_link_libraries(server ${Boost_LIBRARIES})
target_link_libraries(server ${Restbed_Library})
