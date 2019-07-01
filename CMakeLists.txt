project(paresis)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_REQUIRED_FLAGS -std=c++17)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(Boost COMPONENTS fiber context REQUIRED)

include_directories(src)

add_executable( paresis
        src/core/main.cpp
        src/objects/ObjectContext.cpp
        src/core/ObjectList.cpp
        )