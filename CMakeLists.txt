cmake_minimum_required(VERSION 3.15)
project(game)
set(CMAKE_CXX_STANDARD 14)
set(SOURCE main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})
#
#MESSAGE("project name: " ${PROJECT_NAME} )
#MESSAGE("project src dir: " ${PROJECT_SOURCE_DIR} )
#FOREACH(item ${GLFW3_STATIC_LIBRARIES})
#    MESSAGE(STATUS "  using lib: " ${item})
#ENDFOREACH()
#
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    MESSAGE(STATUS "  [Found GLEW library]")
    MESSAGE(STATUS "  GLEW include directory:" ${GLEW_INCLUDE_DIRS})
    FOREACH(item ${GLEW_LIBRARIES})
        MESSAGE(STATUS "  using lib: " ${item})
    ENDFOREACH()

    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
endif()

#link the local openGL driver library
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    MESSAGE(STATUS "  [Found OpenGL library]")
    MESSAGE(STATUS "  OpenGL include directory:" ${OPENGL_INCLUDE_DIR})
    FOREACH(item ${OPENGL_LIBRARIES})
        MESSAGE(STATUS "  using lib: " ${item})
    ENDFOREACH()

    include_directories(${OPENGL_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall")
# using c++14
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_EXTENSIONS OFF)