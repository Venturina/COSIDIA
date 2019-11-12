project(paresis)
cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(Boost COMPONENTS atomic context system fiber thread REQUIRED)


add_executable(paresis
        extern/loguru/loguru.cpp

        src/core/Action.cpp
        src/core/ActionList.cpp
        src/core/Clock.cpp
        src/core/Core.cpp
        src/core/DebugClock.cpp
        src/core/main.cpp
        src/core/SteadyClock.cpp
        src/fiber/customWorkStealing.cpp
        src/mobility/MobilityManager.cpp
        src/mobility/SumoMobilityManager.cpp
        src/objects/BaseObject.cpp
        src/objects/ObjectActionManager.cpp
        src/objects/ObjectContext.cpp
        src/objects/ObjectFactory.cpp
        src/objects/ObjectList.cpp
        src/objects/SimulationManager.cpp
        src/objects/TemporaryObject.cpp
        src/objects/VehicleObject.cpp
        src/radio/MediumAccess.cpp
        src/radio/Microchannel.cpp
        src/radio/Radio.cpp
        src/utils/Geometry.cc
)

add_subdirectory(src/traci)

target_include_directories(paresis PUBLIC src extern extern/hypodermic src/traci src/traci/sumo)
target_link_libraries(paresis
        Boost::boost
        Boost::context
        Boost::system
        Boost::thread
        Boost::fiber
        ${CMAKE_DL_LIBS}
        traci
)

option(USE_JEMALLOC "Use jemalloc memory allocation library" OFF)
if (USE_JEMALLOC)
        find_package(jemalloc MODULE REQUIRED)
        target_link_libraries(paresis jemalloc::jemalloc)
endif()
