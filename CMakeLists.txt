cmake_minimum_required (VERSION 2.8.11)

project(jasper)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# This include should be placed as early as possible.
include(InSourceBuild)

set(JAS_MAJOR_VERSION 1)
set(JAS_MINOR_VERSION 900)
set(JAS_MICRO_VERSION 31)
set(JAS_VERSION "${JAS_MAJOR_VERSION}.${JAS_MINOR_VERSION}.${JAS_MICRO_VERSION}")

include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CTest)
include(Sanitizers)
cmake_policy(SET CMP0012 NEW)

option(JAS_ENABLE_LIBJPEG "Enable use of JPEG Library" true)
option(JAS_ENABLE_OPENGL "Enable use of OpenGL Library" true)
option(JAS_STRICT "Enable pedantic error checking" false)

set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE on)

INCLUDE(InstallRequiredSystemLibraries)
SET(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
SET(CPACK_PACKAGE_VERSION "${JAS_VERSION}")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "JasPer Image Processing Tool Kit")
SET(CPACK_PACKAGE_VENDOR "Michael Adams")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_VERSION_MAJOR "${JAS_MAJOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_MINOR "${JAS_MINOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_PATCH "${JAS_MICRO_VERSION}")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY
  "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
SET(CPACK_PACKAGE_FILE_NAME
  "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
SET(CPACK_GENERATOR "TGZ")
include(CPack)

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
if(NOT JAS_ENABLE_LIBJPEG)
	set(JPEG_FOUND false)
endif()
message("JPEG_FOUND ${JPEG_FOUND}")

find_package(GLUT)
if(GLUT_FOUND)
	include_directories(${GLUT_INCLUDE_DIR})
endif()
check_include_files(GL/glut.h JAS_HAVE_GL_GLUT_H)
if(NOT JAS_HAVE_GL_GLUT_H)
	set(GLUT_FOUND false)
endif()
if(NOT JAS_ENABLE_OPENGL)
	set(GLUT_FOUND false)
endif()
message("GLUT_FOUND ${GLUT_FOUND}")

if(UNIX)
	set(MATH_LIBRARY m)
else()
	set(MATH_LIBRARY "")
endif()

subdirs(src/libjasper src/appl)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/src/libjasper/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/libjasper/include)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/libjasper/include/jasper/jas_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/libjasper/include/jasper/jas_config.h)

set(CTEST_ENVIRONMENT
	"JAS_ABS_TOP_BUILDDIR=${CMAKE_CURRENT_BINARY_DIR}"
	"JAS_ABS_TOP_SRCDIR=${CMAKE_CURRENT_SOURCE_DIR}"
	"JAS_TOP_BUILDDIR=${CMAKE_CURRENT_BINARY_DIR}"
	"JAS_TOP_SRCDIR=${CMAKE_CURRENT_SOURCE_DIR}"
)
#	"JAS_TOP_BUILDDIR=../../build"
#	"JAS_TOP_SRCDIR=../.."

add_test(run_test_1 ${CMAKE_CURRENT_SOURCE_DIR}/test/bin/run_test_1)
add_test(run_test_2 ${CMAKE_CURRENT_SOURCE_DIR}/test/bin/run_test_2)
add_test(run_test_3 ${CMAKE_CURRENT_SOURCE_DIR}/test/bin/run_test_3)
add_test(run_test_4 ${CMAKE_CURRENT_SOURCE_DIR}/test/bin/run_test_4)

#find_package(MakeDist)

#set(EXTRA_DIST data)

#add_makedist(
#	${CMAKE_CURRENT_BINARY_DIR}/README
#	${CMAKE_CURRENT_BINARY_DIR}/LICENSE
#)
