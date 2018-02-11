cmake_minimum_required(VERSION 2.8.12)

INCLUDE(CheckIncludeFiles)

file(GLOB_RECURSE SHADERS_FILES *.frag *.vert *.geom)
file(GLOB_RECURSE SRC_FILES *.cpp *.h *.inl ../include/*.h ../include/*.inl)

if (USE_VULKAN)

	# Vulkan
	find_package(Vulkan REQUIRED)

	add_definitions(-DHAVE_VULKAN=1 -DHAVE_OPENGL=0)

else (USE_VULKAN)

	# OpenGL
	find_package(OpenGL REQUIRED)

	add_definitions(-DHAVE_OPENGL=1 -DHAVE_VULKAN=0)

	CHECK_INCLUDE_FILES (GL/glcorearb.h HAVE_GL_GLCOREARB_H)
	CHECK_INCLUDE_FILES (GL/gl3.h HAVE_GL_GL3_H)
	CHECK_INCLUDE_FILES (GL/gl.h HAVE_GL_GL_H)
	CHECK_INCLUDE_FILES (OpenGL/gl.h HAVE_OPENGL_GL_H)

endif (USE_VULKAN)

# TinyXML
find_package(TinyXML2 REQUIRED)

# Compile application
add_library(${PROJECT_NAME} ${SRC_FILES} ${UI_FILES} ${SHADERS_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC . ../include/)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
	target_link_libraries(${PROJECT_NAME} PUBLIC glBreak)
ENDIF()

target_link_libraries(${PROJECT_NAME} PUBLIC "rt" "dl")

target_link_libraries(${PROJECT_NAME} PUBLIC scene linear-algebra ${OPENGL_LIBRARIES} ${TINYXML2_LIBRARY} Remotery)

add_custom_target(deploy_shaders ALL COMMAND "${PROJECT_SOURCE_DIR}/scripts/deploy_shaders.sh" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

add_test(NAME check_shaders COMMAND "${PROJECT_SOURCE_DIR}/scripts/check_shaders.sh" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/data/shaders")
add_test(NAME check_renderer COMMAND xmllint --noout --valid render.xml WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/data")
