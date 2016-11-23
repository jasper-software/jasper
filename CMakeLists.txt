cmake_minimum_required (VERSION 2.6)
include(CheckIncludeFiles)
include(CheckFunctionExists)
cmake_policy(SET CMP0012 NEW)

project(jasper)

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

find_package(JPEG)

find_package(GLUT)
if(GLUT_FOUND)
	include_directories(${GLUT_INCLUDE_DIR})
endif()
check_include_files(GL/glut.h JAS_HAVE_GL_GLUT_H)
if(NOT JAS_HAVE_GL_GLUT_H)
	message("setting GLUT_FOUND to false")
	set(GLUT_FOUND false)
endif()
#set(GLUT_FOUND false)
message("GLUT_FOUND ${GLUT_FOUND}")

if(UNIX)
	set(MATH_LIBRARY m)
else()
	set(MATH_LIBRARY "")
endif()

subdirs(src/libjasper src/appl)

include_directories(src/libjasper/include)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/libjasper/include/jasper/jas_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/libjasper/include/jasper/jas_config.h)

