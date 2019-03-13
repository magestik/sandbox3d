cmake_minimum_required(VERSION 3.3)

INCLUDE(CheckIncludeFiles)

file(GLOB_RECURSE SHADERS_FILES *.frag *.vert *.geom)
file(GLOB_RECURSE RENDERING_SOURCE_FILES Rendering/*.cpp Rendering/*.h Rendering/*.inl)
file(GLOB_RECURSE RHI_SOURCE_FILES RHI/*.cpp RHI/*.h RHI/*.inl)

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

find_package(RenderGraph REQUIRED)

# Compile application
add_library(Sandbox3D STATIC ${RENDERING_SOURCE_FILES} ${RHI_SOURCE_FILES} ${UI_FILES} ${SHADERS_FILES})
target_include_directories(Sandbox3D PUBLIC . ../include/)

add_library(Sandbox3D_EditorInterface SHARED EditorInterface.cpp)
target_link_libraries(Sandbox3D_EditorInterface PRIVATE Sandbox3D)
set_target_properties(Sandbox3D_EditorInterface PROPERTIES C_VISIBILITY_PRESET hidden)
set_target_properties(Sandbox3D_EditorInterface PROPERTIES CXX_VISIBILITY_PRESET hidden)

target_link_libraries(Sandbox3D PUBLIC scene linear-algebra RenderGraph ${OPENGL_LIBRARIES} Remotery)

add_custom_target(deploy_shaders ALL COMMAND "${PROJECT_SOURCE_DIR}/scripts/deploy_shaders.sh" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

add_test(NAME check_shaders COMMAND "${PROJECT_SOURCE_DIR}/scripts/check_shaders.sh" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/data/shaders")
