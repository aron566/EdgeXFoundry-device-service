set(SUB_DIRS "3third_party/libuv")
set(DIR_SRCS "." "3third_party/tomlc99" "3third_party/libyaml")

foreach(srcdir ${DIR_SRCS})
    aux_source_directory(${srcdir} SRC_LIST)
endforeach()

foreach(dir ${SUB_DIRS})
    add_subdirectory(${dir})
endforeach()

string(REGEX REPLACE ".*/(.*)" "\\1" LIB_NAME ${CMAKE_CURRENT_SOURCE_DIR}) 

if (DIR_SRCS)
    foreach(libname ${LIBNAMES})

        if (${LIB_NAME} STREQUAL ${libname})

            add_library(${libname} STATIC ${SRC_LIST})

            target_link_libraries(${libname} "csdk" "yaml" "uv_a")
        endif()
    endforeach()

else()
    message(WARNING "not find is src file!")
endif()
