if (DOXYGEN_FOUND)

	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
	  ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/mainpage.dox.in
	  ${CMAKE_CURRENT_BINARY_DIR}/src/mainpage.dox @ONLY)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/getting_started.dox.in
	  ${CMAKE_CURRENT_BINARY_DIR}/src/getting_started.dox @ONLY)

	add_custom_target(doc ALL
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
	)
endif()
