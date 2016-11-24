configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/include/jasper/jas_config.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/include/jasper/jas_config.h"
)

list(APPEND libjasper_sources
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
	bmp/bmp_cod.c
	bmp/bmp_dec.c
	bmp/bmp_enc.c
	jp2/jp2_cod.c
	jp2/jp2_dec.c
	jp2/jp2_enc.c
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
	jpg/jpg_val.c
	mif/mif_cod.c
	pgx/pgx_cod.c
	pgx/pgx_dec.c
	pgx/pgx_enc.c
	pnm/pnm_cod.c
	pnm/pnm_dec.c
	pnm/pnm_enc.c
	ras/ras_cod.c
	ras/ras_dec.c
	ras/ras_enc.c
)

if(JPEG_FOUND)
	list(APPEND libjasper_sources
		jpg/jpg_dec.c
		jpg/jpg_enc.c
	)
else()
	list(APPEND libjasper_sources
		jpg/jpg_dummy.c
	)
endif()

set(libjasper_headers
	include/jasper/jas_cm.h
	${CMAKE_CURRENT_BINARY_DIR}/include/jasper/jas_config.h
	include/jasper/jas_debug.h
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

add_library(libjasper ${libjasper_sources})

install(TARGETS libjasper DESTINATION lib)
install(FILES ${libjasper_headers} DESTINATION include/jasper)

target_include_directories(libjasper PUBLIC 
	"${CMAKE_CURRENT_BINARY_DIR}/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

#install(TARGETS "${CMAKE_CURRENT_LIST_DIR}/include/jasper/jas_config.h" PUBLIC_HEADER DESTINATION include/jasper)

set_target_properties(libjasper
  PROPERTIES OUTPUT_NAME jasper)
set_target_properties(libjasper
  PROPERTIES LINKER_LANGUAGE C)
