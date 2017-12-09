configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/include/jasper/jas_config.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/include/jasper/jas_config.h"
)

set(libjasper_headers
	include/jasper/jas_cm.h
	"${CMAKE_CURRENT_BINARY_DIR}/include/jasper/jas_config.h"
	include/jasper/jas_debug.h
	include/jasper/jas_dll.h
	include/jasper/jas_fix.h
	include/jasper/jas_getopt.h
	include/jasper/jas_icc.h
	include/jasper/jas_image.h
	include/jasper/jas_init.h
	include/jasper/jas_malloc.h
	include/jasper/jas_math.h
	include/jasper/jasper.h
	include/jasper/jas_seq.h
	include/jasper/jas_stream.h
	include/jasper/jas_string.h
	include/jasper/jas_tmr.h
	include/jasper/jas_tvp.h
	include/jasper/jas_types.h
	include/jasper/jas_version.h
)

set(libjasper_base_sources
	base/jas_cm.c
	base/jas_debug.c
	base/jas_getopt.c
	base/jas_icc.c
	base/jas_iccdata.c
	base/jas_image.c
	base/jas_init.c
	base/jas_malloc.c
	base/jas_seq.c
	base/jas_stream.c
	base/jas_string.c
	base/jas_tmr.c
	base/jas_tvp.c
	base/jas_version.c
)

set(libjasper_bmp_sources
	bmp/bmp_cod.c
	bmp/bmp_dec.c
	bmp/bmp_enc.c
)

set(libjasper_jp2_sources
	jp2/jp2_cod.c
	jp2/jp2_dec.c
	jp2/jp2_enc.c
)

set(libjasper_jpc_sources
	jpc/jpc_bs.c
	jpc/jpc_cs.c
	jpc/jpc_dec.c
	jpc/jpc_enc.c
	jpc/jpc_math.c
	jpc/jpc_mct.c
	jpc/jpc_mqcod.c
	jpc/jpc_mqdec.c
	jpc/jpc_mqenc.c
	jpc/jpc_qmfb.c
	jpc/jpc_t1cod.c
	jpc/jpc_t1dec.c
	jpc/jpc_t1enc.c
	jpc/jpc_t2cod.c
	jpc/jpc_t2dec.c
	jpc/jpc_t2enc.c
	jpc/jpc_tagtree.c
	jpc/jpc_tsfb.c
	jpc/jpc_util.c
)

set(libjasper_jpg_sources
	jpg/jpg_val.c
)
if(JAS_HAVE_LIBJPEG)
	list(APPEND libjasper_jpg_sources
		jpg/jpg_dec.c
		jpg/jpg_enc.c
	)
else()
	list(APPEND libjasper_jpg_sources
		jpg/jpg_dummy.c
	)
endif()

set(libjasper_mif_sources
	mif/mif_cod.c
)

set(libjasper_pgx_sources
	pgx/pgx_cod.c
	pgx/pgx_dec.c
	pgx/pgx_enc.c
)

set(libjasper_pnm_sources
	pnm/pnm_cod.c
	pnm/pnm_dec.c
	pnm/pnm_enc.c
)

set(libjasper_ras_sources
	ras/ras_cod.c
	ras/ras_dec.c
	ras/ras_enc.c
)

if (JAS_ENABLE_SHARED)
	set(libjasper_type SHARED)
else()
	set(libjasper_type STATIC)
endif()

add_library(libjasper ${libjasper_type}
	${libjasper_headers}
	${libjasper_base_sources}
	${libjasper_bmp_sources}
	${libjasper_jp2_sources}
	${libjasper_jpc_sources}
	${libjasper_jpg_sources}
	${libjasper_mif_sources}
	${libjasper_pgx_sources}
	${libjasper_pnm_sources}
	${libjasper_ras_sources}
)

# The JasPer include directories in the source and build tree must be included
# before any other directories that may contain the include directory for an
# already installed version of the JasPer library.
# The PRIVATE specifier has been used here to force the library user to have
# to explicitly specify the include directories from the source and build tree
# so that the BEFORE specifier can also be employed by the library user.
target_include_directories(libjasper BEFORE PRIVATE
	"${CMAKE_CURRENT_BINARY_DIR}/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set_target_properties(libjasper PROPERTIES OUTPUT_NAME jasper)
set_target_properties(libjasper PROPERTIES LINKER_LANGUAGE C)

if (MSVC)
	target_compile_definitions(libjasper PRIVATE "-D_CRT_SECURE_NO_WARNINGS")
	target_compile_definitions(libjasper PRIVATE "-D_CRT_SECURE_NO_DEPRECATE")
	target_compile_definitions(libjasper PRIVATE "-D_CRT_NONSTDC_NO_DEPRECATE")
endif()

if (UNIX)
	set_target_properties(libjasper PROPERTIES
	  VERSION ${JAS_SO_NAME} SOVERSION ${JAS_SO_VERSION})
endif()

if (JAS_ENABLE_SHARED)
	set_target_properties(libjasper PROPERTIES
	  DEFINE_SYMBOL "JAS_BUILDING_DLL")
endif()

# I am not sure why the following line is needed for building a library, but
# MacOSX does cannot build a shared library without this line.
# Also, try adding MATH_LIBRARY to fix a Gentoo problem.
target_link_libraries(libjasper ${JPEG_LIBRARIES} ${MATH_LIBRARY})

install(TARGETS libjasper
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES ${libjasper_headers}
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/jasper"
)
