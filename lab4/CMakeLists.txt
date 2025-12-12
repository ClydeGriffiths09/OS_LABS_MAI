cmake_minimum_required(VERSION 3.14)
project(lab5)

set(CMAKE_CXX_STANDARD 17)

# DynamicLibrary
add_library(dynamic_lib src/dynamic_library.cpp)
target_include_directories(dynamic_lib PUBLIC include)

# Библиотеки
add_library(naive_prime SHARED lib/naive_prime.cpp)
add_library(eratosthenes_prime SHARED lib/eratosthenes_prime.cpp)
add_library(leibniz_pi SHARED lib/leibniz_pi.cpp)
add_library(wallis_pi SHARED lib/wallis_pi.cpp)

target_include_directories(naive_prime PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(eratosthenes_prime PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(leibniz_pi PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(wallis_pi PRIVATE ${CMAKE_SOURCE_DIR}/include)

set_target_properties(naive_prime PROPERTIES OUTPUT_NAME "prime_naive")
set_target_properties(eratosthenes_prime PROPERTIES OUTPUT_NAME "prime_erat")
set_target_properties(leibniz_pi PROPERTIES OUTPUT_NAME "pi_leibniz")
set_target_properties(wallis_pi PROPERTIES OUTPUT_NAME "pi_wallis")

# Программа 1: набор A
add_executable(program1_setA program1_setA.cpp)
target_link_libraries(program1_setA PRIVATE naive_prime leibniz_pi)
target_include_directories(program1_setA PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Программа 1: набор B
add_executable(program1_setB program1_setB.cpp)
target_link_libraries(program1_setB PRIVATE eratosthenes_prime wallis_pi)
target_include_directories(program1_setB PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Программа 2: динамическая
add_executable(program2_dynamic program2_dynamic.cpp)
target_link_libraries(program2_dynamic PRIVATE dynamic_lib dl)
target_include_directories(program2_dynamic PRIVATE ${CMAKE_SOURCE_DIR}/include)