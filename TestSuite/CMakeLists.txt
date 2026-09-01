cmake_minimum_required(VERSION 3.15)

add_executable(TestSuite Source/Private/main.cpp)
target_link_libraries(TestSuite PRIVATE Oggle::CoreTest)