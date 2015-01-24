cmake_minimum_required(VERSION 2.6)

project(Sandbox3D)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# My Sources
include_directories(include)

include_directories(external/linear-algebra/include external/curves/src external/assimp/include)

add_subdirectory(external)

file(GLOB_RECURSE SHADERS_FILES src/*.frag src/*.vert)
file(GLOB_RECURSE UI_FILES ui/*.ui)
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h src/*.inl include/*.h include/*.inl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions -Wall -Wextra")

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


IF(CMAKE_BUILD_TYPE MATCHES Debug)
	message("Debug build.")
	target_link_libraries(${PROJECT_NAME} glBreak)
ENDIF()

target_link_libraries(${PROJECT_NAME} assimp)

target_link_libraries(${PROJECT_NAME} "rt")

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${ASSIMP_LIB} Qt5::Core Qt5::Widgets Qt5::Gui Qt5::OpenGL)
