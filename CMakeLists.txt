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
				-Wstrict-prototypes -Wsign-compare -Wredundant-decls" )
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

set (COMMON_SRCS cmd_parser.c eep_funcs.c main.c crc.c start_705x.s
	${CMAKE_CURRENT_BINARY_DIR}/version.h
	)


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


#cant really automate the targets because the name and sourcefile and linkerscripts vary

add_executable(npk_SH7051 ${COMMON_SRCS} platf_7050.c pl_flash_7051.c)
add_executable(npk_SH7055_35 ${COMMON_SRCS} platf_7055.c pl_flash_7055_350nm.c)
add_executable(npk_SH7055_18 ${COMMON_SRCS} platf_7055.c pl_flash_705x_180nm.c)
add_executable(npk_SH7058 ${COMMON_SRCS} platf_7055.c pl_flash_705x_180nm.c)


# So far, only 7051 needs a different linker script
target_link_options(npk_SH7051 PRIVATE -T ${CMAKE_SOURCE_DIR}/lkr_7051.ld)
target_link_options(npk_SH7055_35 PRIVATE -T ${CMAKE_SOURCE_DIR}/lkr_7055_7058.ld)
target_link_options(npk_SH7055_18 PRIVATE -T ${CMAKE_SOURCE_DIR}/lkr_7055_7058.ld)
target_link_options(npk_SH7058 PRIVATE -T ${CMAKE_SOURCE_DIR}/lkr_7055_7058.ld)


set(TGT_LIST "SH7051" "SH7055_35" "SH7055_18" "SH7058")

## common operations

foreach (TGTNAME IN LISTS TGT_LIST)
	message(STATUS ${TGTNAME})
	target_compile_definitions(npk_${TGTNAME} PRIVATE PLATF=\"${TGTNAME}\")
	target_compile_definitions(npk_${TGTNAME} PRIVATE ${TGTNAME})
	target_include_directories(npk_${TGTNAME} PUBLIC ${PROJECT_BINARY_DIR})
	make_bin_file(npk_${TGTNAME})
	show_object_size(npk_${TGTNAME})
endforeach()


