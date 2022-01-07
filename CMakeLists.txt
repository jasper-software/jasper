cmake_minimum_required(VERSION 3.12)
# Version 3.12 needed for FindJPEG imported targets.
# Version 3.10 needed for FindOpenGL imported targets.
# Version 3.1 needed for FindGLUT imported targets.

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

project(JasPer VERSION ${JAS_VERSION} LANGUAGES C)

################################################################################
# Include modules and set policies.
################################################################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/build/cmake/modules)

# This include should be placed as early as possible.
include(InSourceBuild)

include(CheckCCompilerFlag)
include(EnableAssertions)

# Adhere to GNU filesystem layout conventions.
include(GNUInstallDirs)

include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckSymbolExists)
include(CheckTypeSize)
include(CheckCSourceCompiles)
include(CTest)
include(Sanitizers)
include(GenerateExportHeader)
include(JasGetIncludes)

################################################################################
# Define options.
################################################################################

if(DEFINED BUILD_SHARED_LIBS)
	option(JAS_ENABLE_SHARED "Enable building of shared library"
	  ${BUILD_SHARED_LIBS})
else()
	option(JAS_ENABLE_SHARED "Enable building of shared library" ON)
endif()
option(JAS_ENABLE_HIDDEN "Hide internal symbols" ON)
option(JAS_ENABLE_32BIT "Use 32 bit integers on 64 bit CPUs" OFF)
option(JAS_ENABLE_LIBJPEG "Enable the use of the JPEG Library" ON)
option(JAS_ENABLE_LIBHEIF "Enable the use of the HEIF Library" ON)
option(JAS_ENABLE_OPENGL "Enable the use of the OpenGL/GLUT Library" ON)
option(JAS_ENABLE_DOC "Enable building of the documentation" ON)
option(JAS_ENABLE_PROGRAMS "Enable building of the programs" ON)
option(JAS_ENABLE_MULTITHREADING_SUPPORT "Enable multithreading support" ON)
option(JAS_PREFER_PTHREAD
  "Prefer POSIX Threading Library (over C11) for threading support" ON)
option(JAS_STRICT "Treat compiler warnings as errors" OFF)

option(JAS_INCLUDE_BMP_CODEC "Include (native) BMP codec support" ON)
option(JAS_INCLUDE_JP2_CODEC "Include (native) JP2 codec support" ON)
option(JAS_INCLUDE_JPC_CODEC "Include (native) JPC codec support" ON)
option(JAS_INCLUDE_JPG_CODEC
  "Include JPG codec support (via IJG JPEG Library)" ON)
option(JAS_INCLUDE_HEIC_CODEC
	"Include experimental HEIC codec support (via Libheif Library)" ON)
option(JAS_INCLUDE_MIF_CODEC "Include (native) MIF codec support" ON)
option(JAS_INCLUDE_PGX_CODEC "Include (native) PGX codec support" ON)
option(JAS_INCLUDE_PNM_CODEC "Include (native) PNM codec support" ON)
option(JAS_INCLUDE_RAS_CODEC "Include (native) RAS codec support" ON)

set(JAS_DEFAULT_MAX_MEM_USAGE
  "1073741824"
  CACHE STRING
  "Set memory usage limit in bytes"
)

# The following options are for testing/debugging and should normally only
# be changed by the developers of JasPer.
option(JAS_USE_JAS_INIT
  "Use jas_init instead of jas_init_custom for initialization (should be false, except for testing)"
  OFF)
option(JAS_ENABLE_NON_THREAD_SAFE_DEBUGGING
  "Enable non-thread safe debug code" OFF)

if(JAS_USE_JAS_INIT)
	add_compile_definitions(JAS_USE_JAS_INIT)
endif()

# WARNING: THE FOLLOWING OPTION SHOULD ONLY BE ENABLED BY THE JASPER SOFTWARE
# DEVELOPMENT TEAM.  IT IS NECESSARY FOR INTERNAL TESTING PURPOSES AND POSES
# SIGNIFICANT SECURITY RISKS IF ENABLED.
option(JAS_ENABLE_DANGEROUS_INTERNAL_TESTING_MODE
  "DO NOT ENABLE FOR SECURITY REASONS! (For internal use by JasPer development team only"
  OFF)

################################################################################
#
################################################################################

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Prefer C11 (if available) but do not require it.
set(CMAKE_C_STANDARD 11)

if(APPLE AND CMAKE_SYSTEM_NAME MATCHES "Darwin")
	set(MACOS TRUE)
else()
	set(MACOS FALSE)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(LINUX TRUE)
else()
	set(LINUX FALSE)
endif()

if((CMAKE_GENERATOR MATCHES "Xcode") OR
  (CMAKE_GENERATOR MATCHES "Visual Studio") OR
  (CMAKE_GENERATOR STREQUAL "Ninja Multi-Config"))
	set(JAS_MULTICONFIGURATION_GENERATOR TRUE)
	message("JAS_MULTICONFIGURATION_GENERATOR ${JAS_MULTICONFIGURATION_GENERATOR}")
else()
	set(JAS_MULTICONFIGURATION_GENERATOR FALSE)
endif()
message("JAS_MULTICONFIGURATION_GENERATOR ${JAS_MULTICONFIGURATION_GENERATOR}")

if(JAS_ENABLE_SHARED AND MACOS)
	set(CMAKE_MACOSX_RPATH TRUE)
endif()

# If a multiconfiguration generator is used, ensure that various output
# files are not placed in subdirectories (such as Debug and Release)
# as this will cause the CTest test suite to fail.
if(JAS_MULTICONFIGURATION_GENERATOR)
	if(CMAKE_CONFIGURATION_TYPES)
		set(CMAKE_DEBUG_POSTFIX d)
	endif()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY .)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY .)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY .)
	foreach(config ${CMAKE_CONFIGURATION_TYPES})
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

if(CMAKE_BUILD_TYPE)
	message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
elseif(CMAKE_CONFIGURATION_TYPES)
	message("CMAKE_CONFIGURATION_TYPES: ${CMAKE_CONFIGURATION_TYPES}")
endif()
message("CMAKE_C_COMPILER_ID: ${CMAKE_C_COMPILER_ID}")
message("CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message("CMAKE_LINKER: ${CMAKE_LINKER}")

# Customize the compiler flags for warning messages.
if(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
	check_c_compiler_flag("-Wno-stringop-truncation"
	  JAS_GCC_WARN_NO_STRINGOP_TRUNCATION)
	add_compile_options(-Wall)
	add_compile_options(-pedantic)
	add_compile_options(-W)
	add_compile_options(-Wno-sign-compare)
	add_compile_options(-Wformat)
	add_compile_options(-Wmissing-prototypes)
	add_compile_options(-Wstrict-prototypes)
	if(JAS_GCC_WARN_NO_STRINGOP_TRUNCATION)
		add_compile_options(-Wno-stringop-truncation)
	endif()
	add_compile_options(-Wmissing-declarations)
	if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
		add_compile_options(-Wno-unused-command-line-argument)
	endif()
	if(JAS_STRICT)
		# Treat all compiler warnings as errors.
		add_compile_options(-pedantic-errors)
	endif()
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
	# possibly lossy conversion
	add_compile_options(/wd4244)
	# different const qualifiers
	add_compile_options(/wd4090)
	# signed/unsigned mismatch
	add_compile_options(/wd4018)
	# function may be unsafe (e.g., sprintf, strncat)
	add_compile_options(/wd4996)
	# allow some kinds of narrowing conversions
	add_compile_options(/wd4267)
	if(JAS_STRICT)
		# Treat all compiler warnings as errors.
		add_compile_options(/WX)
	endif()
endif()

if(JAS_ENABLE_HIDDEN)
	set(CMAKE_C_VISIBILITY_PRESET hidden)
	set(CMAKE_VISIBILITY_INLINES_HIDDEN 0)
	if(NOT (CMAKE_BUILD_TYPE MATCHES "Debug"))
		if(LINUX)
			check_c_compiler_flag("-ffunction-sections" HAVE_FUNCTION_SECTIONS)
			check_c_compiler_flag("-fdata-sections" HAVE_DATA_SECTIONS)
			check_c_compiler_flag("-Wl,--gc-sections" HAVE_GC_SECTIONS)
			if(HAVE_FUNCTION_SECTIONS AND HAVE_DATA_SECTIONS AND
			  HAVE_GC_SECTIONS)
				add_compile_options(-ffunction-sections)
				add_compile_options(-fdata-sections)
				add_compile_options(-Wl,--gc-sections)
			endif()
		endif()
	endif()
endif()

message("CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message("CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}")
message("CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG}")

################################################################################
# Perform platform checks.
################################################################################

find_program(BASH_PROGRAM bash)

check_include_files(fcntl.h JAS_HAVE_FCNTL_H)
check_include_files(io.h JAS_HAVE_IO_H)
check_include_files(unistd.h JAS_HAVE_UNISTD_H)
check_include_files(sys/time.h JAS_HAVE_SYS_TIME_H)
check_include_files(sys/types.h JAS_HAVE_SYS_TYPES_H)

check_type_size(max_align_t MAX_ALIGN_T)
if(HAVE_MAX_ALIGN_T)
	set(JAS_HAVE_MAX_ALIGN_T 1 CACHE INTERNAL "Test JAS_HAVE_MAX_ALIGN_T")
else()
	set(JAS_HAVE_MAX_ALIGN_T 0 CACHE INTERNAL "Test JAS_HAVE_MAX_ALIGN_T")
endif()
message("JAS_HAVE_MAX_ALIGN_T: ${JAS_HAVE_MAX_ALIGN_T}")

check_type_size(ssize_t SSIZE_T)
if(HAVE_SSIZE_T)
	set(JAS_HAVE_SSIZE_T 1 CACHE INTERNAL "Test JAS_HAVE_SSIZE_T")
else()
	set(JAS_HAVE_SSIZE_T 0 CACHE INTERNAL "Test JAS_HAVE_SSIZE_T")
endif()
message("JAS_HAVE_SSIZE_T: ${JAS_HAVE_SSIZE_T}")

check_type_size(uintmax_t UINTMAX_T)
if(HAVE_UINTMAX_T)
	set(JAS_HAVE_UINTMAX_T 1 CACHE INTERNAL "Test JAS_HAVE_UINTMAX_T")
else()
	set(JAS_HAVE_UINTMAX_T 0 CACHE INTERNAL "Test JAS_HAVE_UINTMAX_T")
endif()
message("JAS_HAVE_UINTMAX_T: ${JAS_HAVE_UINTMAX_T}")

check_type_size(intmax_t INTMAX_T)
if(HAVE_INTMAX_T)
	set(JAS_HAVE_INTMAX_T 1 CACHE INTERNAL "Test JAS_HAVE_INTMAX_T")
else()
	set(JAS_HAVE_INTMAX_T 0 CACHE INTERNAL "Test JAS_HAVE_INTMAX_T")
endif()
message("JAS_HAVE_INTMAX_T: ${JAS_HAVE_INTMAX_T}")

check_function_exists(gettimeofday JAS_HAVE_GETTIMEOFDAY)
check_function_exists(getrusage JAS_HAVE_GETRUSAGE)
check_function_exists(mkostemp JAS_HAVE_MKOSTEMP)
if(MSVC)
	check_symbol_exists(snprintf stdio.h JAS_HAVE_SNPRINTF)
else()
	check_function_exists(snprintf JAS_HAVE_SNPRINTF)
endif()

################################################################################
# Check for invalid codec combinations.
################################################################################

if (JAS_INCLUDE_BMP_CODEC AND
  JAS_INCLUDE_JP2_CODEC AND
  JAS_INCLUDE_JPC_CODEC AND
  JAS_INCLUDE_PGX_CODEC AND
  JAS_INCLUDE_PNM_CODEC AND
  JAS_INCLUDE_RAS_CODEC)
	message("Support for all native codecs has been included.")
	set(JAS_HAVE_ALL_NATIVE_CODECS 1)
else()
	message(WARNING
	  "Support for one or more native codecs has been excluded.  "
	  "The inclusion of support for all native codecs is "
	  "STRONGLY RECOMMENDED.")
	set(JAS_HAVE_ALL_NATIVE_CODECS 0)
endif()

if(JAS_INCLUDE_JP2_CODEC AND NOT JAS_INCLUDE_JPC_CODEC)
	message(FATAL_ERROR
	  "The JPC codec must be included in order to include the JP2 codec.")
endif()

################################################################################
# Configure threading support
################################################################################

set(JAS_THREADS FALSE)
set(JAS_THREADS_C11 FALSE)
set(JAS_THREADS_PTHREAD FALSE)
set(JAS_THREADS_WIN32 FALSE)
if(JAS_ENABLE_MULTITHREADING_SUPPORT)
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
	if(MACOS)
		set(THREADS_PREFER_PTHREAD_FLAG TRUE)
		set(CMAKE_HAVE_THREADS_LIBRARY 1)
		set(CMAKE_THREAD_LIBS_INIT "-lpthread")
		set(CMAKE_USE_WIN32_THREADS_INIT 0)
		set(CMAKE_USE_PTHREADS_INIT 1)
		set(Threads_FOUND TRUE)
	else()
		set(THREADS_PREFER_PTHREAD_FLAG TRUE)
		find_package(Threads)
	endif()
	if(Threads_FOUND)
		if(CMAKE_USE_PTHREADS_INIT AND JAS_HAVE_PTHREAD_H)
			set(JAS_HAVE_PTHREAD_THREADS TRUE)
		endif()
		if(CMAKE_USE_WIN32_THREADS_INIT)
			set(JAS_HAVE_WIN32_THREADS TRUE)
		endif()
		set(THREAD_LIBRARY "${CMAKE_THREAD_LIBS_INIT}")
		if(JAS_TRY_COMPILE_C11_THREADS)
			set(JAS_HAVE_C11_THREADS TRUE)
		endif()
	endif()

	# Apply preferences to select one particular threading library.
	if(JAS_HAVE_PTHREAD_THREADS AND JAS_PREFER_PTHREAD)
		set(JAS_THREADS_PTHREAD TRUE)
	elseif(JAS_HAVE_C11_THREADS)
		set(JAS_THREADS_C11 TRUE)
	elseif(JAS_HAVE_WIN32_THREADS)
		set(JAS_THREADS_WIN32 TRUE)
	elseif(JAS_HAVE_PTHREAD_THREADS)
		set(JAS_THREADS_PTHREAD TRUE)
	endif()
	if(JAS_THREADS_C11 OR JAS_THREADS_PTHREAD OR JAS_THREADS_WIN32)
		set(JAS_THREADS TRUE)
		message("Threading support enabled.")
	else()
		message(WARNING "No threading support found.")
	endif()

	message("Prefer POSIX threading support: ${JAS_PREFER_PTHREAD}")
	message("Threading support found: ${JAS_THREADS}")
	message("C11 threading support detected: ${JAS_HAVE_C11_THREADS}")
	message("POSIX threading support detected: ${JAS_HAVE_PTHREAD_THREADS}")
	message("WIN32 threading support detected: ${JAS_HAVE_WIN32_THREADS}")
	message("C11 threading support selected: ${JAS_THREADS_C11}")
	message("POSIX threading support selected: ${JAS_THREADS_PTHREAD}")
	message("Win32 threading support selected: ${JAS_THREADS_WIN32}")
	message("Threading library: ${THREAD_LIBRARY}")
else()
	message("Threading support has been disabled.")
endif()

if(JAS_THREADS_C11 AND JAS_ENABLE_TSAN)
	message(FATAL_ERROR
	  "TSAN does not have interceptors for C11 concurrency primitives.  "
	  "Using C11 threads and TSan will likely not end well.  "
	  "See https://github.com/google/sanitizers/issues/1195")
endif()

################################################################################
# Check for the OpenGL and GLUT libraries.
################################################################################

set(JAS_HAVE_OPENGL 0)
set(JAS_OPENGL_TARGETS)
if(JAS_ENABLE_OPENGL)
	set(OpenGL_GL_PREFERENCE GLVND)
	find_package(OpenGL)
	find_package(GLUT)
	if((TARGET OpenGL::GL) AND (TARGET GLUT::GLUT))
		jas_get_includes_from_targets(CMAKE_REQUIRE_INCLUDES
		  OpenGL::GL OpenGL::GLU GLUT::GLUT)
		message("CMAKE_REQUIRE_INCLUDES ${CMAKE_REQUIRE_INCLUDES}")
		check_include_files(GL/glut.h JAS_HAVE_GL_GLUT_H)
		check_include_files(GLUT/glut.h JAS_HAVE_GLUT_GLUT_H)
		check_include_files(glut.h JAS_HAVE_GLUT_H)
		if(JAS_HAVE_GL_GLUT_H OR JAS_HAVE_GLUT_GLUT_H OR JAS_HAVE_GLUT_H)
			set(JAS_OPENGL_TARGETS GLUT::GLUT OpenGL::GL)
			if (TARGET OpenGL::GLU)
				set(JAS_OPENGL_TARGETS ${JAS_OPENGL_TARGETS} OpenGL::GLU)
			endif()
			set(JAS_HAVE_OPENGL 1)
		else()
			message(WARNING "The glut.h header file is missing.")
			message(WARNING "Disabling OpenGL.")
		endif()
	endif()
endif()

if(JAS_HAVE_OPENGL AND MACOS)
	add_compile_definitions(GL_SILENCE_DEPRECATION)
endif()

################################################################################
# Check for the JPEG library.
################################################################################

# The following is needed to prevent the incorrect version of the JPEG library
# from being found.
if(MACOS)
	set(CMAKE_FIND_FRAMEWORK LAST)
endif()

set(JAS_HAVE_LIBJPEG 0)
set(JAS_LIBJPEG_TARGET)
if(JAS_ENABLE_LIBJPEG)
	find_package(JPEG)
	if (TARGET JPEG::JPEG)
		# In some versions of the JPEG library, the header file jpeglib.h
		# does not include some of the header files upon which it depends
		# (e.g., stdio.h and stdint.h).  So, we cannot reliably use
		# check_include_file here.
		jas_get_includes_from_targets(CMAKE_REQUIRED_INCLUDES JPEG::JPEG)
		check_c_source_compiles("
			#include <stdio.h>
			#include <stdint.h>
			#include <jpeglib.h>
			int main() {}
		" JAS_HAVE_JPEGLIB_H)
		if(JAS_HAVE_JPEGLIB_H)
			set(JAS_HAVE_LIBJPEG 1)
			set(JAS_LIBJPEG_TARGET JPEG::JPEG)
		else()
			message(WARNING "The header file jpeglib.h is missing.")
			message(WARNING "Disabling LIBJPEG.")
		endif()
	endif()
endif()

if(NOT JAS_HAVE_LIBJPEG)
	set(JAS_INCLUDE_JPG_CODEC 0)
endif()
message("JAS_INCLUDE_JPG_CODEC: ${JAS_INCLUDE_JPG_CODEC}")

################################################################################
# Check for the HEIF library.
################################################################################

set(JAS_HAVE_LIBHEIF 0)
if(JAS_ENABLE_LIBHEIF)
	find_library(HEIF_LIBRARY heif)
	check_include_files(libheif/heif.h JAS_HAVE_HEIF_H)
	if(HEIF_LIBRARY AND JAS_HAVE_HEIF_H)
		set(JAS_HAVE_LIBHEIF 1)
		set(HEIF_LIBRARIES "${HEIF_LIBRARY}")
	endif()
endif()
if(NOT JAS_HAVE_LIBHEIF)
	set(JAS_INCLUDE_HEIC_CODEC 0)
endif()
message("JAS_INCLUDE_HEIC_CODEC: ${JAS_INCLUDE_HEIC_CODEC}")

################################################################################
# Check for the Math library.
################################################################################

find_library(MATH_LIBRARY m)
if(NOT MATH_LIBRARY)
	set(MATH_LIBRARY "")
endif()

################################################################################
# Perform shared library setup.
################################################################################

if(JAS_ENABLE_SHARED)
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
	if(isSystemDir EQUAL -1)
	   set(CMAKE_INSTALL_RPATH
		  "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
	endif()
endif()

################################################################################
# Miscellany.
################################################################################

add_subdirectory(src/libjasper)
if(JAS_ENABLE_PROGRAMS)
    add_subdirectory(src/appl)
endif()
if(JAS_ENABLE_DOC)
	add_subdirectory(doc)
endif()

# The package configuation (pc) file should be installed in
# ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig.
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/build/jasper.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/build/jasper.pc" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/build/jasper.pc"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

install(FILES "README" DESTINATION "${CMAKE_INSTALL_DOCDIR}")
