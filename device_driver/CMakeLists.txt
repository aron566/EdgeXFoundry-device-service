aux_source_directory(. DIR_SRCS)
include_directories(.)

string(REGEX REPLACE ".*/(.*)" "\\1" LIB_NAME ${CMAKE_CURRENT_SOURCE_DIR}) 

if (DIR_SRCS)
    foreach(libname ${LIBNAMES})

        if (${LIB_NAME} STREQUAL ${libname})

            add_library(${libname} SHARED ${DIR_SRCS})

            target_link_libraries(${libname} "service_component")
        endif()
    endforeach()

else()
    message(WARNING "not find is src file!")
endif()
