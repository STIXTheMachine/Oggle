set(APPNAME Scratch)

file(
    GLOB_RECURSE
    SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/*.cpp
)

add_executable(
    ${APPNAME}
    ${SOURCES}
)

target_link_libraries(${APPNAME} PRIVATE Oggle)