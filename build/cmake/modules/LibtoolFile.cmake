# Macro CREATE_LIBTOOL_FILE creates a libtool .la file for libraries, in the CMAKE_INSTALL_LIBDIR directory.
# Macro GET_TARGET_PROPERTY_WITH_DEFAULT is helpful to handle properties with default values.

#ADD_LIBRARY(foo SHARED foo1.cpp foo2.cpp)
#CREATE_LIBTOOL_FILE(foo /lib)

if(DEFINED __INCLUDED_CREATE_LIBTOOL_FILE)
    return()
endif()

set(__INCLUDED_CREATE_LIBTOOL_FILE TRUE)

MACRO(GET_TARGET_PROPERTY_WITH_DEFAULT _variable _target _property _default_value)
  GET_TARGET_PROPERTY (${_variable} ${_target} ${_property})
  IF (${_variable} MATCHES NOTFOUND)
    SET (${_variable} ${_default_value})
  ENDIF (${_variable} MATCHES NOTFOUND)
ENDMACRO (GET_TARGET_PROPERTY_WITH_DEFAULT)

MACRO(CREATE_LIBTOOL_FILE _target)
  GET_FILENAME_COMPONENT(_laname ${_target} NAME_WE)
  GET_FILENAME_COMPONENT(_libname ${_target} NAME)
  SET(_laname ${CMAKE_CURRENT_BINARY_DIR}/${_laname}.la)
  SET(_soname ${_libname}.so)
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_shared ${_target} LT_SHARED false)
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_static_lib ${_target} STATIC_LIB ${_libname}.a)
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_dependency_libs ${_target} LT_DEPENDENCY_LIBS "")
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_current ${_target} LT_VERSION_CURRENT 0)
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_age ${_target} LT_VERSION_AGE 0)
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_revision ${_target} LT_VERSION_REVISION 0)
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_installed ${_target} LT_INSTALLED yes)
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_shouldnotlink ${_target} LT_SHOULDNOTLINK yes)
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_dlopen ${_target} LT_DLOPEN "")
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_dlpreopen ${_target} LT_DLPREOPEN "")
  FILE(WRITE ${_laname} "# ${_libname} - a libtool library file\n")
  FILE(APPEND ${_laname} "# Generated by CMake ${CMAKE_VERSION} (like GNU libtool)\n")
  FILE(APPEND ${_laname} "\n# Please DO NOT delete this file!\n# It is necessary for linking the library with libtool.\n\n" )
  FILE(APPEND ${_laname} "# The name that we can dlopen(3).\n")
  IF (${_target_shared})
    FILE(APPEND ${_laname} "dlname='${_soname}.${_target_current}'\n\n")
  ELSE()
    FILE(APPEND ${_laname} "dlname=''\n\n")
  ENDIF()
  FILE(APPEND ${_laname} "# Names of this library.\n")
  IF (${_target_shared})
    FILE(APPEND ${_laname} "library_names='${_soname}.${_target_current}.${_target_age}.${_target_revision} ${_soname}.${_target_current} ${_soname}'\n\n")
  ELSE()
    FILE(APPEND ${_laname} "library_names=''\n\n")
  ENDIF()
  FILE(APPEND ${_laname} "# The name of the static archive.\n")
  IF (NOT ${_target_shared})
    FILE(APPEND ${_laname} "old_library='${_target_static_lib}'\n\n")
  ELSE()
    FILE(APPEND ${_laname} "old_library=''\n\n")
  ENDIF()
  FILE(APPEND ${_laname} "# Libraries that this one depends upon.\n")
  FILE(APPEND ${_laname} "dependency_libs='${_target_dependency_libs}'\n\n")
  FILE(APPEND ${_laname} "# Version information for ${_libname}.\n")
  FILE(APPEND ${_laname} "current=${_target_current}\n")
  FILE(APPEND ${_laname} "age=${_target_age}\n")
  FILE(APPEND ${_laname} "revision=${_target_revision}\n\n")
  FILE(APPEND ${_laname} "# Is this an already installed library?\n")
  FILE(APPEND ${_laname} "installed=${_target_installed}\n\n")
  FILE(APPEND ${_laname} "# Should we warn about portability when linking against -modules?\n")
  FILE(APPEND ${_laname} "shouldnotlink=${_target_shouldnotlink}\n\n")
  FILE(APPEND ${_laname} "# Files to dlopen/dlpreopen\n")
  FILE(APPEND ${_laname} "dlopen='${_target_dlopen}'\n")
  FILE(APPEND ${_laname} "dlpreopen='${_target_dlpreopen}'\n\n")
  FILE(APPEND ${_laname} "# Directory that this library needs to be installed in:\n")
  FILE(APPEND ${_laname} "libdir='${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}'\n")
  INSTALL(FILES ${_laname} DESTINATION ${CMAKE_INSTALL_LIBDIR})
ENDMACRO(CREATE_LIBTOOL_FILE)
