project(paresis)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_REQUIRED_FLAGS -std=c++17)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(Boost COMPONENTS context system fiber thread REQUIRED)

include_directories(src)

add_executable( paresis
        src/core/Action.cpp
        src/core/ActionList.cpp
        src/core/Clock.cpp
        src/core/Core.cpp
        src/core/main.cpp
        src/core/SteadyClock.cpp
        src/fiber/customWorkStealing.cpp
        src/mobility/MobilityManager.cpp
        src/objects/BaseObject.cpp
        src/objects/ObjectActionManager.cpp
        src/objects/ObjectContext.cpp
        src/objects/ObjectList.cpp
        src/objects/SimulationManager.cpp
        src/radio/MediumAccess.cpp
        src/radio/Microchannel.cpp
        src/radio/Radio.cpp
        )

target_link_libraries(paresis
        Boost::boost
        Boost::context
        Boost::system
        Boost::thread
        Boost::fiber
)