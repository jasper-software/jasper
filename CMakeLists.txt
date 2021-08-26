cmake_minimum_required(VERSION 3.12)

project(JasPer C)

################################################################################
# Specify software and shared library version information.
################################################################################

# The major, minor, and micro version numbers of the project.
set(JAS_VERSION_MAJOR 3)
set(JAS_VERSION_MINOR 0)
set(JAS_VERSION_PATCH 0)

# The shared library versioning information.
# Guidelines on how to change this information can be found below.
set(JAS_SO_VERSION 7)
set(JAS_SO_MINOR 0)
set(JAS_SO_RELEASE 0)

# Guidelines for Updating the Shared Library Versioning Information
# =================================================================
#
# If the code did not change (e.g., only documentation was updated):
#     do not change the shared library versioning information.
# If the code changed and the binary interface for the library did not change
# from the previous release (e.g., most bug fixes):
#     increment JAS_SO_RELEASE.
# If the binary interface changed, but remains compatible with the previous
# release (e.g., only new functions were added):
#     - increment JAS_SO_MINOR; and
#     - set JAS_SO_RELEASE to 0.
# If the binary interface changed in a way that breaks compatibility with the
# previous release (e.g., a function was deleted):
#     - increment JAS_SO_VERSION; and
#     - set both JAS_SO_MINOR and JAS_SO_RELEASE to 0.

# Generate the project version.
set(JAS_VERSION
	"${JAS_VERSION_MAJOR}.${JAS_VERSION_MINOR}.${JAS_VERSION_PATCH}")

# Generate the shared library build version.
set(JAS_SO_NAME "${JAS_SO_VERSION}.${JAS_SO_MINOR}.${JAS_SO_RELEASE}")

message("Software version: ${JAS_VERSION}")
message("Shared library ABI version: ${JAS_SO_VERSION}")
message("Shared library build version: ${JAS_SO_NAME}")

################################################################################
# Include modules and set policies.
################################################################################

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/build/cmake/modules/")

# This include should be placed as early as possible.
include(InSourceBuild)

include(CheckCCompilerFlag)

include(EnableAssertions)

# Adhere to GNU filesystem layout conventions.
include(GNUInstallDirs)

include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckCSourceCompiles)
include(CTest)
include(Sanitizers)
include(GenerateExportHeader)

cmake_policy(SET CMP0012 NEW)

################################################################################
# Define options.
################################################################################

option(JAS_ENABLE_SHARED "Enable building of shared library" true)
option(JAS_ENABLE_HIDDEN "Hide internal symbols" true)
option(JAS_ENABLE_32BIT "Use 32 bit integers on 64 bit CPUs" false)
option(JAS_ENABLE_LIBJPEG "Enable the use of the JPEG Library" true)
option(JAS_ENABLE_OPENGL "Enable the use of the OpenGL/GLUT Library" true)
option(JAS_ENABLE_AUTOMATIC_DEPENDENCIES "Enable automatic dependencies" true)
option(JAS_ENABLE_DOC "Enable building of the documentation" true)
option(JAS_ENABLE_PROGRAMS "Enable building of the programs" true)
option(JAS_ENABLE_MULTITHREADING_SUPPORT "Enable multithreading support" true)
option(JAS_PREFER_PTHREAD
  "Prefer POSIX Threading Library (over C11) for threading support" false)

option(JAS_INCLUDE_BMP_CODEC "Include BMP codec support" true)
option(JAS_INCLUDE_JP2_CODEC "Include JP2 codec support" true)
option(JAS_INCLUDE_JPC_CODEC "Include JPC codec support" true)
option(JAS_INCLUDE_JPG_CODEC "Include JPG codec support" true)
option(JAS_INCLUDE_MIF_CODEC "Include MIF codec support" true)
option(JAS_INCLUDE_PGX_CODEC "Include PGX codec support" true)
option(JAS_INCLUDE_PNM_CODEC "Include PNM codec support" true)
option(JAS_INCLUDE_RAS_CODEC "Include RAS codec support" true)

set(JAS_ENABLE_DEFAULT_MAX_MEM_USAGE
  "1073741824"
  CACHE STRING
  "Set memory usage limit in bytes"
)

# The following options are for testing/debugging and should normally only
# be changed by the developers of JasPer.
option(JAS_USE_JAS_INIT
  "Use jas_init instead of jas_init_custom for initialization (should be false, except for testing)"
  false)
option(JAS_ENABLE_NON_THREAD_SAFE_DEBUGGING
  "Enable non-thread safe debug code" false)

if (JAS_USE_JAS_INIT)
	add_definitions(-DJAS_USE_JAS_INIT)
endif()

# WARNING: THE FOLLOWING OPTION SHOULD ONLY BE ENABLED BY THE JASPER SOFTWARE
# DEVELOPMENT TEAM.  IT IS NECESSARY FOR INTERNAL TESTING PURPOSES AND POSES
# SIGNIFICANT SECURITY RISKS IF ENABLED.
option(JAS_ENABLE_DANGEROUS_INTERNAL_TESTING_MODE
  "DO NOT ENABLE FOR SECURITY REASONS! (For internal use by JasPer development team only"
  false)

################################################################################
#
################################################################################

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

# Prefer C11 (if available) but do not require it.
set(CMAKE_C_STANDARD 11)

if (APPLE AND CMAKE_SYSTEM_NAME MATCHES "Darwin")
	set(MACOS true)
else()
	set(MACOS false)
endif()

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(LINUX true)
else()
	set(LINUX false)
endif()

message("Linux: ${LINUX}")
message("MacOS: ${MACOS}")

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

if (JAS_ENABLE_SHARED AND MACOS)
	set(CMAKE_MACOSX_RPATH true)
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

message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message("CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message("CMAKE_LINKER: ${CMAKE_LINKER}")
message("CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message("CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}")
message("CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG}")

# Customize the compiler flags for warning messages.
if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL GNU)
	check_c_compiler_flag("-Wno-stringop-truncation"
	  JAS_GCC_WARN_NO_STRINGOP_TRUNCATION)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-sign-compare")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-prototypes")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstrict-prototypes")
	if (JAS_GCC_WARN_NO_STRINGOP_TRUNCATION)
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-stringop-truncation")
	endif()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-declarations")
endif()

if (JAS_ENABLE_HIDDEN)
	set(CMAKE_C_VISIBILITY_PRESET hidden)
	set(CMAKE_VISIBILITY_INLINES_HIDDEN 0)
	if (NOT (CMAKE_BUILD_TYPE MATCHES "Debug"))
		if (LINUX)
			check_c_compiler_flag("-ffunction-sections" HAVE_FUNCTION_SECTIONS)
			check_c_compiler_flag("-fdata-sections" HAVE_DATA_SECTIONS)
			check_c_compiler_flag("-Wl,--gc-sections" HAVE_GC_SECTIONS)
			if (HAVE_FUNCTION_SECTIONS AND HAVE_DATA_SECTIONS AND
			 HAVE_GC_SECTIONS)
				set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections")
				set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdata-sections")
				set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--gc-sections")
			endif()
		endif()
	endif()
endif()

################################################################################
# Check for the OpenGL and GLUT libraries.
################################################################################

if (JAS_ENABLE_OPENGL)
	set(OpenGL_GL_PREFERENCE GLVND)
	find_package(OpenGL ${JAS_REQUIRED})
	message("OpenGL library found: ${OPENGL_FOUND}")
else()
	set(OPENGL_FOUND false)
endif()
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

################################################################################
# Perform platform checks.
################################################################################

find_program(BASH_PROGRAM bash)

check_include_files(threads.h JAS_HAVE_THREADS_H)
check_include_files(fcntl.h JAS_HAVE_FCNTL_H)
check_include_files(io.h JAS_HAVE_IO_H)
check_include_files(unistd.h JAS_HAVE_UNISTD_H)
check_include_files(sys/time.h JAS_HAVE_SYS_TIME_H)
check_include_files(sys/types.h JAS_HAVE_SYS_TYPES_H)

check_function_exists(gettimeofday JAS_HAVE_GETTIMEOFDAY)
check_function_exists(getrusage JAS_HAVE_GETRUSAGE)
check_function_exists(mkostemp JAS_HAVE_MKOSTEMP)

check_c_source_compiles("
#include <stddef.h>
max_align_t x;
int main() {}
" JAS_HAVE_MAX_ALIGN_T)

################################################################################
# Check for invalid codec combinations.
################################################################################

if (JAS_INCLUDE_JP2_CODEC AND NOT JAS_INCLUDE_JPC_CODEC)
	message(FATAL_ERROR
	  "The JPC codec must be included in order to include the JP2 codec.")
endif()

################################################################################
# Configure threading support
################################################################################

if (JAS_ENABLE_MULTITHREADING_SUPPORT)

	check_c_source_compiles("
	#include <threads.h>
	int main() {
		thrd_t thread;
		mtx_t mutex;
		tss_t tss;
	}
	" JAS_TRY_COMPILE_C11_THREADS)
	check_include_files(pthread.h JAS_HAVE_PTHREAD_H)

	# Check for support for various threading libraries.
	if (MACOS)
		set(THREADS_PREFER_PTHREAD_FLAG true)
		set(CMAKE_HAVE_THREADS_LIBRARY 1)
		set(CMAKE_THREAD_LIBS_INIT "-lpthread")
		set(CMAKE_USE_WIN32_THREADS_INIT 0)
		set(CMAKE_USE_PTHREADS_INIT 1)
		set(Threads_FOUND 1)
	else()
		set(THREADS_PREFER_PTHREAD_FLAG true)
		find_package(Threads)
	endif()
	if (Threads_FOUND)
		if (CMAKE_USE_PTHREADS_INIT AND JAS_HAVE_PTHREAD_H)
			set(JAS_HAVE_PTHREAD_THREADS true)
		else()
			set(JAS_HAVE_PTHREAD_THREADS false)
		endif()
		if (CMAKE_USE_WIN32_THREADS_INIT)
			set(JAS_HAVE_WIN32_THREADS true)
		else()
			set(JAS_HAVE_WIN32_THREADS false)
		endif()
		set(THREAD_LIBRARY "${CMAKE_THREAD_LIBS_INIT}")
		if (JAS_TRY_COMPILE_C11_THREADS)
			set(JAS_HAVE_C11_THREADS true)
		else()
			set(JAS_HAVE_C11_THREADS false)
		endif()
	endif()

	# Apply preferences to select one particular threading library.
	set(JAS_THREADS false)
	set(JAS_THREADS_C11 false)
	set(JAS_THREADS_PTHREAD false)
	set(JAS_THREADS_WIN32 false)
	if (JAS_HAVE_PTHREAD_THREADS AND JAS_PREFER_PTHREAD)
		set(JAS_THREADS_PTHREAD true)
	elseif (JAS_HAVE_C11_THREADS)
		set(JAS_THREADS_C11 true)
	elseif (JAS_HAVE_WIN32_THREADS)
		set(JAS_THREADS_WIN32 true)
	endif()
	if (JAS_THREADS_C11 OR JAS_THREADS_PTHREAD OR JAS_THREADS_WIN32)
		set(JAS_THREADS true)
	else()
		message(ERROR "No threading support found.")
	endif()

	message("Prefer POSIX threading support: ${JAS_PREFER_PTHREAD}")
	message("Threading support found: ${JAS_THREADS}")
	message("C11 threading support detected: ${JAS_HAVE_C11_THREADS}")
	message("POSIX threading support detected: ${JAS_HAVE_POSIX_THREADS}")
	message("WIN32 threading support detected: ${JAS_HAVE_WIN32_THREADS}")
	message("C11 threading support selected: ${JAS_THREADS_C11}")
	message("POSIX threading support selected: ${JAS_THREADS_PTHREAD}")
	message("Win32 threading support selected: ${JAS_THREADS_WIN32}")
	message("Threading library: ${THREAD_LIBRARY}")

else()

	set(JAS_THREADS false)
	set(JAS_THREADS_C11 false)
	set(JAS_THREADS_PTHREAD false)
	set(JAS_THREADS_WIN32 false)

	message("Threading support has been disabled.")

endif()

################################################################################
# Check for the JPEG library.
################################################################################

# The following is needed to prevent the incorrect version of the JPEG library
# from being found.
if (MACOS)
	set(CMAKE_FIND_FRAMEWORK LAST)
endif()

if (JAS_ENABLE_LIBJPEG)
	find_package(JPEG ${JAS_LIBJPEG_REQUIRED})
	message("JPEG library found: ${JPEG_FOUND}")
else()
	set(JPEG_FOUND false)
endif()

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
if (NOT JAS_HAVE_LIBJPEG)
	set(JAS_INCLUDE_JPG_CODEC 0)
endif()

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

#add_definitions("-D_GNU_SOURCE")

################################################################################
# Perform shared library setup.
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
# Miscellany.
################################################################################

add_subdirectory(src/libjasper)
if (JAS_ENABLE_PROGRAMS)
    add_subdirectory(src/appl)
endif ()
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
