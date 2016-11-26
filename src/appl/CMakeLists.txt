set(programs "")

# The jasper program
add_executable(jasper jasper.c)
target_link_libraries(jasper libjasper ${JPEG_LIBRARIES} ${MATH_LIBRARY})
set(programs "${programs}" jasper)

# The imginfo program
add_executable(imginfo imginfo.c)
target_link_libraries(imginfo libjasper ${JPEG_LIBRARIES} ${MATH_LIBRARY})
set(programs "${programs}" imginfo)

# The imgcmp program
add_executable(imgcmp imgcmp.c)
target_link_libraries(imgcmp libjasper ${JPEG_LIBRARIES} ${MATH_LIBRARY})
set(programs "${programs}" imgcmp)

# The jiv program (which requires GLUT)
if(GLUT_FOUND)
	add_executable(jiv jiv.c)
	target_link_libraries(jiv libjasper ${JPEG_LIBRARIES} ${GLUT_LIBRARIES} ${MATH_LIBRARY})
	set(programs "${programs}" jiv)
else()
	message("GLUT library not found; skipping build of jiv program")
endif()

# Install of the the programs that were built.
install(TARGETS ${programs} DESTINATION bin)

