file(
    GLOB_RECURSE
    SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(
    Scratch
    ${SOURCES}
)

target_link_libraries(Scratch PRIVATE Oggle::Application)