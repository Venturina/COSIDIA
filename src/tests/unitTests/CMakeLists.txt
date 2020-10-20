#add_executable(ObjectActionManagerTest ObjectActionManagerTest.cpp)

#target_include_directories(ObjectActionManagerTest PUBLIC ${PROJECT_SOURCE_DIR}/extern)

add_catch_test(ActionList ActionListTest.cpp)
add_catch_test(ObjectManager ObjectActionManagerTest.cpp)
add_catch_test(BaseObject BaseObjectTest.cpp)
add_catch_test(ObjectList ObjectListTest.cpp)