
# MSVC does not depend on copying all.h to the binary directory
if (NOT MSVC)
   add_custom_command(
      OUTPUT ${PROJECT_BINARY_DIR}/all.h
      COMMAND ${CMAKE_COMMAND}
      ARGS -E copy ${PROJECT_SOURCE_DIR}/all.h ${PROJECT_BINARY_DIR}/all.h
      DEPENDS ${PROJECT_SOURCE_DIR}/all.h
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
      )
endif (NOT MSVC)


if (NOT MSVC)
   set(_all_h_file "${PROJECT_BINARY_DIR}/all.h")
else (NOT MSVC)
   set(_all_h_file "${PROJECT_SOURCE_DIR}/all.h")
endif (NOT MSVC)

add_executable(
        webmscore 
        ${_all_h_file}
        main.cpp
)

target_link_libraries(webmscore libmscore)
target_link_libraries(webmscore ${QT_LIBRARIES})

set_target_properties (
    webmscore
    PROPERTIES
        COMPILE_FLAGS "${PCH_INCLUDE}"
)
