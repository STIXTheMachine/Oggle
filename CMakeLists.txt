cmake_minimum_required(VERSION 3.15)
project(OggleEngine)

option(OGGLE_BUILD_TESTS ON)

add_subdirectory(Engine)
add_subdirectory(Projects/OggleApp)
add_subdirectory(Projects/Scratch)

#if(OGGLE_BUILD_TESTS)
    add_subdirectory(TestSuite)
#endif()