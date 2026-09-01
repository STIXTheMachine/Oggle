cmake_minimum_required(VERSION 3.15)
set(MODULE Core)

add_library(${MODULE}
        Private/Logging/LoggingGlobals.cpp
        Private/ModuleInterface.cpp
        Private/Math/Color.cpp
        Private/Math/ColorConversionTables.cpp
        Private/Utilities/Assert.cpp
        Public/Core/Utilities/MemoryUtils.hpp
)

target_include_directories(
        ${MODULE}
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Public"
)

target_compile_definitions(
        Core PUBLIC
        $<$<CONFIG:Debug>:OGGLE_DEBUG>
        OGGLE_DEBUG_ENABLED=$<IF:$<CONFIG:Debug>,1,0>
)

add_library(Oggle::${MODULE} ALIAS ${MODULE})

#if(OGGLE_BUILD_TESTS)
    add_subdirectory(Tests)
#endif()