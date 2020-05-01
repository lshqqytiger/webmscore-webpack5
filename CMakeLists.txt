
project(mscore)

# Minimum tested, report if it works with older
CMAKE_MINIMUM_REQUIRED(VERSION 3.3.0)

include(build/functions.cmake) # library of CMake functions ("fn__" namespace)

set (CI $ENV{CI})
if (CI)
    SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif (CI)

# Libraries linked via full path no longer produce linker search paths.
cmake_policy(SET CMP0003 NEW)

# Apparently needed on Mac only (?)
if (APPLE)
    # Issue no warning non-existent target argument to get_target_property()
    # and set the result variable to a -NOTFOUND value rather than issuing a FATAL_ERROR 40
    if(POLICY CMP0045)
        cmake_policy(SET CMP0045 OLD)
    endif(POLICY CMP0045)

    # Silently ignore non-existent dependencies (mops1, mops2)
    if(POLICY CMP0046)
        cmake_policy(SET CMP0046 OLD)
    endif(POLICY CMP0046)
endif (APPLE)

# RPATH settings on macOS do not affect install_name
if(POLICY CMP0068)
      cmake_policy(SET CMP0068 NEW)
endif(POLICY CMP0068)

# Don't process generated source files with AUTOMOC
if(POLICY CMP0071)
      cmake_policy(SET CMP0071 OLD)
endif(POLICY CMP0071)

# Define MINGW for VS, as it appears not to be defined
if (MSVC)
   set (MINGW false)
endif (MSVC)


# Include modules
SET(QT_MIN_VERSION    "5.8.0")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build" ${CMAKE_MODULE_PATH})
include (FindQt5)
include (GetCompilerVersion)
include (CreatePrecompiledHeader)

# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_AUTOMOC     TRUE)

set(USE_SSE           FALSE)
SET (USE_WEBENGINE 0)

option(BUILD_PCH "Build using precompiled headers." ON)
option(USE_SYSTEM_FREETYPE "Use system FreeType" OFF)          # requires freetype >= 2.5.2, does not work on win


set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG   "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -DQT_NO_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Woverloaded-virtual")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DQT_NO_DEBUG_OUTPUT")

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON) # Call CMake with option -DCMAKE_SKIP_RPATH to not set RPATH (Debian packaging requirement)
set(CMAKE_SKIP_RULE_DEPENDENCY TRUE)


# The MuseScore version number.
SET(MUSESCORE_NAME "MuseScore")
SET(MUSESCORE_VERSION_MAJOR  "3")
SET(MUSESCORE_VERSION_MINOR  "4")
SET(MUSESCORE_VERSION_PATCH  "2")
SET(MUSESCORE_VERSION_LABEL  "Release")
SET(MUSESCORE_VERSION       "${MUSESCORE_VERSION_MAJOR}.${MUSESCORE_VERSION_MINOR}")
SET(MUSESCORE_NAME_VERSION "${MUSESCORE_NAME} ${MUSESCORE_VERSION_MAJOR}")
#
# Version schema x.x.x is hardcoded in source
#
SET(MUSESCORE_VERSION_FULL  "${MUSESCORE_VERSION}.${MUSESCORE_VERSION_PATCH}")


##
## freetype2 >= 2.5.2
##

if (USE_SYSTEM_FREETYPE)
      if (APPLE)
            PKGCONFIG (freetype2 2.5.2 FREETYPE_INCLUDE_DIRS FREETYPE_LIBDIR FREETYPE_LIBRARIES FREETYPE_CPP)
            if (FREETYPE_INCLUDE_DIRS)
                  STRING(REGEX REPLACE  "\"" "" FREETYPE_INCLUDE_DIRS ${FREETYPE_INCLUDE_DIRS})
                  STRING(REGEX REPLACE  "\"" "" FREETYPE_LIBDIR ${FREETYPE_LIBDIR})
                  message("freetype2 detected ${FREETYPE_INCLUDE_DIRS} ${FREETYPE_LIBDIR} ${FREETYPE_LIBRARIES}")
            else (FREETYPE_INCLUDE_DIRS)
                  message(FATAL_ERROR "freetype >= 2.5.2 is required\n")
            endif (FREETYPE_INCLUDE_DIRS)
      else (APPLE)
            find_package(Freetype REQUIRED)
      endif (APPLE)
endif (USE_SYSTEM_FREETYPE)


#
#  Create precompiled header file
#

# all.h is expected in PROJECT_BINARY_DIR by subdirs, except for MSVC
if (NOT MSVC)
   execute_process(
      COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/all.h ${PROJECT_BINARY_DIR}/all.h
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
      )
endif (NOT MSVC)

precompiled_header(QT_INCLUDES all ${BUILD_PCH})

# MSVC does not require these targets, as all.h is not copied and the
#   PCH generation is done per-project
if (NOT MSVC)
   ADD_CUSTOM_TARGET(mops1 DEPENDS ${PROJECT_BINARY_DIR}/all.h)
   ADD_CUSTOM_TARGET(mops2 DEPENDS ${PCH})
endif (NOT MSVC)


##

## Includes
##
include_directories(
   ${PROJECT_SOURCE_DIR}
   ${PROJECT_BINARY_DIR}
#    ${ALSA_INCDIR}
#    ${JACK_INCDIR}
#    ${PORTAUDIO_INCDIR}
#    ${OGG_INCDIR}
#    ${VORBIS_INCDIR}
#    ${SNDFILE_INCDIR}
#    ${LAME_INCLUDE_DIR}
)

if (USE_SYSTEM_FREETYPE)
      include_directories(${FREETYPE_INCLUDE_DIRS})
else (USE_SYSTEM_FREETYPE)
      include_directories(${PROJECT_SOURCE_DIR}/thirdparty/freetype/include)
endif (USE_SYSTEM_FREETYPE)

# add_subdirectory(global)
# add_subdirectory(main)

##
## produce config.h file
##
configure_file (
      ${PROJECT_SOURCE_DIR}/build/config.h.in
      ${PROJECT_BINARY_DIR}/config.h
      )

##
## Add subdirs
##
# add_subdirectory(effects)
# add_subdirectory(synthesizer)

subdirs(
      libmscore
      thirdparty/qzip
      )

if (NOT USE_SYSTEM_FREETYPE)
      subdirs (thirdparty/freetype)
endif (NOT USE_SYSTEM_FREETYPE)
