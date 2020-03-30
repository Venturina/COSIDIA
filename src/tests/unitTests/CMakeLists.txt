#add_executable(ObjectActionManagerTest ObjectActionManagerTest.cpp)

#target_include_directories(ObjectActionManagerTest PUBLIC ${PROJECT_SOURCE_DIR}/extern)

add_catch_test(ObjectManager ObjectActionManagerTest.cpp)
