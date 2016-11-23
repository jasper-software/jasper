
foreach(i base bmp jp2 jpc jpg mif pgx pnm ras)
	include(${i}/CMakeLists.txt)
endforeach(i)

include(include/jasper/CMakeLists.txt)

add_library(libjasper ${libjasper_SOURCES})

install(TARGETS libjasper DESTINATION lib)
install(FILES ${libjasper_HEADERS} DESTINATION include/jasper)

#install(TARGETS "${CMAKE_CURRENT_LIST_DIR}/include/jasper/jas_config.h" PUBLIC_HEADER DESTINATION include/jasper)

set_target_properties(libjasper
  PROPERTIES OUTPUT_NAME jasper)
set_target_properties(libjasper
  PROPERTIES LINKER_LANGUAGE C)
