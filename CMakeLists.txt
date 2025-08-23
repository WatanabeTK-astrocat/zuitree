cmake_minimum_required(VERSION 3.14)
project(zuitree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SOURCES "source/*.cpp")

# Options mirroring Makefile variables
set(GPU "no" CACHE STRING "GPU acceleration: managed, nonmanaged, or no")
set_property(CACHE GPU PROPERTY STRINGS no managed nonmanaged)
option(DEBUG "Enable debugging" ON)
option(OPTIMIZE "Enable extra optimization" OFF)

set(EXTRA_COMPILE_FLAGS -Wall -Wextra)
set(EXTRA_LINK_FLAGS "")

if(GPU STREQUAL "managed")
    list(APPEND EXTRA_COMPILE_FLAGS -acc=gpu -gpu=cc80,mem:managed -Minfo=accel,opt)
    list(APPEND EXTRA_LINK_FLAGS -acc=gpu -gpu=mem:managed)
elif(GPU STREQUAL "nonmanaged")
    list(APPEND EXTRA_COMPILE_FLAGS -acc=gpu -gpu=cc80 -Minfo=accel,opt)
    list(APPEND EXTRA_LINK_FLAGS -acc=gpu)
endif()

if(DEBUG)
    list(APPEND EXTRA_COMPILE_FLAGS -O0 -g)
else()
    list(APPEND EXTRA_COMPILE_FLAGS -O3 -fast)
endif()

if(OPTIMIZE)
    list(APPEND EXTRA_COMPILE_FLAGS -Mfprelaxed=rsqrt)
endif()

add_executable(collapse ${SOURCES})
target_include_directories(collapse PRIVATE ${CMAKE_SOURCE_DIR}/source)
target_compile_options(collapse PRIVATE ${EXTRA_COMPILE_FLAGS})
target_link_options(collapse PRIVATE ${EXTRA_LINK_FLAGS})

# Testing
enable_testing()
add_executable(type_test tests/type_test.cpp)
target_include_directories(type_test PRIVATE ${CMAKE_SOURCE_DIR}/source)
add_test(NAME type_test COMMAND type_test)

# Doxygen documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM)
    add_custom_target(doc-clean
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/document
        COMMENT "Clean documentation")
endif()
