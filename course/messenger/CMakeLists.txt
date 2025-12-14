cmake_minimum_required(VERSION 3.14)
project(chat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED IMPORTED_TARGET libzmq)

find_package(Threads REQUIRED)

find_library(SQLITE3_LIBRARY sqlite3 REQUIRED)
find_path(SQLITE3_INCLUDE_DIR sqlite3.h REQUIRED)

set(COMMON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)

add_executable(server
    server/main_server.cpp
    server/server.cpp
    common/message.cpp
    thread/thread.cpp
)

target_include_directories(server PRIVATE ${COMMON_INCLUDE_DIR} ${SQLITE3_INCLUDE_DIR})
target_link_libraries(server PRIVATE PkgConfig::ZMQ Threads::Threads ${SQLITE3_LIBRARY})

add_executable(client
    client/main_client.cpp
    client/client.cpp
    common/message.cpp
    thread/thread.cpp
)

target_include_directories(client PRIVATE ${COMMON_INCLUDE_DIR})
target_link_libraries(client PRIVATE PkgConfig::ZMQ Threads::Threads)