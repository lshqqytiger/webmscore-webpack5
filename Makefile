#=============================================================================
#  Mscore
#  Linux Music Score Editor
#
#  Copyright (C) 2002-2012 by Werner Schweer and others
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License version 2.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#=============================================================================

REVISION  := `cat mscore/revision.h`
CPUS      := $(shell getconf _NPROCESSORS_ONLN 2>/dev/null || getconf NPROCESSORS_ONLN 2>/dev/null || echo 1)

PREFIX    = "/usr/local"
#VERSION  = "3.4b-${REVISION}"
VERSION   = 3.4.2
BUILD_NUMBER=""

# Override SUFFIX and LABEL when multiple versions are installed to avoid conflicts.
SUFFIX=""# E.g.: SUFFIX="dev" --> "mscore" becomes "mscoredev"
LABEL=""# E.g.: LABEL="Development Build" --> "MuseScore 2" becomes "MuseScore 2 Development Build"

NO_RPATH="FALSE"# Package maintainers may want to override this (e.g. Debian)
# EMBED_PRELOADS="ON"

#
# change path to include your Qt5 installation
#
BINPATH      = ${PATH}
PREFIX_PATH = /usr/qt515/qtbase

release:
	if test ! -d build.release; then mkdir build.release; fi; \
      cd build.release;                                        \
      export PATH=${BINPATH};                                   \
	  export CMAKE_PREFIX_PATH=${PREFIX_PATH};                   \
	  export NODE_OPTIONS=--max_old_space_size=4096;              \
      emcmake cmake -DCMAKE_BUILD_TYPE=RELEASE	                   \
	  -DEMBED_PRELOADS="${EMBED_PRELOADS}"                          \
  	  -DCMAKE_TOOLCHAIN_FILE="${CMAKE_TOOLCHAIN_FILE}"               \
  	  -DCMAKE_INSTALL_PREFIX="${PREFIX}"                              \
  	  -DCMAKE_BUILD_NUMBER="${BUILD_NUMBER}"                           \
  	  -DCMAKE_SKIP_RPATH="${NO_RPATH}"     ..;                          \
      emmake make -j ${CPUS};                                            \
	  mv ./libmscore/webmscore.* ../web-public;                           \

debug:
	if test ! -d build.debug; then mkdir build.debug; fi; \
      cd build.debug;                                      \
      export PATH=${BINPATH};                               \
	  export CMAKE_PREFIX_PATH=${PREFIX_PATH};               \
	  export NODE_OPTIONS=--max_old_space_size=4096;          \
      emcmake cmake -DCMAKE_BUILD_TYPE=DEBUG	               \
	  -DEMBED_PRELOADS="${EMBED_PRELOADS}"                      \
  	  -DCMAKE_TOOLCHAIN_FILE="${CMAKE_TOOLCHAIN_FILE}"           \
  	  -DCMAKE_INSTALL_PREFIX="${PREFIX}"                          \
  	  -DCMAKE_BUILD_NUMBER="${BUILD_NUMBER}"                       \
  	  -DCMAKE_SKIP_RPATH="${NO_RPATH}"     ..;                      \
      emmake make -j ${CPUS};                                        \
	  mv ./libmscore/webmscore.* ../web-public;                       \

#
# clean out of source build
#
clean:
	-rm -rf build.debug build.release
	-rm -rf build.wasm build.js
	-rm -rf win32build win32install
	-rm -rf web-public/.cache
	-rm web-public/webmscore.*

revision:
	@git rev-parse --short=7 HEAD > mscore/revision.h

version:
	@echo ${VERSION}


