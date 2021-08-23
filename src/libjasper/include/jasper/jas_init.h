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

/*!
 * @file jas_init.h
 * @brief JasPer Initialization/Cleanup Code
 */

#ifndef JAS_INIT_H
#define JAS_INIT_H

/* The configuration header file should be included first. */
#include <jasper/jas_config.h>

#include "jasper/jas_malloc.h"
#include "jasper/jas_image.h"
#include "jasper/jas_log.h"

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup init
 * @{
 */

#if defined(JAS_INTERNAL_USE_ONLY)
/*
User-configurable settings for library.
This is for internal library use only.
*/
typedef struct {

	/*
	A boolean flag indicating if the library has been configured
	by invoking the jas_conf_clear function.
	*/
	bool initialized;

	/*
	The allocator to be used by the library.
	*/
	jas_allocator_t *allocator;

	/*
	A boolean flag indicating if the allocator should be accessed through
	a wrapper that allows memory usage to be tracked and limited.
	*/
	bool enable_allocator_wrapper;

	/*
	The maximum amount of memory to be used by the library if the
	allocator wrapper is used.
	*/
	size_t max_mem;

	/*
	The jas_cleanup function should be invoked via atexit.
	It is not recommended that this be used, as it can cause many problems.
	*/
	bool enable_atexit_cleanup;

	/*
	The image format information to be used to populate the image format
	table in newly created contexts.
	*/
	const jas_image_fmt_t *image_formats;
	size_t num_image_formats;
	
	/*
	The maximum number of samples allowable in an image to be decoded to be
	used in newly created contexts.
	*/
	size_t dec_default_max_samples;

	/*
	The level of debugging checks/output enabled by the library for newly
	created contexts.
	A larger value corresponds to a greater level of debugging checks/output.
	*/
	int debug_level;

	/*
	The function used to output error/warning/informational messages
	for newly created contexts.
	*/
	int (*vlogmsgf)(jas_logtype_t type, const char *format, va_list ap);

} jas_conf_t;
#endif

#if defined(JAS_INTERNAL_USE_ONLY)
/* This is for internal library use only. */
jas_conf_t *jas_get_conf_ptr(void);
extern jas_conf_t jas_conf;
#endif

typedef struct {

	/* The maximum number of samples allowable in an image to be decoded. */
	size_t dec_default_max_samples;

	/*
	The level of debugging checks/output enabled by the library.
	A larger value corresponds to a greater level of debugging checks/output.
	*/
	int debug_level;

	/*
	The function used to output error/warning/informational messages.
	*/
	int (*vlogmsgf)(jas_logtype_t type, const char *format, va_list ap);

	/*
	The image format information to be used to populate the image format
	table.
	*/
	size_t image_numfmts;
	jas_image_fmtinfo_t image_fmtinfos[JAS_IMAGE_MAXFMTS];

} jas_ctx_t;

/*!
@brief
An opaque handle type used to represent a JasPer library context.
*/
typedef void *jas_context_t;

/******************************************************************************\
* Functions.
\******************************************************************************/

/*!
@brief Initialize the JasPer library with the current configuration settings.

@details
The library must be configured by invoking the @c jas_conf_clear function
prior to calling @c jas_initialize.

@returns
If the initialization of the library is successful, zero is returned;
otherwise, a nonzero value is returned.
*/
JAS_EXPORT
int jas_initialize(void);

/*!
@brief Configure and initialize the JasPer library using the default
configuration settings.

@details
The @c jas_init function initializes the JasPer library.
The library must be initialized before most code in the library can be used.

The @ jas_init function exists only for reasons of backward compatibility
with earlier versions of the library.
It is recommended that this function not be used.
Instead, the @c jas_conf_clear and @c jas_initialize functions should be used
to configure and initialize the library.

@returns
If the library is succesfully initialized, zero is returned;
otherwise, a nonzero value is returned.
*/
JAS_EXPORT
int jas_init(void);

/*!
@brief Perform any clean up for the JasPer library.

@details
This function performs any clean up for the JasPer library.
*/
JAS_EXPORT
void jas_cleanup(void);

/*!
@brief Configure the JasPer library with the default configuration settings.

@details
This function configures the JasPer library with the default configuration
settings.
These settings may be change via the @c jas_conf_* family of function
prior to invoking @c jas_initialize.
*/
JAS_EXPORT
void jas_conf_clear(void);

/*!
@brief Set the memory allocator to be used by the library.

@details
The object referenced by @c allocator must have a live at least
until @c jas_cleanup is invoked.
How the memory in which @c *allocator reside is allocated is the
responsibility of the caller.
*/
JAS_EXPORT
void jas_conf_set_allocator(jas_allocator_t *allocator);

/*!
@brief Set the memory allocator wrapper enabled flag.

@details
This function sets the value of the allocator wrapper enabled flag.
Setting this flag to true shall invoke the wrath of all of the world's
most-evil hackers.
It is strongly recommended that the allocator wrapper not be disabled
unless the memory allocator being used imposes an upper bound on memory
usage.
Not placing a bound on the amount of memory used by the JasPer library
would have many severe negative security implications.
*/
JAS_EXPORT
void jas_conf_set_allocator_wrapper(bool enable);

/*!
@brief Set the initial debug level for the library.
@details
*/
JAS_EXPORT
void jas_conf_set_debug_level(int debug_level);

/*!
@brief Set the maximum amount of memory that can be used by the library
(assuming the allocator wrapper is not disabled).

@details
*/
JAS_EXPORT
void jas_conf_set_max_mem(size_t max_mem);

/*!
@brief Set the default value for the maximum number of samples that is
allowed in an image to be decoded.

@details
*/
JAS_EXPORT
void jas_conf_set_dec_default_max_samples(size_t max_samples);

/*!
@brief Set the function used by the library to output error, warning, and
informational messages.

@details
*/
JAS_EXPORT
void jas_conf_set_vlogmsgf(int (*func)(jas_logtype_t, const char *,
  va_list));

/*!
@brief Set the image-format table to be used to initialize the library.

@details
The entries for the image-format table that reside in the array pointed to
by @c formats must have a lifetime that includes the point at which the
JasPer library is cleaned up (via @c jas_cleanup).
*/
JAS_EXPORT
void jas_conf_set_image_format_table(const jas_image_fmt_t *,
  size_t num_formats);

/*!
@brief
@details
*/
JAS_EXPORT
jas_context_t jas_context_create(void);

/*!
The context being destroyed must not be the current context.
*/
JAS_EXPORT
void jas_context_destroy(jas_context_t context);

/*!
@brief
Get the current context for the calling thread.
@details
*/
JAS_EXPORT
jas_context_t jas_get_context(void);

/*!
@brief
Set the current context for the calling thread.
@details
*/
JAS_EXPORT
void jas_set_context(jas_context_t context);

#if defined(JAS_INTERNAL_USE_ONLY)
/* This function is only for internal use by the library. */
static inline jas_ctx_t *jas_get_ctx(void)
{
	return JAS_CAST(jas_ctx_t *, jas_get_context());
}
#endif

/*!
@brief
Set the debug level for a particular context.
@details
*/
JAS_EXPORT
int jas_context_set_debug_level(jas_context_t context, int debug_level);

/*!
@brief
Get the debug level for a particular context.
@details
*/
static inline int jas_context_get_debug_level(jas_context_t context)
{
	jas_ctx_t *ctx = JAS_CAST(jas_ctx_t *, context);
	return ctx->debug_level;
}

/*!
@brief
Set the default maximum number of samples that a decoder is permitted to
process.

@details
*/
JAS_EXPORT
void jas_context_set_dec_default_max_samples(jas_context_t context,
  size_t max_samples);

/*!
@brief
Get the default maximum number of samples that a decoder is permitted to
process.
*/
static inline size_t jas_context_get_dec_default_max_samples(
  jas_context_t context)
{
	jas_ctx_t *ctx = JAS_CAST(jas_ctx_t *, context);
	return ctx->dec_default_max_samples;
}

/*!
@brief
Set the function to be used for log messages.
@details
*/
JAS_EXPORT
void jas_context_set_vlogmsgf(jas_context_t context,
  int (*func)(jas_logtype_t, const char *, va_list));

/*!
@brief
Get the function to be used for log messages.
@details
*/
static inline
int (*jas_context_get_vlogmsgf(jas_context_t context))(jas_logtype_t,
  const char *, va_list)
{
	jas_ctx_t *ctx = JAS_CAST(jas_ctx_t *, context);
	return ctx->vlogmsgf;
}

/*!
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
