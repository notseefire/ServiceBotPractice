cmake_minimum_required(VERSION 3.21)
project(Parser)

set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false *.hpp *.cpp)

set(SOURCES ${SOURCES})

link_libraries(stdc++fs)

find_package(Boost 1.76 REQUIRED COMPONENTS
        log)

include_directories(
        ${Boost_INCLUDE_DIRS}
)

add_library(Parser STATIC ${SOURCES})

target_link_libraries(Parser PRIVATE
    ${Boost_LIBRARIES}
)
