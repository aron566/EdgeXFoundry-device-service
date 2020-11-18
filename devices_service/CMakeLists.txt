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

foreach(findlib ${APP_LINK_LIBS})
    target_link_libraries(${TARGETS} PRIVATE ${findlib})
endforeach()