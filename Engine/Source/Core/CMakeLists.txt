cmake_minimum_required(VERSION 3.15)
set(MODULE Core)

add_subdirectory(Tests)

add_library(${MODULE}
        Private/ModuleInterface.cpp
        Private/Math/Color.cpp
        Private/Math/ColorConversionTables.cpp
        Private/Utilities/Assert.cpp
        Public/Core/Utilities/MemoryUtils.hpp
        Private/Logging/Logging.cpp
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
target_compile_options(Core PUBLIC -fno-rtti -fno-exceptions)
target_link_libraries(Core PUBLIC stdc++exp)

target_compile_definitions(Core PUBLIC
        $<$<CONFIG:Debug>:OGGLE_ENABLE_ASSERTS>
        $<IF:$<CONFIG:Debug>,OGGLE_ASSERTS_ENABLED=1,OGGLE_ASSERTS_ENABLED=0>
        $<$<NOT:$<CONFIG:Release>>:OGGLE_ENABLE_LOGGING>
)