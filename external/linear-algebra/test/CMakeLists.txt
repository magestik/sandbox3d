cmake_minimum_required(VERSION 2.8)

project(test)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(../include/)

file(GLOB_RECURSE HEADERS ../include/*.h ../include/*.inl)

add_executable(${PROJECT_NAME} test.cpp ${HEADERS})

