get_filename_component(
    UCRAFT_LWJSON_SOURCE_DIR
    "${CMAKE_CURRENT_LIST_DIR}/../../lwjson/lwjson"
    ABSOLUTE
)

if(TARGET lwjson)
    return()
endif()

if(NOT EXISTS "${UCRAFT_LWJSON_SOURCE_DIR}/src/lwjson/lwjson.c")
    message(STATUS "lwjson sources not found at ${UCRAFT_LWJSON_SOURCE_DIR}. Cloning from git")
    find_package(Git REQUIRED)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive 3rdparty/lwjson
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

set(UCRAFT_LWJSON_SOURCES
    "${UCRAFT_LWJSON_SOURCE_DIR}/src/lwjson/lwjson.c"
    "${UCRAFT_LWJSON_SOURCE_DIR}/src/lwjson/lwjson_stream.c"
)

add_library(lwjson STATIC ${UCRAFT_LWJSON_SOURCES})
add_library(LWJSON::LWJSON ALIAS lwjson)

target_include_directories(
    lwjson
    PUBLIC "${UCRAFT_LWJSON_SOURCE_DIR}/src/include"
)
target_compile_definitions(lwjson PUBLIC LWJSON_IGNORE_USER_OPTS)
set_target_properties(lwjson PROPERTIES LINKER_LANGUAGE C)
