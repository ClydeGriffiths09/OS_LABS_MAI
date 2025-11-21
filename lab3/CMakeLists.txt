cmake_minimum_required(VERSION 3.10)
project(OS_LABS CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Общие исходники (всё, кроме main)
set(COMMON_SOURCES
    src/parent.cpp
    src/child.cpp
    src/stringprocessor.cpp
    src/os.cpp
)

# Родитель
add_executable(parent
    main.cpp
    ${COMMON_SOURCES}
)

# Дочерний процесс
add_executable(child
    src/child_main.cpp
    ${COMMON_SOURCES}   # ← ДОБАВЛЕНО!
)

# Установка выходной директории для ОБОИХ программ
set_target_properties(parent child
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
