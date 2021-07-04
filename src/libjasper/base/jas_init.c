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

#include "jasper/jas_conf.h"
#include "jasper/jas_init.h"
#include "jasper/jas_image.h"
#include "jasper/jas_malloc.h"
#include "jasper/jas_debug.h"

#include <stdlib.h>
#include <stdbool.h>

/******************************************************************************\
* Code.
\******************************************************************************/

static void jas_init_codecs(void);

JAS_DLLEXPORT
int jas_init()
{
	return jas_init_custom(0, 0);
}

JAS_DLLEXPORT
int jas_init_custom(const jas_allocator_t* allocator, const jas_conf_t* conf)
{
	jas_set_allocator(allocator);
	jas_set_conf(conf);
	jas_init_codecs();

	/*
	If an allocator is specified, the library user is responsible for
	invoking jas_cleanup.
	*/
	if (!allocator) {
		/*
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

	fmtid = 0;

#if defined(JAS_INCLUDE_MIF_CODEC) && defined(JAS_ENABLE_MIF_CODEC)
	fmtops.decode = mif_decode;
	fmtops.encode = mif_encode;
	fmtops.validate = mif_validate;
	jas_image_addfmt(fmtid, "mif", "mif", "My Image Format (MIF)", &fmtops);
	++fmtid;
#endif

#if defined(JAS_INCLUDE_PNM_CODEC)
	fmtops.decode = pnm_decode;
	fmtops.encode = pnm_encode;
	fmtops.validate = pnm_validate;
	jas_image_addfmt(fmtid, "pnm", "pnm", "Portable Graymap/Pixmap (PNM)",
	  &fmtops);
	jas_image_addfmt(fmtid, "pnm", "pgm", "Portable Graymap/Pixmap (PNM)",
	  &fmtops);
	jas_image_addfmt(fmtid, "pnm", "ppm", "Portable Graymap/Pixmap (PNM)",
	  &fmtops);
	++fmtid;
#endif

#if defined(JAS_INCLUDE_BMP_CODEC)
	fmtops.decode = bmp_decode;
	fmtops.encode = bmp_encode;
	fmtops.validate = bmp_validate;
	jas_image_addfmt(fmtid, "bmp", "bmp", "Microsoft Bitmap (BMP)", &fmtops);
	++fmtid;
#endif

#if defined(JAS_INCLUDE_RAS_CODEC)
	fmtops.decode = ras_decode;
	fmtops.encode = ras_encode;
	fmtops.validate = ras_validate;
	jas_image_addfmt(fmtid, "ras", "ras", "Sun Rasterfile (RAS)", &fmtops);
	++fmtid;
#endif

#if defined(JAS_INCLUDE_JP2_CODEC)
	fmtops.decode = jp2_decode;
	fmtops.encode = jp2_encode;
	fmtops.validate = jp2_validate;
	jas_image_addfmt(fmtid, "jp2", "jp2",
	  "JPEG-2000 JP2 File Format Syntax (ISO/IEC 15444-1)", &fmtops);
	++fmtid;
#endif
#if defined(JAS_INCLUDE_JPC_CODEC) || defined(JAS_INCLUDE_JP2_CODEC)
	fmtops.decode = jpc_decode;
	fmtops.encode = jpc_encode;
	fmtops.validate = jpc_validate;
	jas_image_addfmt(fmtid, "jpc", "jpc",
	  "JPEG-2000 Code Stream Syntax (ISO/IEC 15444-1)", &fmtops);
	++fmtid;
#endif

#if defined(JAS_INCLUDE_JPG_CODEC)
	fmtops.decode = jpg_decode;
	fmtops.encode = jpg_encode;
	fmtops.validate = jpg_validate;
	jas_image_addfmt(fmtid, "jpg", "jpg", "JPEG (ISO/IEC 10918-1)", &fmtops);
	++fmtid;
#endif

#if defined(JAS_INCLUDE_PGX_CODEC)
	fmtops.decode = pgx_decode;
	fmtops.encode = pgx_encode;
	fmtops.validate = pgx_validate;
	jas_image_addfmt(fmtid, "pgx", "pgx", "JPEG-2000 VM Format (PGX)", &fmtops);
	++fmtid;
#endif
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
