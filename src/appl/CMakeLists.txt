set(programs "")
set(man_pages "")

# The jasper program
add_executable(jasper jasper.c)
target_link_libraries(jasper libjasper ${JPEG_LIBRARIES} ${MATH_LIBRARY})
set(programs "${programs}" jasper)
set(man_pages "${man_pages}" jasper.1)

# The imginfo program
add_executable(imginfo imginfo.c)
target_link_libraries(imginfo libjasper ${JPEG_LIBRARIES} ${MATH_LIBRARY})
set(programs "${programs}" imginfo)
set(man_pages "${man_pages}" imginfo.1)

# The imgcmp program
add_executable(imgcmp imgcmp.c)
target_link_libraries(imgcmp libjasper ${JPEG_LIBRARIES} ${MATH_LIBRARY})
set(programs "${programs}" imgcmp)
set(man_pages "${man_pages}" imgcmp.1)

# The jiv program (which requires GLUT)
if(JAS_HAVE_OPENGL)
	add_executable(jiv jiv.c)
	target_include_directories(jiv PUBLIC
	  ${GLUT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
	target_link_libraries(jiv libjasper ${JPEG_LIBRARIES} ${GLUT_LIBRARIES}
	  ${OPENGL_LIBRARIES} ${MATH_LIBRARY})
	set(programs "${programs}" jiv)
	set(man_pages "${man_pages}" jiv.1)
else()
	message("GLUT library not found; skipping build of jiv program")
endif()

# The JasPer include directories in the source and build tree must be included
# before any other directories that may contain the include directory for an
# already installed version of the JasPer library.
include_directories(BEFORE
	"${CMAKE_CURRENT_BINARY_DIR}/../libjasper/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../libjasper/include"
)

# Install of the the programs that were built.
install(TARGETS ${programs} DESTINATION "${CMAKE_INSTALL_BINDIR}")

# Install the manual pages for the programs that were built.
install(FILES ${man_pages} DESTINATION "${CMAKE_INSTALL_MANDIR}/man1")
