cmake_minimum_required(VERSION 2.6)

project(Sandbox3D)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# My Sources
include_directories(include)

include_directories(external/linear-algebra/include external/curves/src external/assimp/include)

add_subdirectory(external)

file(GLOB_RECURSE SHADERS_FILES data/shaders/*)
file(GLOB_RECURSE UI_FILES ui/*.ui)
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h src/*.inl include/*.h include/*.inl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
endif()

# Qt
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5OpenGL)

include_directories(
	${Qt5Core_INCLUDE_DIRS}
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
	${Qt5OpenGL_INCLUDE_DIRS}
)

if(Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

qt5_wrap_ui(UI_HEADER_FILES ${UI_FILES})

set(QT_USE_OPENGL TRUE)

ADD_DEFINITIONS(${QT_DEFINITIONS})

# OpenGL
find_package(OpenGL REQUIRED)

# Compile application
add_executable(${PROJECT_NAME} ${SRC_FILES} ${UI_FILES} ${SHADERS_FILES})

target_link_libraries(${PROJECT_NAME} glBreak)

target_link_libraries(${PROJECT_NAME} assimp)

target_link_libraries(${PROJECT_NAME} "rt")

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${ASSIMP_LIB} Qt5::Core Qt5::Widgets Qt5::Gui Qt5::OpenGL)
