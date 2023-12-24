cmake_minimum_required(VERSION 3.10)
project(oop_7)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
add_subdirectory(googletest)

add_executable(main main.cpp npc.cpp orc.cpp outlaw.cpp werewolf.cpp)

add_executable(tests tests.cpp npc.cpp orc.cpp outlaw.cpp werewolf.cpp)

target_link_libraries(tests PRIVATE GTest::gtest_main)
include(GoogleTest)