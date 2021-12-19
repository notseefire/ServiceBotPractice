cmake_minimum_required(VERSION 3.21)
project(Web)

set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false *.hpp *.cpp)

set(JSONXX_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../lib/jsonxx")
set(JSONXX_SRC "${JSONXX_ROOT}/jsonxx.cc")
set(JSONXX_HEADER "${JSONXX_ROOT}/jsonxx.h")

set(SOURCES ${SOURCES}
    ${JSONXX_SRC}
    ${JSONXX_HEADER}
)

link_libraries(stdc++fs)

find_package(Boost 1.76 REQUIRED COMPONENTS
        log)

include_directories(${Boost_INCLUDE_DIRS})

add_library(Web STATIC ${SOURCES})

target_link_libraries(Web PRIVATE
    ${Boost_LIBRARIES})
