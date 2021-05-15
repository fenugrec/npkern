# cmake tool chain for crosscompiling on a linux host.
# This may be fragile; frankenstein'd from a few different projects

set(TOOLCHAIN_BASE sh-elf)

# where is the target environment
set (CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_BASE})

set(TOOLCHAIN gcc-sh-none-elf)
find_path(
    TOOLCHAIN_BIN_DIR 
    sh-elf-gcc
    HINTS
        $ENV{HOME}/bin/${TOOLCHAIN}/bin
        $ENV{HOME}/opt/${TOOLCHAIN}/bin
        /opt/${TOOLCHAIN}/bin
        /srv/${TOOLCHAIN}/bin
		/usr/local/${TOOLCHAIN}/bin
		/usr/${TOOLCHAIN}/bin
	ENV TOOLCHAIN_BIN_DIR
)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR sh)

set(CMAKE_C_COMPILER "${TOOLCHAIN_BIN_DIR}/${TOOLCHAIN_BASE}-gcc" CACHE INTERNAL "")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_BIN_DIR}/${TOOLCHAIN_BASE}-g++" CACHE INTERNAL "")
set(CMAKE_EXE_LINKER_FLAGS "" CACHE INTERNAL "")

set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set (CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

SET (CMAKE_C_COMPILER_WORKS 1)
SET (CMAKE_CXX_COMPILER_WORKS 1)