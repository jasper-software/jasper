cmake_minimum_required (VERSION 2.8.11)

project(JasPer LANGUAGES C)

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
set(JAS_VERSION_PATCH 12)

# The project version.
set(JAS_VERSION
	"${JAS_VERSION_MAJOR}.${JAS_VERSION_MINOR}.${JAS_VERSION_PATCH}")

message("Software version: ${JAS_VERSION}")

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

if (APPLE AND CMAKE_SYSTEM_NAME MATCHES "Darwin")
	set(MACOSX true)
else()
	set(MACOSX false)
endif()

# To fix a problem with OSX, increase JAS_SO_VERSION to 6 (instead of 5)
# when it is next increased.
set(JAS_SO_VERSION 4)
set(JAS_SO_MINOR 0)
set(JAS_SO_RELEASE 0)
# This is a temporary hack for OSX that should be removed when JAS_SO_VERSION
# is next incremented.
if (MACOSX)
set(JAS_SO_NAME "5.${JAS_SO_MINOR}.${JAS_SO_RELEASE}")
else()
set(JAS_SO_NAME "${JAS_SO_VERSION}.${JAS_SO_MINOR}.${JAS_SO_RELEASE}")
endif()

message("Shared library ABI version: ${JAS_SO_VERSION}")
message("Shared library build version: ${JAS_SO_NAME}")

################################################################################
# Include modules and set policies.
################################################################################

# Adhere to GNU filesystem layout conventions.
include(GNUInstallDirs)

include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckCSourceCompiles)
include(CTest)
include(Sanitizers)
include(EnableAssertions)

cmake_policy(SET CMP0012 NEW)

################################################################################
# Define options.
################################################################################

option(JAS_ENABLE_SHARED "Enable building of shared library" true)
option(JAS_ENABLE_LIBJPEG "Enable the use of the JPEG Library" true)
option(JAS_ENABLE_OPENGL "Enable the use of the OpenGL/GLUT Library" true)
option(JAS_ENABLE_STRICT "Enable pedantic error checking" false)
option(JAS_ENABLE_AUTOMATIC_DEPENDENCIES "Enable automatic dependencies" true)
option(JAS_LOCAL "Enable local hacks for developers (do not enable)" false)
option(JAS_ENABLE_DOC "Enable building of the documentation" true)

################################################################################
#
################################################################################

#set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_C_STANDARD 11)

if ((CMAKE_GENERATOR MATCHES Xcode) OR
  (CMAKE_GENERATOR MATCHES "Visual Studio"))
	set(JAS_MULTICONFIGURATION_GENERATOR 1)
else()
	set(JAS_MULTICONFIGURATION_GENERATOR 0)
endif()
message("JAS_MULTICONFIGURATION_GENERATOR ${JAS_MULTICONFIGURATION_GENERATOR}")

if (JAS_ENABLE_AUTOMATIC_DEPENDENCIES)
	message(WARNING
	  "If this software is being built as a package for a Linux distribution, "
	  "you should probably set JAS_ENABLE_AUTOMATIC_DEPENDENCIES to false.")
	set(JAS_REQUIRED "")
else()
	set(JAS_REQUIRED "REQUIRED")
endif()

# The following lines have been temporarily moved above:
#if (APPLE AND CMAKE_SYSTEM_NAME MATCHES "Darwin")
#	set(MACOSX true)
#else()
#	set(MACOSX false)
#endif()

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

if (JAS_ENABLE_SHARED AND MACOSX)
	set(CMAKE_MACOSX_RPATH true)
endif()

if (JAS_ENABLE_SHARED)
	set(JAS_DLL 1)
endif()

message("JAS_LOCAL ${JAS_LOCAL}")
message("CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")
message("CMAKE_C_COMPILER_ID ${CMAKE_C_COMPILER_ID}")
message("CMAKE_C_COMPILER ${CMAKE_C_COMPILER}")
message("CMAKE_C_FLAGS ${CMAKE_C_FLAGS}")
if (JAS_LOCAL AND (CMAKE_BUILD_TYPE MATCHES "Debug"))
	if (CMAKE_C_COMPILER_ID MATCHES "GNU" OR
	  CMAKE_C_COMPILER_ID MATCHES "Clang")
		add_definitions("-O0")
	endif()
endif()

# If a multiconfiguration generator is used, ensure that various output
# files are not placed in subdirectories (such as Debug and Release)
# as this will cause the CTest test suite to fail.
if (JAS_MULTICONFIGURATION_GENERATOR)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY .)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY .)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY .)
	foreach (config ${CMAKE_CONFIGURATION_TYPES})
		string(TOUPPER "${config}" config)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} .)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config} .)
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config} .)
	endforeach()
endif()

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${JAS_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "JasPer Image Processing Tool Kit")
set(CPACK_PACKAGE_VENDOR "Michael Adams")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${JAS_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${JAS_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${JAS_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY
  "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
set(CPACK_PACKAGE_FILE_NAME
  "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_GENERATOR "TGZ")
include(CPack)

if (JAS_STRICT)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unused")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wformat -Wmissing-prototypes -Wstrict-prototypes")
endif()

################################################################################
# Perform plaform checks.
################################################################################

find_package(Doxygen)
find_package(LATEX COMPONENTS PDFLATEX)
find_program(BASH_PROGRAM bash)

# On some (or maybe all?) systems, LATEX_FOUND is not set by FindLATEX.
# So, instead, rely on LATEX_PDFLATEX_FOUND.
message("PDFLATEX_COMPILER: ${PDFLATEX_COMPILER}")
if ((NOT LATEX_FOUND) AND PDFLATEX_COMPILER)
	message(WARNING "Setting LATEX_FOUND to true.")
	message(WARNING "Your version of CMake may be buggy.")
	set(LATEX_FOUND true)
endif()
message("LATEX_FOUND ${LATEX_FOUND}")

check_include_files(fcntl.h JAS_HAVE_FCNTL_H)
check_include_files(io.h JAS_HAVE_IO_H)
check_include_files(unistd.h JAS_HAVE_UNISTD_H)
check_include_files(windows.h JAS_HAVE_WINDOWS_H)
check_include_files(sys/time.h JAS_HAVE_SYS_TIME_H)
check_include_files(sys/types.h JAS_HAVE_SYS_TYPES_H)

check_function_exists(gettimeofday JAS_HAVE_GETTIMEOFDAY)
check_function_exists(getrusage JAS_HAVE_GETRUSAGE)

################################################################################
# Check for the JPEG library.
################################################################################

find_package(JPEG ${JAS_LIBJPEG_REQUIRED})
message("JPEG library found: ${JPEG_FOUND}")
if (JAS_ENABLE_LIBJPEG AND JPEG_FOUND)
	set(JAS_HAVE_LIBJPEG 0)
	message("JPEG include directory: ${JPEG_INCLUDE_DIR}")
	message("JPEG libraries: ${JPEG_LIBRARIES}")
	# In some versions of the JPEG library, the header file jpeglib.h
	# does not include some of the header files upon which it depends
	# (e.g., stdio.h and stdint.h).  So, we cannot reliably use
	# check_include_file here.
	set(CMAKE_REQUIRED_INCLUDES ${JPEG_INCLUDE_DIR})
	check_c_source_compiles("
		#include <stdio.h>
		#include <stdint.h>
		#include <jpeglib.h>
		int main() {}
	" JAS_HAVE_JPEGLIB_H)
	message("JAS_HAVE_JPEGLIB_H: ${JAS_HAVE_JPEGLIB_H}")
	if(JAS_HAVE_JPEGLIB_H)
		set(JAS_HAVE_LIBJPEG 1)
		include_directories(${JPEG_INCLUDE_DIR})
	else()
		message(WARNING "The header file jpeglib.h appears to be missing.")
		message(WARNING "Disabling LIBJPEG.")
		set(JPEG_FOUND false)
		set(JPEG_LIBRARIES "")
		set(JPEG_INCLUDE_DIR "")
		set(JAS_ENABLE_LIBJPEG 0)
	endif()
else()
	set(JAS_HAVE_LIBJPEG 0)
	set(JPEG_INCLUDE_DIR "")
	set(JPEG_LIBRARIES "")
endif()
message("JAS_HAVE_LIBJPEG: ${JAS_HAVE_LIBJPEG}")

################################################################################
# Check for the OpenGL and GLUT libraries.
################################################################################

find_package(OpenGL ${JAS_REQUIRED})
message("JAS_ENABLE_OPENGL: ${JAS_ENABLE_OPENGL}")
message("OpenGL library found: ${OPENGL_FOUND}")
if (JAS_ENABLE_OPENGL AND OPENGL_FOUND)
	set(JAS_HAVE_OPENGL 0)
	message("OpenGL include directory: ${OPENGL_INCLUDE_DIR}")
	message("OpenGL libraries: ${OPENGL_LIBRARIES}")
	find_package(GLUT ${JAS_REQUIRED})
	message("GLUT library found: ${GLUT_FOUND}")
	if (GLUT_FOUND)
		message("GLUT include directory: ${GLUT_INCLUDE_DIR}")
		message("GLUT libraries: ${GLUT_LIBRARIES}")
		set(CMAKE_REQUIRED_INCLUDES ${GLUT_INCLUDE_DIR})
		check_include_files(GL/glut.h JAS_HAVE_GL_GLUT_H)
		check_include_files(glut.h JAS_HAVE_GLUT_H)
		message("JAS_HAVE_GLUT_H: ${JAS_HAVE_GLUT_H}")
		message("JAS_HAVE_GL_GLUT_H: ${JAS_HAVE_GL_GLUT_H}")
		if (JAS_HAVE_GL_GLUT_H OR JAS_HAVE_GLUT_H)
			set(JAS_HAVE_OPENGL 1)
			include_directories(${GLUT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
		else()
			message(WARNING "The header files GL/glut.h and glut.h both appear to be missing.")
			message(WARNING "Disabling OpenGL.")
		endif()
	endif()
	# On some systems (e.g., Fedora 21), there is a bug in the cmake code
	# that detects GLUT libraries.  The following ugliness is a workaround for 
	# this problem.
	if (NOT GLUT_Xmu_LIBRARY OR NOT GLUT_Xmi_LIBRARY)
		if (NOT GLUT_Xmu_LIBRARY)
			set(GLUT_Xmu_LIBRARY "")
			message(WARNING "Clearing bogus value for GLUT_Xmu_LIBRARY.")
			message(WARNING "Your version of CMake may be buggy.")
		endif()
		if (NOT GLUT_Xmi_LIBRARY)
			set(GLUT_Xmi_LIBRARY "")
			message(WARNING "Clearing bogus value for GLUT_Xmi_LIBRARY.")
			message(WARNING "Your version of CMake may be buggy.")
		endif()
		set(GLUT_LIBRARIES "${GLUT_glut_LIBRARY}")
	endif()
else()
	set(JAS_HAVE_OPENGL 0)
	set(OPENGL_INCLUDE_DIR "")
	set(OPENGL_LIBRARIES "")
	set(GLUT_INCLUDE_DIR "")
	set(GLUT_LIBRARIES "")
endif()
message("JAS_HAVE_OPENGL: ${JAS_HAVE_OPENGL}")

################################################################################
# Check for the Math library.
################################################################################

find_library(MATH_LIBRARY m)
if (NOT MATH_LIBRARY)
	set(MATH_LIBRARY "")
endif()

################################################################################
#
################################################################################

if (JAS_MEMORY_LIMIT)
	add_definitions("-DJAS_MEMORY_LIMIT=${JAS_MEMORY_LIMIT}")
endif()

################################################################################
#
################################################################################

if (JAS_ENABLE_SHARED)

	# use, i.e. don't skip the full RPATH for the build tree
	set(CMAKE_SKIP_BUILD_RPATH FALSE)

	# when building, don't use the install RPATH already
	# (but later on when installing)
	set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

	set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

	# add the automatically determined parts of the RPATH
	# which point to directories outside the build tree to the install RPATH
	set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

	# The RPATH to be used when installing, but only if it's not a
	# system directory
	list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
	  "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
	if("${isSystemDir}" STREQUAL "-1")
	   set(CMAKE_INSTALL_RPATH
		  "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
	endif("${isSystemDir}" STREQUAL "-1")

endif()

################################################################################
#
################################################################################

add_subdirectory(src/libjasper)
add_subdirectory(src/appl)
if (JAS_ENABLE_DOC)
	add_subdirectory(doc)
endif ()

# The package configuation (pc) file should be installed in
# ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig.
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/build/jasper.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/build/jasper.pc" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/build/jasper.pc"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

install(FILES "README" DESTINATION "${CMAKE_INSTALL_DOCDIR}")

################################################################################
# Test suite
################################################################################

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/test/bin/wrapper.in"
  "${CMAKE_CURRENT_BINARY_DIR}/test/bin/wrapper" @ONLY)

if (BASH_PROGRAM)
	add_test(run_test_1
	  "${BASH_PROGRAM}" "${CMAKE_CURRENT_BINARY_DIR}/test/bin/wrapper"
	  "${CMAKE_CURRENT_SOURCE_DIR}/test/bin/run_test_1")
	add_test(run_test_2
	  "${BASH_PROGRAM}" "${CMAKE_CURRENT_BINARY_DIR}/test/bin/wrapper"
	  "${CMAKE_CURRENT_SOURCE_DIR}/test/bin/run_test_2")
	add_test(run_test_3
	  "${BASH_PROGRAM}" "${CMAKE_CURRENT_BINARY_DIR}/test/bin/wrapper"
	  "${CMAKE_CURRENT_SOURCE_DIR}/test/bin/run_test_3")
	add_test(run_test_4
	  "${BASH_PROGRAM}" "${CMAKE_CURRENT_BINARY_DIR}/test/bin/wrapper"
	  "${CMAKE_CURRENT_SOURCE_DIR}/test/bin/run_test_4")
endif()
