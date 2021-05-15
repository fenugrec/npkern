# source: simplified from
# https://www.mattkeeter.com/blog/2018-01-06-versioning/

execute_process(COMMAND git log --pretty=format:'%h' -n 1
				OUTPUT_VARIABLE GIT_REV
				WORKING_DIRECTORY ${SRCDIR}
                ERROR_QUIET)

# count # of commits
execute_process(COMMAND git rev-list --count HEAD
				OUTPUT_VARIABLE GIT_NCOMMITS
				WORKING_DIRECTORY ${SRCDIR}
                ERROR_QUIET)

# Check whether we got any revision (which isn't
# always the case, e.g. when someone downloaded a zip
# file from Github instead of a checkout)
if ("${GIT_REV}" STREQUAL "")
    set(GIT_REV "N/A")
    set(GIT_NCOMMITS "x")
else()
    string(STRIP "${GIT_REV}" GIT_REV)
    string(SUBSTRING "${GIT_REV}" 1 7 GIT_REV)
endif()

string(STRIP "${GIT_NCOMMITS}" GIT_NCOMMITS)

set(VERSION "#define NPK_COMMIT \"${GIT_NCOMMITS}-${GIT_REV}\"\n")

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/version.h)
    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/version.h VERSION_)
else()
    set(VERSION_ "")
endif()

if (NOT "${VERSION}" STREQUAL "${VERSION_}")
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/version.h "${VERSION}")
endif()
