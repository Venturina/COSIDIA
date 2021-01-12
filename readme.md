# COSIDIA

COSIDIA is your concurrent simulation of discrete actions.

resolve linking issue: export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH:/home/obermaier/lib/boost_1_70_0/lib

invoke protoc:
cd to ./src/guiConnector
protoc --cpp_out=gen guiMsg.proto --experimental_allow_proto3_optional