cmake_minimum_required(VERSION 3.10)
project(OS_LABS CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(parent 
    main.cpp 
    src/parent.cpp 
    src/child.cpp       
    src/stringprocessor.cpp
    src/os.cpp
)

set_target_properties(parent
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
