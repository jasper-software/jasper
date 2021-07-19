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

/******************************************************************************\
* Code.
\******************************************************************************/

/*
Various user-configurable settings.
*/
jas_conf_t jas_conf;

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

static const jas_image_fmttab_t jas_image_fmttab = {
	.num_entries = sizeof(jas_image_fmts) / sizeof(jas_image_fmt_t),
	.entries = jas_image_fmts
};

JAS_DLLEXPORT
const jas_image_fmttab_t *jas_get_image_fmttab()
{
	return &jas_image_fmttab;
}

/******************************************************************************\
* Code.
\******************************************************************************/

static void jas_init_codecs(void);

JAS_DLLEXPORT
void jas_get_default_conf(jas_conf_t *conf)
{
	memset(conf, 0, sizeof(jas_conf_t));
	conf->image_fmttab = jas_image_fmttab;
	conf->allocator.alloc = jas_bma_alloc;
	conf->allocator.free = jas_bma_free;
	conf->allocator.realloc = jas_bma_realloc;
	conf->max_mem = JAS_DEFAULT_MAX_MEM_USAGE;
	conf->dec_default_max_samples = JAS_DEC_DEFAULT_MAX_SAMPLES;
	conf->atexit_cleanup = false;
}

JAS_DLLEXPORT
int jas_init()
{
	jas_conf_t conf;
	jas_get_default_conf(&conf);
	conf.atexit_cleanup = true;
	return jas_init_custom(&conf);
}

JAS_DLLEXPORT
int jas_init_custom(const jas_conf_t* conf)
{
	jas_set_allocator(&conf->allocator);
	jas_set_conf(conf);
	jas_init_codecs();

	if (conf->atexit_cleanup) {
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
static void jas_init_codecs()
{
	jas_image_fmtops_t fmtops;
	int fmtid;
	const char delim[] = " \t";

	const jas_image_fmttab_t *fmttab = jas_get_image_fmttab();
	const jas_image_fmt_t *fmt;
	size_t i;
	fmtid = 0;
	for (fmt = fmttab->entries, i = 0; i < fmttab->num_entries; ++fmt, ++i) {
		char *buf = jas_strdup(fmt->exts);
		bool first = true;
		for (;;) {
			char *ext;
			if (!(ext = strtok(first ? buf : NULL, delim))) {
				break;
			}
			JAS_DBGLOG(10, ("adding image format %s %s\n", fmt->name, ext));
			jas_image_addfmt(fmtid, fmt->name, ext, fmt->desc, &fmt->ops);
			++fmtid;
			first = false;
		}
		jas_free(buf);
	}
}

JAS_DLLEXPORT
void jas_cleanup()
{
	JAS_DBGLOG(10, ("jas_cleanup invoked\n"));

	jas_image_clearfmts();

	memset(&jas_allocator, 0, sizeof(jas_allocator_t));
	jas_allocator.alloc = 0;
	jas_allocator.free = 0;
	jas_allocator.realloc = 0;

	JAS_DBGLOG(10, ("jas_cleanup returning\n"));
}

/******************************************************************************\
\******************************************************************************/

JAS_DLLEXPORT
void jas_set_conf(const jas_conf_t* conf)
{
	if (!conf) {
		jas_conf.dec_default_max_samples = JAS_DEC_DEFAULT_MAX_SAMPLES;
		jas_set_max_mem_usage(JAS_DEFAULT_MAX_MEM_USAGE);
	} else {
		jas_conf = *conf;
		jas_set_max_mem_usage(conf->max_mem);
	}
}

JAS_DLLEXPORT void jas_get_conf(jas_conf_t* conf)
{
	*conf = jas_conf;
}

/* For internal library use only. */
jas_conf_t *jas_get_conf_ptr()
{
	return &jas_conf;
}
