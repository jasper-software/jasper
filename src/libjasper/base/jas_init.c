/*
 * Copyright (c) 2001-2002 Michael David Adams.
 * All rights reserved.
 */

/* __START_OF_JASPER_LICENSE__
 * 
 * JasPer License Version 2.0
 * 
 * Copyright (c) 2001-2006 Michael David Adams
 * Copyright (c) 1999-2000 Image Power, Inc.
 * Copyright (c) 1999-2000 The University of British Columbia
 * 
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person (the
 * "User") obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * 1.  The above copyright notices and this permission notice (which
 * includes the disclaimer below) shall be included in all copies or
 * substantial portions of the Software.
 * 
 * 2.  The name of a copyright holder shall not be used to endorse or
 * promote products derived from the Software without specific prior
 * written permission.
 * 
 * THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL PART OF THIS
 * LICENSE.  NO USE OF THE SOFTWARE IS AUTHORIZED HEREUNDER EXCEPT UNDER
 * THIS DISCLAIMER.  THE SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS
 * "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS.  IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
 * INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
 * FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.  NO ASSURANCES ARE
 * PROVIDED BY THE COPYRIGHT HOLDERS THAT THE SOFTWARE DOES NOT INFRINGE
 * THE PATENT OR OTHER INTELLECTUAL PROPERTY RIGHTS OF ANY OTHER ENTITY.
 * EACH COPYRIGHT HOLDER DISCLAIMS ANY LIABILITY TO THE USER FOR CLAIMS
 * BROUGHT BY ANY OTHER ENTITY BASED ON INFRINGEMENT OF INTELLECTUAL
 * PROPERTY RIGHTS OR OTHERWISE.  AS A CONDITION TO EXERCISING THE RIGHTS
 * GRANTED HEREUNDER, EACH USER HEREBY ASSUMES SOLE RESPONSIBILITY TO SECURE
 * ANY OTHER INTELLECTUAL PROPERTY RIGHTS NEEDED, IF ANY.  THE SOFTWARE
 * IS NOT FAULT-TOLERANT AND IS NOT INTENDED FOR USE IN MISSION-CRITICAL
 * SYSTEMS, SUCH AS THOSE USED IN THE OPERATION OF NUCLEAR FACILITIES,
 * AIRCRAFT NAVIGATION OR COMMUNICATION SYSTEMS, AIR TRAFFIC CONTROL
 * SYSTEMS, DIRECT LIFE SUPPORT MACHINES, OR WEAPONS SYSTEMS, IN WHICH
 * THE FAILURE OF THE SOFTWARE OR SYSTEM COULD LEAD DIRECTLY TO DEATH,
 * PERSONAL INJURY, OR SEVERE PHYSICAL OR ENVIRONMENTAL DAMAGE ("HIGH
 * RISK ACTIVITIES").  THE COPYRIGHT HOLDERS SPECIFICALLY DISCLAIM ANY
 * EXPRESS OR IMPLIED WARRANTY OF FITNESS FOR HIGH RISK ACTIVITIES.
 * 
 * __END_OF_JASPER_LICENSE__
 */

/******************************************************************************\
* Includes.
\******************************************************************************/

#define JAS_INTERNAL_USE_ONLY

#include "jasper/jas_init.h"
#include "jasper/jas_image.h"
#include "jasper/jas_malloc.h"
#include "jasper/jas_debug.h"
#include "jasper/jas_string.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/******************************************************************************\
* Function prototypes.
\******************************************************************************/

void jas_context_init(jas_ctx_t *ctx);

static int jas_init_codecs(jas_ctx_t *ctx);

static int jas_init_helper(void);

void jas_context_cleanup(jas_ctx_t *ctx);

/******************************************************************************\
* Data.
\******************************************************************************/

static const jas_image_fmt_t jas_image_fmts[] = {

#if defined(JAS_INCLUDE_MIF_CODEC) && defined(JAS_ENABLE_MIF_CODEC)
	{
		"mif",
		"My Image Format (MIF)",
		"mif",
		{
			.decode = mif_decode,
			.encode = mif_encode,
			.validate = mif_validate
		}
	},
#endif

#if defined(JAS_INCLUDE_PNM_CODEC)
	{
		"pnm",
		"Portable Graymap/Pixmap (PNM)",
		"pnm pbm pgm ppm",
		{
			.decode = pnm_decode,
			.encode = pnm_encode,
			.validate = pnm_validate
		}
	},
#endif

#if defined(JAS_INCLUDE_BMP_CODEC)
	{
		"bmp",
		"Microsoft Bitmap (BMP)",
		"bmp",
		{
			.decode = bmp_decode,
			.encode = bmp_encode,
			.validate = bmp_validate
		}
	},
#endif

#if defined(JAS_INCLUDE_RAS_CODEC)
	{
		"ras",
		"Sun Rasterfile (RAS)",
		"ras",
		{
			.decode = ras_decode,
			.encode = ras_encode,
			.validate = ras_validate
		}
	},
#endif

#if defined(JAS_INCLUDE_JP2_CODEC)
	{
		"jp2",
		"JPEG-2000 JP2 File Format Syntax (ISO/IEC 15444-1)",
		"jp2",
		{
			.decode = jp2_decode,
			.encode = jp2_encode,
			.validate = jp2_validate
		}
	},
#endif

#if defined(JAS_INCLUDE_JPC_CODEC) || defined(JAS_INCLUDE_JP2_CODEC)
	{
		"jpc",
		"JPEG-2000 Code Stream Syntax (ISO/IEC 15444-1)",
		"jpc",
		{
			.decode = jpc_decode,
			.encode = jpc_encode,
			.validate = jpc_validate
		}
	},
#endif

#if defined(JAS_INCLUDE_JPG_CODEC)
	{
		"jpg",
		"JPEG (ISO/IEC 10918-1)",
		"jpg",
		{
			.decode = jpg_decode,
			.encode = jpg_encode,
			.validate = jpg_validate
		}
	},
#endif

#if defined(JAS_INCLUDE_PGX_CODEC)
	{
		"pgx",
		"JPEG-2000 VM Format (PGX)",
		"pgx",
		{
			.decode = pgx_decode,
			.encode = pgx_encode,
			.validate = pgx_validate
		}
	},
#endif

};

/*
Various user-configurable settings.
*/
jas_conf_t jas_conf = {
	.initialized = 0,
};

#if defined(JAS_ENABLE_MULTITHREADING_SUPPORT)
static jas_tss_t jas_tss;
#endif

static jas_ctx_t *jas_global_ctx = 0;

static jas_ctx_t jas_global_ctx_buf;

/******************************************************************************\
* Code.
\******************************************************************************/

JAS_DLLEXPORT
void jas_get_image_format_table(const jas_image_fmt_t** formats,
  size_t *num_formats)
{
	*formats = jas_image_fmts;
	*num_formats = sizeof(jas_image_fmts) / sizeof(jas_image_fmt_t);
}

JAS_DLLEXPORT
void jas_conf_clear()
{
	memset(&jas_conf, 0, sizeof(jas_conf_t));
	jas_conf.initialized = 1;
	jas_conf.allocator = 0;
	jas_conf.enable_allocator_wrapper = 1;
	jas_conf.max_mem = JAS_DEFAULT_MAX_MEM_USAGE;
	jas_conf.dec_default_max_samples = JAS_DEC_DEFAULT_MAX_SAMPLES;
	jas_conf.debug_level = 0;
	jas_conf.enable_atexit_cleanup = 0;
	jas_conf.veprintf = jas_veprintf_stderr;
	//jas_conf.veprintf = jas_veprintf_discard_impl;
	jas_conf.num_image_formats = sizeof(jas_image_fmts) /
	  sizeof(jas_image_fmt_t);
	jas_conf.image_formats = jas_image_fmts;
}

JAS_DLLEXPORT
void jas_conf_set_allocator(jas_allocator_t *allocator)
{
	jas_conf.allocator = allocator;
}

JAS_DLLEXPORT
void jas_conf_set_allocator_wrapper(bool enable)
{
	jas_conf.enable_allocator_wrapper = enable;
}

JAS_DLLEXPORT
void jas_conf_set_debug_level(int debug_level)
{
	jas_conf.debug_level = debug_level;
}

JAS_DLLEXPORT
void jas_conf_set_max_mem(size_t max_mem)
{
	jas_conf.max_mem = max_mem;
}

JAS_DLLEXPORT
void jas_conf_set_dec_default_max_samples(size_t n)
{
	jas_conf.dec_default_max_samples = n;
}

JAS_DLLEXPORT
void jas_conf_set_veprintf(int (*func)(const char *, va_list))
{
	jas_conf.veprintf = func;
}

JAS_DLLEXPORT
void jas_conf_set_image_format_table(const jas_image_fmt_t *formats,
  size_t num_formats)
{
	jas_conf.image_formats = formats;
	jas_conf.num_image_formats = num_formats;
}

JAS_DLLEXPORT
int jas_init()
{
	jas_conf_clear();
	jas_conf.enable_atexit_cleanup = 1;
	return jas_init_helper();
}

JAS_DLLEXPORT
int jas_initialize()
{
	assert(jas_conf.initialized);
	return jas_init_helper();
}

static int jas_init_helper()
{
#if defined(JAS_ENABLE_MULTITHREADING_SUPPORT)
	memset(&jas_tss, 0, sizeof(jas_tss_t));
#endif

	/*
	The initialization of the global context must be performed first.
	The memory allocator uses information from the context in order to
	handle log messages.
	*/
	jas_context_init(&jas_global_ctx_buf);
	jas_global_ctx = &jas_global_ctx_buf;

	if (jas_conf.enable_allocator_wrapper) {
		jas_allocator_t *delegate;
		if (!jas_conf.allocator) {
			jas_std_allocator_init(&jas_std_allocator);
			delegate = &jas_std_allocator.base;
		} else {
			delegate = jas_conf.allocator;
		}
		jas_basic_allocator_init(&jas_basic_allocator, delegate,
		  jas_conf.max_mem);
		jas_allocator = &jas_basic_allocator.base;
	} else {
		if (!jas_conf.allocator) {
			jas_std_allocator_init(&jas_std_allocator);
			jas_allocator = &jas_std_allocator.base;
		} else {
			jas_allocator = jas_conf.allocator;
		}
	}

#if defined(JAS_ENABLE_MULTITHREADING_SUPPORT)
	if (jas_tss_create(&jas_tss, 0)) {
		return -1;
	}
#endif

	jas_global_ctx->dec_default_max_samples = jas_conf.dec_default_max_samples;
	jas_global_ctx->debug_level = jas_conf.debug_level;
	//jas_setdbglevel(jas_conf.debug_level);

	if (jas_init_codecs(jas_global_ctx)) {
		return -1;
	}

	if (jas_conf.enable_atexit_cleanup) {
		/*
		Note:
		We must not register the JasPer library exit handler until after
		at least one memory allocation is performed.  This is desirable
		as it ensures that the JasPer exit handler is called before the
		debug memory allocator exit handler.
		*/
		atexit(jas_cleanup);
	}

	return 0;
}

/* Initialize the image format table. */
static int jas_init_codecs(jas_ctx_t *ctx)
{
	int fmtid;
	const char delim[] = " \t";

	const jas_image_fmt_t *fmt;
	size_t i;
	fmtid = 0;
	for (fmt = jas_conf.image_formats, i = 0; i < jas_conf.num_image_formats;
	  ++fmt, ++i) {
		char *buf = jas_strdup(fmt->exts);
		bool first = true;
		for (;;) {
			char *saveptr;
			char *ext;
			if (!(ext = jas_strtok(first ? buf : NULL, delim, &saveptr))) {
				break;
			}
			JAS_DBGLOG(10, ("adding image format %s %s\n", fmt->name, ext));
			jas_image_addfmt_internal(ctx->image_fmtinfos, &ctx->image_numfmts,
			  fmtid, fmt->name, ext, fmt->desc, &fmt->ops);
			++fmtid;
			first = false;
		}
		jas_free(buf);
	}
	return 0;
}

JAS_DLLEXPORT
void jas_cleanup()
{
	JAS_DBGLOG(10, ("jas_cleanup invoked\n"));

	jas_global_ctx = &jas_global_ctx_buf;
	jas_context_cleanup(&jas_global_ctx_buf);
	jas_context_set_debug_level(&jas_global_ctx_buf, 0);

	assert(jas_allocator);
	jas_allocator_cleanup(jas_allocator);
	jas_allocator = 0;

	JAS_DBGLOG(10, ("jas_cleanup returning\n"));

#if defined(JAS_ENABLE_MULTITHREADING_SUPPORT)
	jas_tss_delete(jas_tss);
#endif
}

/******************************************************************************\
\******************************************************************************/

/* For internal library use only. */
jas_conf_t *jas_get_conf_ptr()
{
	return &jas_conf;
}

void jas_context_init(jas_ctx_t *ctx)
{
	memset(ctx, 0, sizeof(jas_ctx_t));
	ctx->dec_default_max_samples = jas_conf.dec_default_max_samples;
	ctx->debug_level = jas_conf.debug_level;
	ctx->veprintf = jas_conf.veprintf;
	ctx->image_numfmts = 0;
}

JAS_DLLEXPORT
jas_context_t jas_context_create()
{
	jas_ctx_t *ctx;
	if (!(ctx = jas_malloc(sizeof(jas_ctx_t)))) {
		return 0;
	}
	jas_context_init(ctx);
	jas_init_codecs(ctx);
	return JAS_CAST(jas_context_t, ctx);
}

void jas_context_cleanup(jas_ctx_t *ctx)
{
	jas_image_clearfmts_internal(ctx->image_fmtinfos, &ctx->image_numfmts);
}

JAS_DLLEXPORT
void jas_context_destroy(jas_context_t context)
{
	jas_context_cleanup(JAS_CAST(jas_ctx_t *, context));
	jas_free(context);
}

JAS_DLLEXPORT
jas_context_t jas_get_context()
{
#if defined(JAS_ENABLE_MULTITHREADING_SUPPORT)
	jas_ctx_t *ctx = JAS_CAST(jas_context_t, jas_tss_get(jas_tss));
	if (!ctx) {
		ctx = jas_global_ctx;
	}
	return JAS_CAST(jas_context_t, ctx);
#else
	return JAS_CAST(jas_context_t, (jas_global_ctx) ? jas_global_ctx :
	  &jas_global_ctx_buf);
#endif
}

JAS_DLLEXPORT
void jas_set_context(jas_context_t context)
{
#if defined(JAS_ENABLE_MULTITHREADING_SUPPORT)
	if (jas_tss_set(jas_tss, JAS_CAST(void *, context))) {
		assert(0);
		abort();
	}
#else
	jas_global_ctx = JAS_CAST(jas_ctx_t *, context);
#endif
}

JAS_DLLEXPORT
int jas_context_set_debug_level(jas_context_t context, int debug_level)
{
	jas_ctx_t *ctx = JAS_CAST(jas_ctx_t *, context);
	int old = ctx->debug_level;
	ctx->debug_level = debug_level;
	return old;
}

JAS_DLLEXPORT
void jas_context_set_dec_default_max_samples(jas_context_t context,
  size_t max_samples)
{
	jas_ctx_t *ctx = JAS_CAST(jas_ctx_t *, context);
	ctx->dec_default_max_samples = max_samples;
}

JAS_DLLEXPORT
void jas_context_set_veprintf(jas_context_t context,
  int (*func)(const char *, va_list))
{
	jas_ctx_t *ctx = JAS_CAST(jas_ctx_t *, context);
	ctx->veprintf = func;
}
