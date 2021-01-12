set(SOURCES
    src/traci/Angle.cc
    src/traci/API.cpp
    src/traci/LiteAPI.cc
    src/traci/Position.cc
    src/traci/PosixLauncher.cpp
    src/traci/sumo/foreign/tcpip/socket.cpp
    src/traci/sumo/foreign/tcpip/storage.cpp
    src/traci/sumo/utils/traci/TraCIAPI.cpp
)

set_property(TARGET cosidia APPEND PROPERTY SOURCES ${SOURCES})
target_include_directories(cosidia PUBLIC sumo ${Boost_INCLUDE_DIRS})