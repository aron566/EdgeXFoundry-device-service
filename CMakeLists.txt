cmake_minimum_required(VERSION 3.4.1)

project(custom-device VERSION 1.0.0 LANGUAGES C)

set(TARGETS "custom-device")
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} "/home/aron566/Workspace/C_SDK/device-sdk-c/include")
set(LIB_DIRS "/home/aron566/Workspace/C_SDK/device-sdk-c/build/release/_CPack_Packages/Linux/TGZ/csdk-1.3.0/lib")
set(LIBNAMES "csdk")

set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -O0 -g -ggdb")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3 -DNDEBUG")

link_directories(${LIB_DIRS})

include_directories(${INCLUDE_DIRS})

aux_source_directory(. DIR_SRCS)

add_executable(${TARGETS} ${DIR_SRCS})

if (${CMAKE_VERSION} VERSION_EQUAL "3.4.1" OR
    ${CMAKE_VERSION} VERSION_GREATER "3.4.1")

    set_target_properties(${TARGETS} PROPERTIES C_STANDARD 99)
else()
    # Remove this fallback when removing support for CMake version less than 3.4.1
    target_compile_options(${TARGETS} PRIVATE
                           "$<$<C_COMPILER_ID:AppleClang>:-std=c99>"
                           "$<$<C_COMPILER_ID:Clang>:-std=c99>"
                           "$<$<C_COMPILER_ID:GNU>:-std=c99>")
endif()

target_link_libraries(${TARGETS} PRIVATE ${LIBNAMES})