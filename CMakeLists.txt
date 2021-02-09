cmake_minimum_required(VERSION 3.13)
project(COSIDIA VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(Boost COMPONENTS atomic context system fiber thread REQUIRED)
find_package(TBB REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(xxhash REQUIRED IMPORTED_TARGET libxxhash)

set(VANETZA_EXPORT_PACKAGE OFF)
set(VANETZA_INSTALL OFF)
add_subdirectory(extern/vanetza)

set(SQLITECPP_RUN_CPPCHECK OFF)
set(SQLITECPP_RUN_CPPLINT OFF)
set(SQLITECPP_USE_STATIC_RUNTIME OFF)
add_subdirectory(extern/sqlitecpp)

add_library(cosidia STATIC
        extern/loguru/loguru.cpp

        src/core/Action.cpp
        src/core/ActionList.cpp
        src/core/Config.cpp
        src/core/Core.cpp
        src/core/DebugClock.cpp
        src/core/Symbol.cpp
        src/core/SteadyClock.cpp
        src/fiber/customWorkStealing.cpp
        src/mobility/MobilityManager.cpp
        src/mobility/SumoMobilityManager.cpp
        src/mobility/SumoUpdater.cpp
        src/networking/AccessInterface.cpp
        src/networking/PositionProvider.cpp
        src/networking/Router.cpp
        src/networking/Runtime.cpp
        src/objects/BaseObject.cpp
        src/objects/CacheQuery.cpp
        src/objects/ObjectActionManager.cpp
        src/objects/ObjectCache.cpp
        src/objects/ObjectContext.cpp
        src/objects/ObjectFactory.cpp
        src/objects/ObjectList.cpp
        src/objects/ObjectRemover.cpp
        src/objects/SimulationManager.cpp
        src/objects/TemporaryObject.cpp
        src/objects/VehicleObject.cpp
        src/output/TimingScope.cpp
        src/output/SqliteBackend.cpp
        src/radio/InterferenceDecider.cpp
        src/radio/MediumAccess.cpp
        src/radio/Microchannel.cpp
        src/radio/PhysicalDecider.cpp
        src/radio/Transmission.cpp
        src/radio/Radio.cpp
        src/utils/Geometry.cc
)

option(GUI_MODE "Enable GUI extension" OFF)
if(GUI_MODE)
        find_package(Protobuf REQUIRED)
        include_directories(${PROTOBUF_INCLUDE_DIR})
        target_sources(cosidia PRIVATE
                src/guiConnector/GuiConnector.cpp
                src/guiConnector/gen/guiMsg.pb.cc
        )
        target_compile_definitions(cosidia PUBLIC WITH_GUI)
        target_link_libraries(cosidia PUBLIC
                ${PROTOBUF_LIBRARY}
        )
endif()

target_include_directories(cosidia PUBLIC src extern extern/hypodermic)
target_link_libraries(cosidia PUBLIC
        Vanetza::vanetza
        Boost::boost
        Boost::context
        Boost::system
        Boost::thread
        Boost::fiber
        ${CMAKE_DL_LIBS}
        yaml-cpp
        pthread
        PkgConfig::xxhash
        SQLiteCpp
        TBB::tbb
)

option(COSIDIA_SAFE_MODE "Build COSIDIA with safety runtime checks" ON)
if(COSIDIA_SAFE_MODE)
        target_compile_definitions(cosidia PUBLIC COSIDIA_SAFE)
endif()

option(COSIDIA_MEASURE_TIMING "Build COSIDIA with timing probes" ON)
if(COSIDIA_MEASURE_TIMING)
        target_compile_definitions(cosidia PUBLIC COSIDIA_MEASURE_TIMING)
endif()

macro(add_catch_test NAME)
        add_executable(test_${NAME} ${ARGN} ${PROJECT_SOURCE_DIR}/src/debug/DebugObject.cpp)
        target_include_directories(test_${NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/extern)
        target_link_libraries(test_${NAME} cosidia)
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

add_executable(cosidia_exec src/core/main.cpp)
target_link_libraries(cosidia_exec cosidia)
target_include_directories(cosidia_exec PUBLIC extern/CLI11/include)
set_property(TARGET cosidia_exec PROPERTY OUTPUT_NAME cosidia)

option(USE_JEMALLOC "Use jemalloc memory allocation library" OFF)
if (USE_JEMALLOC)
        find_package(jemalloc MODULE REQUIRED)
        target_link_libraries(cosidia PUBLIC jemalloc::jemalloc)
endif()
