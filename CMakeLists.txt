cmake_minimum_required (VERSION 2.8.11)

set(JAS_PROJECT_NAME "JasPer")

project("${JAS_PROJECT_NAME}")

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/build/cmake/modules/")

# This include should be placed as early as possible.
include(InSourceBuild)

################################################################################
# Version information.
################################################################################

# The major, minor, and micro version numbers of the project.
set(JAS_VERSION_MAJOR 2)
set(JAS_VERSION_MINOR 0)
set(JAS_VERSION_PATCH 0)

# The project version.
set(JAS_VERSION
	"${JAS_VERSION_MAJOR}.${JAS_VERSION_MINOR}.${JAS_VERSION_PATCH}")

# The RPM version information.
set(JAS_RPM_RELEASE 1)

# The shared library versioning information, which is specified by the
# following variables: JAS_SO_VERSION, JAS_SO_MINOR, and JAS_SO_RELEASE.
# Each new software release should update the values of these variables.
#
# Guidelines for updating this information:
# If the code did not change (e.g., only documentation was updated), do
# nothing.
# If the code changed and the binary interface for the library did not change
# from the previous release (e.g., most bug fixes), increment JAS_SO_RELEASE.
# If the binary interface changed, but remains compatible with the previous
# release (e.g., only new functions were added), increment JAS_SO_MINOR and
# set JAS_SO_RELEASE to 0.
# If the binary interface changed in a way that breaks compatibility with the
# previous release (e.g., a function was deleted), increment JAS_SO_VERSION and
# set both JAS_SO_MINOR and JAS_SO_RELEASE to 0.
#
# History of shared library versioning information:
# JasPer 2.0.0: 4.0.0

set(JAS_SO_VERSION 4)
set(JAS_SO_MINOR 0)
set(JAS_SO_RELEASE 0)
set(JAS_SO_NAME "${JAS_SO_VERSION}.${JAS_SO_MINOR}.${JAS_SO_RELEASE}")

################################################################################

set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE on)

include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CTest)
include(Sanitizers)
cmake_policy(SET CMP0012 NEW)

option(JAS_ENABLE_LIBJPEG "Enable use of JPEG Library" true)
option(JAS_ENABLE_OPENGL "Enable use of OpenGL Library" true)
option(JAS_ENABLE_STRICT "Enable pedantic error checking" false)
option(JAS_ENABLE_SHARED "Enable building of shared library" true)

if (APPLE AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(MACOSX true)
endif()

if (UNIX)
	if (MACOSX)
		set(JAS_PLATFORM "UNIX (OSX)")
	else()
		set(JAS_PLATFORM "UNIX (Not OSX)")
	endif()
elseif (WIN32)
	set(JAS_PLATFORM "Microsoft Windows")
else()
	set(JAS_PLATFORM "Unknown")
endif()
message("Platform ${JAS_PLATFORM}")

if (WIN32)
	if (JAS_ENABLE_SHARED)
		message(WARNING
		  "Disabling the building of shared libraries "
		  "(since this does not currently work on this platform). "
		  "Setting JAS_ENABLE_SHARED to false."
		)
		set(JAS_ENABLE_SHARED false)
	endif()
endif()

if (JAS_ENABLE_SHARED AND MACOSX)
	set(CMAKE_MACOSX_RPATH true)
endif()

message("CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")
message("CMAKE_C_COMPILER_ID ${CMAKE_C_COMPILER_ID}")
message("CMAKE_C_COMPILER ${CMAKE_C_COMPILER}")
message("CMAKE_C_FLAGS ${CMAKE_C_FLAGS}")
if (${CMAKE_BUILD_TYPE} MATCHES Debug)
	if (${CMAKE_C_COMPILER_ID} MATCHES GNU OR ${CMAKE_C_COMPILER_ID} MATCHES Clang)
		add_definitions("-O0")
	endif()
endif()

INCLUDE(InstallRequiredSystemLibraries)
SET(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
SET(CPACK_PACKAGE_VERSION "${JAS_VERSION}")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "JasPer Image Processing Tool Kit")
SET(CPACK_PACKAGE_VENDOR "Michael Adams")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_VERSION_MAJOR "${JAS_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${JAS_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${JAS_VERSION_PATCH}")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY
  "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
SET(CPACK_PACKAGE_FILE_NAME
  "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
SET(CPACK_GENERATOR "TGZ")
include(CPack)

find_package(Doxygen)

find_program(BASH_PROGRAM bash)
# HACK to disable tests
#set(BASH_PROGRAM "")

check_include_files(fcntl.h JAS_HAVE_FCNTL_H)
check_include_files(io.h JAS_HAVE_IO_H)
check_include_files(unistd.h JAS_HAVE_UNISTD_H)
check_include_files(windows.h JAS_HAVE_WINDOWS_H)
check_include_files(sys/time.h JAS_HAVE_SYS_TIME_H)
check_include_files(sys/types.h JAS_HAVE_SYS_TYPES_H)

check_function_exists(gettimeofday JAS_HAVE_GETTIMEOFDAY)
check_function_exists(getrusage JAS_HAVE_GETRUSAGE)

if (JAS_STRICT)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unused")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wformat -Wmissing-prototypes -Wstrict-prototypes")
endif()

find_package(JPEG)
if (JPEG_FOUND)
	include_directories("${JPEG_INCLUDE_DIR}")
endif()
#check_include_files(jpeglib.h JAS_HAVE_JPEGLIB_H)
# HACK
set(JAS_HAVE_JPEGLIB_H 1)
message("JPEG library found: ${JPEG_FOUND}")
message("JPEG libraries: ${JPEG_LIBRARIES}")
message("JPEG include directory: ${JPEG_INCLUDE_DIR}")
message("JAS_ENABLE_LIBJPEG: ${JAS_ENABLE_LIBJPEG}")
message("JAS_HAVE_JPEGLIB_H: ${JAS_HAVE_JPEGLIB_H}")
if(JPEG_FOUND AND (NOT JAS_ENABLE_LIBJPEG OR NOT JAS_HAVE_JPEGLIB_H))
	message("disabling LIBJPEG")
	set(JPEG_FOUND false)
	set(JPEG_LIBRARIES "")
	set(JPEG_INCLUDE_DIR "")
endif()
# HACK
#if (MACOSX)
#	message("disabling LIBJPEG")
#	set(JPEG_FOUND false)
#	set(JPEG_LIBRARIES "")
#	set(JPEG_INCLUDE_DIR "")
#endif()

find_package(GLUT)
if(GLUT_FOUND)
	include_directories("${GLUT_INCLUDE_DIR}")
endif()
check_include_files(GL/glut.h JAS_HAVE_GL_GLUT_H)
message("GLUT library found: ${GLUT_FOUND}")
message("GLUT libraries: ${GLUT_LIBRARIES}")
message("GLUT include directory: ${GLUT_INCLUDE_DIR}")
message("JAS_ENABLE_OPENGL: ${JAS_ENABLE_OPENGL}")
message("JAS_HAVE_GL_GLUT_H: ${JAS_HAVE_GL_GLUT_H}")
if(GLUT_FOUND AND (NOT JAS_HAVE_GL_GLUT_H OR NOT JAS_ENABLE_OPENGL))
	message("disabling GLUT")
	set(GLUT_FOUND false)
	set(GLUT_LIBRARIES "")
	set(GLUT_INCLUDE_DIR "")
endif()

if(UNIX)
	set(MATH_LIBRARY m)
else()
	set(MATH_LIBRARY "")
endif()

if (JAS_MEMORY_LIMIT)
	add_definitions(-DJAS_MEMORY_LIMIT=${JAS_MEMORY_LIMIT})
endif()

################################################################################

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

################################################################################

subdirs(src/libjasper src/appl doc)

#include_directories(${CMAKE_CURRENT_BINARY_DIR}/src/libjasper/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/libjasper/include)
#target_include_directories(libjasper PUBLIC 
#"${CMAKE_CURRENT_BINARY_DIR}/src/libjasper/include"
#"${CMAKE_CURRENT_SOURCE_DIR}/src/libjasper/include")

set(ENV{JAS_ABS_TOP_BUILDDIR} "${CMAKE_CURRENT_BINARY_DIR}")
set(ENV{JAS_ABS_TOP_SRCDIR} "${CMAKE_CURRENT_SOURCE_DIR}")
set(ENV{JAS_TOP_BUILDDIR} "${CMAKE_CURRENT_BINARY_DIR}")
set(ENV{JAS_TOP_SRCDIR} "${CMAKE_CURRENT_SOURCE_DIR}")
#	"JAS_TOP_BUILDDIR=../../build"
#	"JAS_TOP_SRCDIR=../.."

if (BASH_PROGRAM)
	add_test(run_test_1
	  "${BASH_PROGRAM}" "${CMAKE_CURRENT_SOURCE_DIR}/test/bin/wrapper"
	  "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
	  "${CMAKE_CURRENT_SOURCE_DIR}/test/bin/run_test_1")
	add_test(run_test_2
	  "${BASH_PROGRAM}" "${CMAKE_CURRENT_SOURCE_DIR}/test/bin/wrapper"
	  "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
	  "${CMAKE_CURRENT_SOURCE_DIR}/test/bin/run_test_2")
	add_test(run_test_3
	  "${BASH_PROGRAM}" "${CMAKE_CURRENT_SOURCE_DIR}/test/bin/wrapper"
	  "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
	  "${CMAKE_CURRENT_SOURCE_DIR}/test/bin/run_test_3")
	add_test(run_test_4
	  "${BASH_PROGRAM}" "${CMAKE_CURRENT_SOURCE_DIR}/test/bin/wrapper"
	  "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
	  "${CMAKE_CURRENT_SOURCE_DIR}/test/bin/run_test_4")
endif()

#find_package(MakeDist)

#set(EXTRA_DIST data)

#add_makedist(
#	${CMAKE_CURRENT_BINARY_DIR}/README
#	${CMAKE_CURRENT_BINARY_DIR}/LICENSE
#)

#file(GENERATE OUTPUT ChangeLog2 CONTENT echo)
execute_process(COMMAND git log OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/ChangeLog2)

# The package configuation (pc) file should be installed in
# ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig.
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build/jasper.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/build/jasper.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/build/jasper.pc
  DESTINATION lib/pkgconfig)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build/jasper.spec.in
  ${CMAKE_CURRENT_BINARY_DIR}/build/jasper.spec @ONLY)

