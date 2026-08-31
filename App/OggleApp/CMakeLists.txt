set(APPNAME OggleApp)

file(
    GLOB_RECURSE
    SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(
    ${APPNAME}
    ${SOURCES}
)

target_link_libraries(${APPNAME} PRIVATE Oggle)