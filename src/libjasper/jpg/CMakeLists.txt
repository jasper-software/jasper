if(${JPEG_FOUND})
	list(APPEND libjasper_SOURCES
		"${CMAKE_CURRENT_LIST_DIR}/jpg_dec.c"
		"${CMAKE_CURRENT_LIST_DIR}/jpg_enc.c"
	)
else()
	list(APPEND libjasper_SOURCES
		"${CMAKE_CURRENT_LIST_DIR}/jpg_dummy.c"
	)
endif()

list(APPEND libjasper_SOURCES
	"${CMAKE_CURRENT_LIST_DIR}/jpg_val.c"
)
