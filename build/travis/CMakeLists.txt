cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(demo LANGUAGES C)
cmake_policy(SET CMP0012 NEW)

include(CheckIncludeFiles)
include(CheckCSourceCompiles)

option(ENABLE_LIBJPEG "Enable JPEG library" ON)

if (NOT JASPER_INCLUDE_DIR)
	set(JASPER_INCLUDE_DIR "/tmp/jasper/install/include")
endif()
if (NOT JASPER_LIBRARIES)
	set(JASPER_LIBRARIES "-L/tmp/jasper/install/lib" "-ljasper")
endif()

find_package(JPEG)
message("JPEG library found: ${JPEG_FOUND}")
if (ENABLE_LIBJPEG AND JPEG_FOUND)
	message("JPEG include directory: ${JPEG_INCLUDE_DIR}")
	message("JPEG libraries: ${JPEG_LIBRARIES}")
	include_directories(${JPEG_INCLUDE_DIR})
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
	" HAVE_JPEGLIB_H)
	message("HAVE_JPEGLIB_H: ${HAVE_JPEGLIB_H}")
	if(NOT HAVE_JPEGLIB_H)
		message(WARNING "Disabling LIBJPEG.")
		set(JPEG_FOUND false)
		set(JPEG_LIBRARIES "")
		set(JPEG_INCLUDE_DIR "")
		set(ENABLE_LIBJPEG 0)
	endif()
endif()
message("ENABLE_LIBJPEG: ${ENABLE_LIBJPEG}")

if (UNIX)
	set(MATH_LIBRARY "m")
else()
	set(MATH_LIBRARY "")
endif()

message("JASPER_INCLUDE_DIR ${JASPER_INCLUDE_DIR}")
message("JASPER_LIBRARIES ${JASPER_LIBRARIES}")
include_directories(${JASPER_INCLUDE_DIR})
link_libraries(${JASPER_LIBRARIES} ${JPEG_LIBRARIES} ${MATH_LIBRARY})
add_executable(jasper jasper.c)
