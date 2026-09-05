cmake_minimum_required(VERSION 3.15)
set(MODULE CoreTest)

# Must be an object library to avoid static linking issues!
add_library(${MODULE} OBJECT
        Vocabulary/Result.test.cpp
        Vocabulary/Optional.test.cpp
        Math/Color.test.cpp
)

target_link_libraries(${MODULE} PRIVATE Oggle::Testing)
target_link_libraries(${MODULE} PRIVATE Oggle::Core)

add_library(Oggle::${MODULE} ALIAS ${MODULE})