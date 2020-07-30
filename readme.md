This is the repository for PARESIS - A PArallel discRete Event Simulator for ItS

resolve linking issue: export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH:/home/obermaier/lib/boost_1_70_0/lib

invoke protoc:
cd to ./src/guiConnector
protoc --cpp_out=gen guiMsg.proto --experimental_allow_proto3_optional