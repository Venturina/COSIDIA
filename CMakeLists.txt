project(paresis)
cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(Boost COMPONENTS atomic context system fiber thread REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
add_subdirectory(extern/CLI11)

add_library(libparesis STATIC
        extern/loguru/loguru.cpp

        src/core/Action.cpp
        src/core/ActionList.cpp
        src/core/Clock.cpp
        src/core/Config.cpp
        src/core/Core.cpp
        src/core/DebugClock.cpp
        src/core/main.cpp
        src/core/SteadyClock.cpp
        src/fiber/customWorkStealing.cpp
        src/mobility/MobilityManager.cpp
        src/mobility/SumoMobilityManager.cpp
        src/mobility/SumoUpdater.cpp
        src/objects/BaseObject.cpp
        src/objects/ObjectActionManager.cpp
        src/objects/ObjectContext.cpp
        src/objects/ObjectFactory.cpp
        src/objects/ObjectList.cpp
        src/objects/ObjectRemover.cpp
        src/objects/SimulationManager.cpp
        src/objects/TemporaryObject.cpp
        src/objects/VehicleObject.cpp
        src/radio/MediumAccess.cpp
        src/radio/Microchannel.cpp
        src/radio/Radio.cpp
        src/utils/Geometry.cc
)
target_include_directories(libparesis PUBLIC src extern extern/hypodermic)
target_link_libraries(libparesis PUBLIC
        Boost::boost
        Boost::context
        Boost::system
        Boost::thread
        Boost::fiber
        CLI11::CLI11
        ${CMAKE_DL_LIBS}
        yaml-cpp
	pthread
)
set_property(TARGET libparesis PROPERTY OUTPUT_NAME paresis)

option(PARESIS_SAFE_MODE "foo" ON)
if(PARESIS_SAFE_MODE)
        target_compile_definitions(libparesis PUBLIC PARESIS_SAFE)
endif()

macro(add_catch_test NAME)
        add_executable(test_${NAME} ${ARGN} ${PROJECT_SOURCE_DIR}/src/debug/DebugObject.cpp)
        target_include_directories(test_${NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/extern)
        target_link_libraries(test_${NAME} libparesis)
        set_target_properties(test_${NAME} PROPERTIES
                RUNTIME_OUTPUT_NAME ${NAME}
                RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/tests
        )
        target_compile_definitions(test_${NAME} PRIVATE CATCH_CONFIG_MAIN)
        add_test(NAME ${NAME}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMAND test_${NAME})
endmacro()
enable_testing()

add_subdirectory(src/traci)
add_subdirectory(src/tests/unitTests)

add_executable(paresis src/core/main.cpp)
target_link_libraries(paresis libparesis)

option(USE_JEMALLOC "Use jemalloc memory allocation library" OFF)
if (USE_JEMALLOC)
        find_package(jemalloc MODULE REQUIRED)
        target_link_libraries(libparesis PUBLIC jemalloc::jemalloc)
endif()
