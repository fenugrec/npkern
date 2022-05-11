# TODO:
# - generate .lst and .map files, and stack-usage
# - 'install' .bin files to $sourcedir/precompiled

cmake_minimum_required(VERSION 3.0)

set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "Default install prefix")
set (TEMP_INSTPREFIX ${CMAKE_INSTALL_PREFIX})
message("Using install prefix : ${CMAKE_INSTALL_PREFIX}")

project(npkern C ASM)

#check if install prefix was clobbered
if ( NOT(${CMAKE_INSTALL_PREFIX} STREQUAL ${TEMP_INSTPREFIX}))
	message(WARNING "Default install prefix has been corrupted ! please report this.")
	#note : if this happens, the alternative is something like
	#if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	# set(installprefix .etc... FORCE)
	#endif()
	#placed after project().
endif ()

###### add some CFLAGS

set(CPU_FLAGS "-mb -m2")
set(OPT_FLAGS "-Os -ffunction-sections")

# removed -pedantic from cflags because we have conversions from u32 to function-pointers which are "problematic"
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPU_FLAGS} ${OPT_FLAGS} -Wall -Wextra -Wformat -std=gnu11 \
				-Wstrict-prototypes -Wsign-compare -Wredundant-decls -ffreestanding" )
set(ASM_OPTIONS "-x assembler-with-cpp")
set(CMAKE_ASM_FLAGS "${CFLAGS} ${ASM_OPTIONS}" )

add_link_options(-nostartfiles -Wl,-gc-sections)


# Add a custom command that produces version.c, plus
# a dummy output that's not actually produced, in order
# to force version.cmake to always be re-run before the build

add_custom_target(version_h BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/version.h"
	COMMAND ${CMAKE_COMMAND}
		-D SRCDIR="${CMAKE_CURRENT_SOURCE_DIR}"
		-P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/gitversion.cmake
)

set (COMMON_SRCS cmd_parser.c eep_funcs.c main.c crc.c
	${CMAKE_CURRENT_BINARY_DIR}/version.h
	)

set (NISSAN_SRCS ${COMMON_SRCS} start_705x.s)
set (SUBARU_SRCS ${COMMON_SRCS} start_ssm.s)

## objcopy to produce .bin file
function(make_bin_file target)
    add_custom_command(
        TARGET ${target} POST_BUILD
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        BYPRODUCTS ${target}.bin
        COMMAND ${CMAKE_OBJCOPY} -O binary ${target} ${target}.bin
    )
endfunction()

## report size
function(show_object_size target)
    string(REPLACE "objcopy" "size" CMAKE_OBJSIZE "${CMAKE_OBJCOPY}")
    add_custom_command(
        TARGET ${target} POST_BUILD
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_OBJSIZE} ${target}
    )
endfunction()


## add a target
function(add_kernel brand TGTNAME)
	message(STATUS ${brand}_${TGTNAME})
	target_compile_definitions(${brand}_${TGTNAME} PRIVATE PLATF=\"${TGTNAME}\")
	target_compile_definitions(${brand}_${TGTNAME} PRIVATE ${TGTNAME})
	target_include_directories(${brand}_${TGTNAME} PUBLIC ${PROJECT_BINARY_DIR})
	make_bin_file(${brand}_${TGTNAME})
	show_object_size(${brand}_${TGTNAME})
endfunction()


#can't really automate the targets because sourcefiles and linkerscripts vary

add_executable(npk_SH7051 ${NISSAN_SRCS} platf_7050.c pl_flash_7051.c)
add_executable(npk_SH7055_35 ${NISSAN_SRCS} platf_7055.c pl_flash_7055_350nm.c)
add_executable(npk_SH7055_18 ${NISSAN_SRCS} platf_7055.c pl_flash_705x_180nm.c)
add_executable(npk_SH7058 ${NISSAN_SRCS} platf_7055.c pl_flash_705x_180nm.c)
add_executable(ssmk_SH7058 ${SUBARU_SRCS} platf_7055.c pl_flash_705x_180nm.c)


# Select proper linker script
target_link_options(npk_SH7051 PRIVATE -T ${CMAKE_SOURCE_DIR}/ldscripts/lkr_7051.ld)
target_link_options(npk_SH7055_35 PRIVATE -T ${CMAKE_SOURCE_DIR}/ldscripts/lkr_7055_7058.ld)
target_link_options(npk_SH7055_18 PRIVATE -T ${CMAKE_SOURCE_DIR}/ldscripts/lkr_7055_7058.ld)
target_link_options(npk_SH7058 PRIVATE -T ${CMAKE_SOURCE_DIR}/ldscripts/lkr_7055_7058.ld)
target_link_options(ssmk_SH7058 PRIVATE -T ${CMAKE_SOURCE_DIR}/ldscripts/lkr_subaru_7058.ld)


## Add all Nissan targets here
set(TGT_LIST "SH7051" "SH7055_35" "SH7055_18" "SH7058")

foreach (TGTNAME IN LISTS TGT_LIST)
	add_kernel(npk ${TGTNAME})
endforeach()

## Add Subaru kernel target

add_kernel(ssmk SH7058)
