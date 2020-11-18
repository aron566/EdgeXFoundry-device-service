cmake_minimum_required(VERSION 3.4.1)

project(custom-device VERSION 1.0.0 LANGUAGES C)

set(TARGETS "custom-device")
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} "/home/aron566/Workspace/C_SDK/device-sdk-c/include")
set(LIB_DIRS "/home/aron566/Workspace/C_SDK/device-sdk-c/build/release/_CPack_Packages/Linux/TGZ/csdk-1.3.0/lib")
set(LIBNAMES "csdk" "service_component" "device_driver")
set(APP_LINK_LIBS "csdk" "device_driver")
set(SUBDIRS "service_component" "devices_service" "device_driver")

set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -O0 -g -ggdb")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3 -DNDEBUG")

link_directories(${LIB_DIRS})

include_directories(${INCLUDE_DIRS})

foreach(subdir ${SUBDIRS})
    add_subdirectory(${subdir})
endforeach()