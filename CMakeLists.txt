
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
option(EMBED_PRELOADS "Embed preload files in the .js file, otherwise pack into a separate .data file." OFF)
option(SOUNDFONT3    "Ogg Vorbis compressed fonts" ON)         # Enable Ogg Vorbis compressed fonts, requires Ogg & Vorbis
option(HAS_AUDIOFILE "Enable audio export" ON)                 # Requires libsndfile


set(CMAKE_EXECUTABLE_SUFFIX ".lib.js")

set(WASM_LINK_FLAGS         "${WASM_LINK_FLAGS} --bind")
set(WASM_LINK_FLAGS         "${WASM_LINK_FLAGS} --source-map-base ./")
# set(WASM_LINK_FLAGS         "${WASM_LINK_FLAGS} -s WASM=0 -s SINGLE_FILE=1")
set(WASM_LINK_FLAGS         "${WASM_LINK_FLAGS} -s WASM=1")
set(WASM_LINK_FLAGS         "${WASM_LINK_FLAGS} -s EXPORT_ES6=1 -s USE_ES6_IMPORT_META=0 -s MODULARIZE=1")
set(WASM_LINK_FLAGS         "${WASM_LINK_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
# set(WASM_LINK_FLAGS         "${WASM_LINK_FLAGS} -s ASSERTIONS=1")
# set(WASM_LINK_FLAGS         "${WASM_LINK_FLAGS} -s EXPORTED_FUNCTIONS='[\"_test\"]'")  # use EMSCRIPTEN_KEEPALIVE instead
set(WASM_LINK_FLAGS         "${WASM_LINK_FLAGS} -s EXTRA_EXPORTED_RUNTIME_METHODS='[\"ccall\", \"cwrap\", \"stringToUTF8\", \"UTF8ToString\", \"getValue\", \"setValue\"]'")

set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -std=gnu++11 -fsigned-char")
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-override")
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wno-deprecated")
# set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wno-deprecated-copy")
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -s USE_ZLIB=1")      # 1 = use zlib from emscripten-ports
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -s USE_FREETYPE=1")  # 1 = use freetype from emscripten-ports
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -s USE_VORBIS=1")    # 1 = use vorbis from emscripten-ports
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -s USE_OGG=1")       # 1 = use ogg from emscripten-ports
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -s DEMANGLE_SUPPORT=1")

set(CMAKE_CXX_FLAGS_DEBUG   "-g3 -s ASSERTIONS=2 -s STACK_OVERFLOW_CHECK=2 -s SAFE_HEAP=1")
set(CMAKE_CXX_FLAGS_RELEASE "-Oz -DNDEBUG -DQT_NO_DEBUG")
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


## Set preload/embedded files
##
if (EMBED_PRELOADS)
    SET (PRELOAD_TYPE_FLAG "--embed-file")
else (EMBED_PRELOADS)
    SET (PRELOAD_TYPE_FLAG "--preload-file")
endif (EMBED_PRELOADS)

set(_preload_files
    fonts/musejazz/MuseJazzText.otf
    fonts/campania/Campania.otf
    fonts/mscoreTab.ttf
    fonts/mscore-BC.ttf
    fonts/bravura/BravuraText.otf
    fonts/gootville/GootvilleText.otf
    fonts/mscore/MScoreText.ttf

    fonts/bravura/Bravura.otf
    fonts/bravura/metadata.json
    
    fonts/mscore/mscore.ttf
    fonts/mscore/metadata.json

    fonts/gootville/Gootville.otf
    fonts/gootville/metadata.json

    fonts/musejazz/MuseJazz.otf
    fonts/musejazz/metadata.json

    fonts/smufl/glyphnames.json
    fonts/fonts_tablature.xml
    fonts/fonts_figuredbass.xml

    fonts/FreeSans.ttf
    fonts/FreeSerif.ttf
    fonts/FreeSerifBold.ttf
    fonts/FreeSerifItalic.ttf
    fonts/FreeSerifBoldItalic.ttf
)
foreach(_file ${_preload_files})
    set(WASM_LINK_FLAGS "${WASM_LINK_FLAGS} ${PRELOAD_TYPE_FLAG} ${PROJECT_SOURCE_DIR}/${_file}@/${_file}")
endforeach()

set(WASM_LINK_FLAGS "${WASM_LINK_FLAGS} ${PRELOAD_TYPE_FLAG} ${PROJECT_SOURCE_DIR}/share/styles@/styles")


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
      midi
      synthesizer
      effects
      fluid
      audiofile
      thirdparty/qzip
      thirdparty/libsndfile/src
      )

