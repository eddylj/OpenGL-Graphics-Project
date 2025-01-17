cmake_minimum_required(VERSION 3.12)

project("COMP3421 Assignment 3")

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# If I was better at CMake I would make this nicer
if (NOT (EXISTS ${PROJECT_SOURCE_DIR}/lib))
    message(FATAL_ERROR "Dependencies not found. Please run fetch_deps.sh [-G Ninja] from the project root and reconfigure")
endif ()

find_package(glad REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib)
find_package(glfw3 REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib)
find_package(glm REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib)
find_package(stb REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib)
find_package(tinyobjloader REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib)
find_package(chicken3421 REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib)

set(COMMON_LIBS glad::glad glm::glm glfw stb tinyobjloader::tinyobjloader chicken3421)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include(copy_resources)
copy_resources(${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res)

set(ACTIVITY ass3)
add_executable(${ACTIVITY})
target_include_directories(${ACTIVITY} PUBLIC include)
target_sources(${ACTIVITY}
        ${target}
        PRIVATE
        include/ass3/memes.hpp
        include/ass3/camera.hpp
        include/ass3/primitives.hpp
        include/ass3/renderer.hpp
        include/ass3/scene.hpp
        include/ass3/animator.hpp
        include/ass3/framebuffer.hpp
        include/ass3/cubemap.hpp
        include/ass3/light.hpp

        src/memes.cpp

        src/main.cpp
        src/camera.cpp
        src/primitives.cpp
        src/renderer.cpp
        src/scene.cpp
        src/animator.cpp
        src/framebuffer.cpp
        src/cubemap.cpp
        src/light.cpp
        )
target_link_libraries(${ACTIVITY} PUBLIC ${COMMON_LIBS})
target_compile_options(
        ${ACTIVITY}
        PRIVATE
        -Wall -Wextra -pedantic -fvisibility=hidden -fdiagnostics-color=always -Wcast-align
        -Wconversion -Wdouble-promotion -Wshadow -Wsign-conversion -Wsign-promo -Wnull-dereference -Wodr
)