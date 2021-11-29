set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(httplib_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cpp-httplib")
set(BINARY ${CMAKE_PROJECT_NAME})

#file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)
#set(SOURCES ${SOURCES})

cmake_minimum_required(VERSION 3.21)
project(Service)

find_package(Boost 1.76 REQUIRED COMPONENTS
        log)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cpp-httplib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/parser)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/googletest)

message("${httplib_FOUND}")

set(JSONXX_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/lib/jsonxx")
set(JSONXX_SRC "${JSONXX_ROOT}/jsonxx.cc")
set(JSONXX_HEADER "${JSONXX_ROOT}/jsonxx.h")
set(HTTPLIB_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/cpp-httplib/httplib.h")
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${JSONXX_SRC}
    ${JSONXX_HEADER}
    ${HTTPLIB_HEADER}
)

add_executable(${BINARY}_run ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(${BINARY}_run PRIVATE
        ${Boost_LIBRARIES}
        httplib
        Parser)
    target_include_directories(${BINARY}_run PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp-httplib
        ${CMAKE_CURRENT_SOURCE_DIR}/parser)
    target_link_libraries(${BINARY}_lib PRIVATE
        ${Boost_LIBRARIES}
        httplib
        Parser)
    target_include_directories(${BINARY}_lib PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp-httplib
        ${CMAKE_CURRENT_SOURCE_DIR}/parser)
endif()
