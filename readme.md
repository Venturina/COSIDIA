# COSIDIA

COSIDIA is your concurrent simulation of discrete actions.

# DESCRIPTION
Please see https://dl.acm.org/doi/10.1145/3437959.3459250 for details about the simulator.
# POSSIBLE ISSUES
resolve linking issue: export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH:/home/obermaier/lib/boost_1_70_0/lib

invoke protoc:
cd to ./src/guiConnector
protoc --cpp_out=gen guiMsg.proto --experimental_allow_proto3_optional