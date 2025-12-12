cmake_minimum_required(VERSION 3.14)
project(chat)

set(CMAKE_CXX_STANDARD 17)

# ZeroMQ через pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

# Создаем цель ZeroMQ вручную, если она не существует
if(NOT TARGET PkgConfig::ZMQ)
    add_library(PkgConfig::ZMQ INTERFACE IMPORTED)
    target_include_directories(PkgConfig::ZMQ INTERFACE ${ZMQ_INCLUDE_DIRS})
    target_link_libraries(PkgConfig::ZMQ INTERFACE ${ZMQ_LIBRARIES})
    if(ZMQ_CFLAGS_OTHER)
        target_compile_options(PkgConfig::ZMQ INTERFACE ${ZMQ_CFLAGS_OTHER})
    endif()
    message(STATUS "Created PkgConfig::ZMQ target manually")
endif()

# SQLite3
find_library(SQLITE3_LIBRARY sqlite3 REQUIRED)
find_path(SQLITE3_INCLUDE_DIR sqlite3.h REQUIRED)

find_package(Threads REQUIRED)

# Сервер
add_executable(server
    server/main_server.cpp
    server/server.cpp
)
target_include_directories(server PRIVATE 
    common 
    ${SQLITE3_INCLUDE_DIR}
    ${ZMQ_INCLUDE_DIRS}  # Добавляем явно на всякий случай
)
target_link_libraries(server PRIVATE 
    PkgConfig::ZMQ 
    Threads::Threads 
    ${SQLITE3_LIBRARY}
)

# Клиент
add_executable(client
    client/main_client.cpp
    client/client.cpp
)
target_include_directories(client PRIVATE 
    common 
    ${ZMQ_INCLUDE_DIRS}
)
target_link_libraries(client PRIVATE 
    PkgConfig::ZMQ 
    Threads::Threads
)

# Вывод информации для отладки
message(STATUS "ZeroMQ include dirs: ${ZMQ_INCLUDE_DIRS}")
message(STATUS "ZeroMQ libraries: ${ZMQ_LIBRARIES}")
message(STATUS "SQLite3 library: ${SQLITE3_LIBRARY}")
message(STATUS "SQLite3 include dir: ${SQLITE3_INCLUDE_DIR}")