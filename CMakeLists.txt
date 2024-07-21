cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

if(POLICY CMP0092)
    cmake_policy(SET CMP0092 NEW) # Don't add -W3 warning level by default.
endif()


project(cs2-plugify VERSION 1.0.0.0 DESCRIPTION "Plugify CS2 Core" HOMEPAGE_URL "https://github.com/untrustedmodders/cs2-plugify" LANGUAGES C CXX ASM)

include("cmake/shared.cmake")

set(PLUGIFY_PROJECT_VERSION "0" CACHE STRING "Set version name")

# We target plugify as object to have exported methods of plugify
set(PLUGIFY_BUILD_OBJECT_LIB ON CACHE INTERNAL "")
if(LINUX)
    set(PLUGIFY_USE_STATIC_STDLIB ON CACHE INTERNAL "")
endif()
add_subdirectory(external/plugify)

set(SOURCE_FILES
    src/mm_plugin.cpp
    src/mm_plugin.h
    src/mm_logger.cpp
    src/mm_logger.h
    external/hl2sdk-cs2/tier1/convar.cpp
)

if(LINUX)
    # memoverride.cpp is not usable on CMake Windows, because CMake default link libraries always link ucrt.lib
    set(SOURCE_FILES
        ${SOURCE_FILES}
        external/hl2sdk-cs2/public/tier0/memoverride.cpp
    )
endif()

set(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")

## Generate protobuf source & headers
if(LINUX)
    set(PROTOC_EXECUTABLE ${EXTERNAL_DIR}/hl2sdk-cs2/devtools/bin/linux/protoc)
elseif(WIN32)
    set(PROTOC_EXECUTABLE ${EXTERNAL_DIR}/hl2sdk-cs2/devtools/bin/protoc.exe)
endif()

add_custom_command(
        OUTPUT protobuf_output_stamp
        COMMAND ${PROTOC_EXECUTABLE} --proto_path=thirdparty/protobuf-3.21.8/src --proto_path=common --cpp_out=common common/network_connection.proto
        COMMENT "Generating protobuf file"
        WORKING_DIRECTORY ${EXTERNAL_DIR}/hl2sdk-cs2
        VERBATIM
)

SET(SOURCE_FILES ${SOURCE_FILES} protobuf_output_stamp)

# Logger
set(LOGGER_DIR "${EXTERNAL_DIR}/logger")
set(LOGGER_BINARY_DIR "logger")
set(LOGGER_INCLUDE_DIR "${LOGGER_DIR}/include")
add_subdirectory(${LOGGER_DIR} ${LOGGER_BINARY_DIR})

# Sources
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${NATIVES_SOURCES} ${CONVERSIONS_SOURCES} ${CONVERSIONS_HEADERS})

target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/cs2_sdk
        ${CMAKE_BINARY_DIR}/exports

        ${LOGGER_INCLUDE_DIR}
)

if(LINUX)
    include("cmake/linux.cmake")
elseif(WIN32)
    include("cmake/windows.cmake")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "plugify")

target_compile_definitions(${PROJECT_NAME} PRIVATE
        PLUGIFY_PROJECT_NAME="${PROJECT_NAME}"
        PLUGIFY_PROJECT_DESCRIPTION="${CMAKE_PROJECT_DESCRIPTION}"
        PLUGIFY_PROJECT_HOMEPAGE_URL="${CMAKE_PROJECT_HOMEPAGE_URL}"
        PLUGIFY_PROJECT_VERSION="${PLUGIFY_PROJECT_VERSION}")

# Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${PLUGIFY_LINK_LIBRARIES} ${LOGGER_BINARY_DIR})

if(APPLE)
    target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-exported_symbols_list,${CMAKE_CURRENT_SOURCE_DIR}/sym/exported_symbols.lds")
elseif(UNIX)
    target_link_options(${PROJECT_NAME} PRIVATE "-Wl,--version-script,${CMAKE_CURRENT_SOURCE_DIR}/sym/version_script.lds")
endif()

if(LINUX)
    set(PLUGIFY_PREFER_OWN_SYMBOLS "true")
else()
    set(PLUGIFY_PREFER_OWN_SYMBOLS "false")
endif()
configure_file(
    ${CMAKE_SOURCE_DIR}/plugify.pconfig.in
    ${CMAKE_BINARY_DIR}/plugify.pconfig
)