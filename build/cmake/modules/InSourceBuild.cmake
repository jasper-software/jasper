option(ALLOW_IN_SOURCE_BUILD "Allow an in-source build" false)

function(prevent_in_source_build)
	get_filename_component(source_dir "${CMAKE_SOURCE_DIR}" REALPATH)
	get_filename_component(binary_dir "${CMAKE_BINARY_DIR}" REALPATH)
	if("${source_dir}" STREQUAL "${binary_dir}")
		message(FATAL_ERROR
			"The use of an in-source build is not recommended.  "
			"For this reason, the use of in-source build is disabled by default.  "
			"If you want to override this default behavior, add the -DALLOW_IN_SOURCE_BUILD option to cmake."
		)
	endif()
endfunction()

if (NOT ALLOW_IN_SOURCE_BUILD)
	prevent_in_source_build()
endif()
