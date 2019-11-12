set(SOURCES
    Angle.cc
    API.cpp
    LiteAPI.cc
    Position.cc
    PosixLauncher.cpp
    sumo/foreign/tcpip/socket.cpp
    sumo/foreign/tcpip/storage.cpp
    sumo/utils/traci/TraCIAPI.cpp
)

add_library(traci SHARED ${SOURCES})

target_include_directories(traci PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/traci/sumo ${PROJECT_SOURCE_DIR}/extern )