cmake_minimum_required (VERSION 2.6)

project(jasper)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

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

set(JAS_MAJOR_VERSION 1)
set(JAS_MINOR_VERSION 0)
set(JAS_MICRO_VERSION 31)
set(JAS_VERSION "${JAS_MAJOR_VERSION}.${JAS_MINOR_VERSION}.${JAS_MICRO_VERSION}")

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

include_directories(src/libjasper/include)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/libjasper/include/jasper/jas_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/libjasper/include/jasper/jas_config.h)

add_test(run_test_1 test/bin/run_test_1)
add_test(run_test_2 test/bin/run_test_2)
add_test(run_test_3 test/bin/run_test_3)
add_test(run_test_4 test/bin/run_test_4)

