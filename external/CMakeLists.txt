add_subdirectory(glBreak)

if (NOT TARGET linear-algebra)
    add_subdirectory(linear-algebra)
endif ()

find_package(Threads)
add_library(Remotery STATIC Remotery/lib/Remotery.c)
target_include_directories(Remotery PUBLIC Remotery/lib/)
target_link_libraries(Remotery PUBLIC ${CMAKE_THREAD_LIBS_INIT})
target_compile_definitions(Remotery PUBLIC RMT_USE_OPENGL=1)
