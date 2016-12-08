if (DOXYGEN_FOUND)

	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in"
	  "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" @ONLY)
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/mainpage.dox.in"
	  "${CMAKE_CURRENT_BINARY_DIR}/src/mainpage.dox" @ONLY)
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/getting_started.dox.in"
	  "${CMAKE_CURRENT_BINARY_DIR}/src/getting_started.dox" @ONLY)

	add_custom_target(doc ALL
		${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "Generating API documentation with Doxygen" VERBATIM
	)

	if (LATEX_FOUND)
		add_custom_target(manual_pdf ALL
		  "${PDFLATEX_COMPILER}" refman.tex
		  COMMAND "${PDFLATEX_COMPILER}" refman.tex
		  COMMAND "${PDFLATEX_COMPILER}" refman.tex
		  COMMAND "${PDFLATEX_COMPILER}" refman.tex
		  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/latex"
		  DEPENDS doc
		)
	endif()

	# Install the HTML version of the manual.
	install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html"
	  DESTINATION "${CMAKE_INSTALL_DOCDIR}")

	# Install the PDF version of the manual built from LaTeX.
	if (LATEX_FOUND)
		install(FILES "${CMAKE_CURRENT_BINARY_DIR}/latex/refman.pdf"
		  DESTINATION "${CMAKE_INSTALL_DOCDIR}" RENAME manual.pdf)
	endif()
endif()

# Install papers and technical reports.
install(FILES "jasper.pdf" "jpeg2000.pdf"
  DESTINATION "${CMAKE_INSTALL_DOCDIR}")
