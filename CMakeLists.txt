cmake_minimum_required(VERSION 2.8.12)

project(Sandbox3D)

enable_testing()

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug) # Default
endif ()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# My Sources
include_directories(include)

include_directories(external/linear-algebra/include external/qt5-node-editor/src)

add_subdirectory(external)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Werror=return-local-addr -Werror=return-type")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address ")

option(USE_VULKAN "Use the Vulkan API for rendering" OFF)

add_subdirectory(src)

