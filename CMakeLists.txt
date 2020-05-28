cmake_minimum_required(VERSION 3.10)
project(DEDCOM)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pthread")

include_directories(include)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/include/*.hpp)

file(GLOB SV_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/server/*.cpp)
add_executable(dedcom_server ${SV_SOURCES})


file(GLOB CL_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/client/*.cpp)
add_executable(dedcom_client ${CL_SOURCES})