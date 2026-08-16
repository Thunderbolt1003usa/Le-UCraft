get_filename_component(
    UCRAFT_ZLIB_SOURCE_DIR
    "${CMAKE_CURRENT_LIST_DIR}/../../zlib"
    ABSOLUTE
)

if(TARGET zlib)
    return()
endif()

if(NOT EXISTS "${UCRAFT_ZLIB_SOURCE_DIR}/deflate.c")
    message(STATUS "zlib sources not found at ${UCRAFT_ZLIB_SOURCE_DIR}. Cloning from git")
    find_package(Git REQUIRED)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive 3rdparty/zlib
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../../..
    )
endif()

set(UCRAFT_ZLIB_DEFLATE_SOURCES
    "${UCRAFT_ZLIB_SOURCE_DIR}/adler32.c"
    "${UCRAFT_ZLIB_SOURCE_DIR}/compress.c"
    "${UCRAFT_ZLIB_SOURCE_DIR}/crc32.c"
    "${UCRAFT_ZLIB_SOURCE_DIR}/deflate.c"
    "${UCRAFT_ZLIB_SOURCE_DIR}/trees.c"
    "${UCRAFT_ZLIB_SOURCE_DIR}/zutil.c"
)

add_library(zlib STATIC ${UCRAFT_ZLIB_DEFLATE_SOURCES})
add_library(ZLIB::ZLIB ALIAS zlib)

target_include_directories(zlib PUBLIC "${UCRAFT_ZLIB_SOURCE_DIR}")
target_compile_definitions(zlib PRIVATE ZLIB_BUILD)
set_target_properties(zlib PROPERTIES LINKER_LANGUAGE C)
