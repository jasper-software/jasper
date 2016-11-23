add_executable(jasper jasper.c)
target_link_libraries(jasper libjasper ${JPEG_LIBRARIES} ${MATH_LIBRARY})

add_executable(imginfo imginfo.c)
target_link_libraries(imginfo libjasper ${JPEG_LIBRARIES} ${MATH_LIBRARY})

add_executable(imgcmp imgcmp.c)
target_link_libraries(imgcmp libjasper ${JPEG_LIBRARIES} ${MATH_LIBRARY})

install(TARGETS jasper imginfo imgcmp DESTINATION bin)

#if(${GLUT_FOUND})
#	add_executable(jiv jiv.c)
#	target_link_libraries(jiv libjasper ${JPEG_LIBRARIES} ${GLUT_LIBRARIES} ${MATH_LIBRARY})
#endif()
