find_package(Doxygen)

find_package(LATEX COMPONENTS PDFLATEX)

# On some (or maybe all?) systems, LATEX_FOUND is not set by FindLATEX.
# So, instead, rely on LATEX_PDFLATEX_FOUND.
message("PDFLATEX_COMPILER: ${PDFLATEX_COMPILER}")
if ((NOT LATEX_FOUND) AND PDFLATEX_COMPILER)
	message(WARNING "Setting LATEX_FOUND to true.")
	message(WARNING "Your version of CMake may be buggy.")
	set(LATEX_FOUND true)
endif()
message("LATEX_FOUND ${LATEX_FOUND}")

if (DOXYGEN_FOUND)

	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in"
	  "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" @ONLY)
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/mainpage.dox.in"
	  "${CMAKE_CURRENT_BINARY_DIR}/src/mainpage.dox" @ONLY)
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/getting_started.dox.in"
	  "${CMAKE_CURRENT_BINARY_DIR}/src/getting_started.dox" @ONLY)

	file(GLOB_RECURSE DOXYGEN_DEPENDS "${CMAKE_SOURCE_DIR}/*.[ch]")
	add_custom_command(OUTPUT "html/index.html" "latex/index.tex"
		COMMAND ${DOXYGEN_EXECUTABLE} "Doxyfile"
		BYPRODUCTS "html" "latex"
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		MAIN_DEPENDENCY "Doxyfile"
		DEPENDS "${DOXYGEN_DEPENDS}"
		COMMENT "Generating API documentation with Doxygen" VERBATIM
	)

	add_custom_target(doc ALL
		DEPENDS "html/index.html"
	)

	if (LATEX_FOUND)
		add_custom_command(OUTPUT "latex/refman.pdf"
		  COMMAND "${PDFLATEX_COMPILER}" refman.tex
		  COMMAND "${PDFLATEX_COMPILER}" refman.tex
		  COMMAND "${PDFLATEX_COMPILER}" refman.tex
		  COMMAND "${PDFLATEX_COMPILER}" refman.tex
		  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/latex"
		  DEPENDS "latex/index.tex"
		)

		add_custom_target(manual_pdf ALL
			DEPENDS "latex/refman.pdf"
		)
	endif()

	set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES "html" "latex")

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
