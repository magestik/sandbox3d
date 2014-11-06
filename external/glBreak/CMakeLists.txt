cmake_minimum_required(VERSION 2.6)

project(glBreak)

file(GLOB SRC src/*.c src/*.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(include)

add_library(glBreak SHARED ${SRC})

find_package(OpenGL)

target_link_libraries(glBreak ${GL_LIBRARIES})

target_link_libraries(glBreak "dl")
