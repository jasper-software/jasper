/*
 * Copyright (c) 1999-2000 Image Power, Inc. and the University of
 *   British Columbia.
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

/*
 * Memory Allocator
 *
 * $Id$
 */

/******************************************************************************\
* Includes.
\******************************************************************************/

#define JAS_INTERNAL_USE_ONLY

#include "jasper/jas_malloc.h"
#include "jasper/jas_debug.h"
#include "jasper/jas_math.h"

#include <stdio.h>
#include <stdlib.h>

/* We need the prototype for memset. */
#include <string.h>

/******************************************************************************\
* Memory Allocator Configuration.
\******************************************************************************/

/* The memory allocator object to be used for all memory allocation. */
jas_allocator_t jas_allocator = {
	.alloc = 0,
	.free = 0,
	.realloc = 0
};

void jas_set_allocator(const jas_allocator_t *allocator)
{
	assert(!jas_allocator.alloc && !jas_allocator.free &&
	  !jas_allocator.realloc);
	if (!allocator) {
#if defined(JAS_USE_STD_ALLOCATOR_BY_DEFAULT)
		jas_allocator.alloc = malloc;
		jas_allocator.free = free;
		jas_allocator.realloc = realloc;
#else
		jas_allocator.alloc = jas_bma_alloc;
		jas_allocator.free = jas_bma_free;
		jas_allocator.realloc = jas_bma_realloc;
#endif
	} else {
		jas_allocator = *allocator;
	}
}

JAS_DLLEXPORT
void jas_get_allocator(jas_allocator_t *allocator)
{
	assert(allocator);
	*allocator = jas_allocator;
}

/******************************************************************************\
* Basic memory allocation and deallocation primitives.
\******************************************************************************/

JAS_DLLEXPORT
void *jas_malloc(size_t size)
{
	void *result;
	JAS_DBGLOG(101, ("jas_malloc(%zu)\n", size));
	result = (jas_allocator.alloc)(size);
	JAS_DBGLOG(100, ("jas_malloc(%zu) -> %p\n", size, result));
	return result;
}

JAS_DLLEXPORT
void *jas_realloc(void *ptr, size_t size)
{
	void *result;
	JAS_DBGLOG(101, ("jas_realloc(%p, %zu)\n", ptr, size));
	result = (jas_allocator.realloc)(ptr, size);
	JAS_DBGLOG(100, ("jas_realloc(%p, %zu) -> %p\n", ptr, size, result));
	return result;
}

JAS_DLLEXPORT
void jas_free(void *ptr)
{
	JAS_DBGLOG(100, ("jas_free(%p)\n", ptr));
	(jas_allocator.free)(ptr);
}

/******************************************************************************\
* Memory Allocator That Tracks Memory Usage.
\******************************************************************************/

static size_t jas_mem = 0;

#if defined(JAS_DEFAULT_MAX_MEM_USAGE)
static size_t jas_max_mem = JAS_DEFAULT_MAX_MEM_USAGE;
#else
static size_t jas_max_mem = 0;
#endif

#define JAS_BMA_MAGIC 0xdeadbeefULL

typedef struct {
	unsigned long long magic;
	size_t size;
} jas_mb_t;

#define JAS_MB_ADJUST \
  ((sizeof(jas_mb_t) + sizeof(max_align_t) - 1) / sizeof(max_align_t))
#define JAS_MB_SIZE (JAS_MB_ADJUST * sizeof(max_align_t))

//static jas_mb_t *jas_get_mb(void *ptr);
//static void *jas_mb_get_data(jas_mb_t *mb);

static void jas_mb_init(jas_mb_t *mb, size_t size)
{
	mb->magic = JAS_BMA_MAGIC;
	mb->size = size;
}

static void jas_mb_destroy(jas_mb_t *mb)
{
	mb->magic = 0;
	mb->size = 0;
}

static void *jas_mb_get_data(jas_mb_t *mb)
{
	assert(mb->magic == JAS_BMA_MAGIC);
	return JAS_CAST(void *, JAS_CAST(max_align_t *, mb) + JAS_MB_ADJUST);
}

static jas_mb_t *jas_get_mb(void *ptr)
{
	jas_mb_t *mb = JAS_CAST(jas_mb_t *,
	  JAS_CAST(max_align_t *, ptr) - JAS_MB_ADJUST);
	assert(mb->magic == JAS_BMA_MAGIC);
	/* This is one check that I do not want disabled with NDEBUG. */
	if (mb->magic != JAS_BMA_MAGIC) {abort();}
	return mb;
}

void jas_set_max_mem_usage(size_t max_mem)
{
	jas_max_mem = max_mem;
}

size_t jas_get_mem_usage()
{
	return jas_mem;
}

JAS_DLLEXPORT
void *jas_bma_alloc(size_t size)
{
	void *result;
	jas_mb_t *mb;
	size_t ext_size;
	size_t mem;

	JAS_DBGLOG(100, ("jas_bma_alloc(%zu)\n", size));
#if defined(JAS_MALLOC_RETURN_NULL_PTR_FOR_ZERO_SIZE)
	if (!size) {
		return 0;
	}
#endif
	if (!jas_safe_size_add(size, JAS_MB_SIZE, &ext_size)) {
		jas_eprintf("requested memory size is too large\n");
		result = 0;
		mb = 0;
	} else if (!jas_safe_size_add(jas_mem, size, &mem) ||
	  mem > jas_max_mem) {
		jas_eprintf("maximum memory limit (%zu) would be exceeded\n",
		  jas_max_mem);
		result = 0;
		mb = 0;
	} else {
		JAS_DBGLOG(100, ("jas_bma_alloc: ext_size=%zu\n", ext_size));
		if ((mb = malloc(ext_size))) {
			jas_mb_init(mb, size); /* mb->size = size; */
			result = jas_mb_get_data(mb);
			jas_mem = mem;
		} else {
			result = 0;
		}
	}
	JAS_DBGLOG(99, ("jas_bma_alloc(%zu) -> %p (mb=%p)\n", size, result,
	  mb));
	JAS_DBGLOG(102, ("max_mem=%zu; mem=%zu\n", jas_max_mem, jas_mem));
	return result;
}

JAS_DLLEXPORT
void *jas_bma_realloc(void *ptr, size_t size)
{
	void *result;
	jas_mb_t *mb;
	jas_mb_t *old_mb;
	size_t old_size;
	size_t ext_size;
	size_t mem;

	JAS_DBGLOG(100, ("jas_bma_realloc(%p, %zu)\n", ptr, size));
	if (!ptr) {
		return jas_bma_alloc(size);
	}
	if (ptr && !size) {
		jas_bma_free(ptr);
	}
	if (!jas_safe_size_add(size, JAS_MB_SIZE, &ext_size)) {
		jas_eprintf("requested memory size is too large\n");
		return 0;
	}
	old_mb = jas_get_mb(ptr);
	old_size = old_mb->size;
	JAS_DBGLOG(101, ("jas_bma_realloc: old_mb=%p; old_size=%zu\n", old_mb,
	  old_size));
	if (size > old_size) {
		if (!jas_safe_size_add(jas_mem, size - old_size, &mem) ||
		  mem > jas_max_mem) {
			jas_eprintf("maximum memory limit (%zu) would be exceeded\n",
			  jas_max_mem);
			return 0;
		}
	} else {
		if (!jas_safe_size_sub(jas_mem, old_size - size, &jas_mem)) {
			jas_eprintf("heap corruption detected\n");
			abort();
		}
	}
	JAS_DBGLOG(100, ("jas_bma_realloc: realloc(%p, %zu)\n", old_mb,
	  ext_size));
	jas_mb_destroy(old_mb);
	if (!(mb = realloc(old_mb, ext_size))) {
		result = 0;
	} else {
		jas_mb_init(mb, size); /* mb->size = size; */
		result = jas_mb_get_data(mb);
		jas_mem = mem;
	}
	JAS_DBGLOG(100, ("jas_bma_realloc(%p, %zu) -> %p (%p)\n", ptr, size,
	  result, mb));
	JAS_DBGLOG(102, ("max_mem=%zu; mem=%zu\n", jas_max_mem, jas_mem));
	return result;
}

JAS_DLLEXPORT
void jas_bma_free(void *ptr)
{
	jas_mb_t *mb;
	size_t size;
	JAS_DBGLOG(100, ("jas_bma_free(%p)\n", ptr));
	if (ptr) {
		mb = jas_get_mb(ptr);
		size = mb->size;
		JAS_DBGLOG(101, ("jas_bma_free(%p) (mb=%p; size=%zu)\n", ptr, mb,
		  size));
		if (!jas_safe_size_sub(jas_mem, size, &jas_mem)) {
			jas_eprintf("heap corruption detected\n");
			abort();
		}
		JAS_DBGLOG(100, ("jas_bma_free: free(%p)\n", mb));
		jas_mb_destroy(mb);
		free(mb);
	}
	JAS_DBGLOG(102, ("max_mem=%zu; mem=%zu\n", jas_max_mem, jas_mem));
}

/******************************************************************************\
* Additional memory allocation and deallocation primitives
* (mainly for overflow checking).
\******************************************************************************/

void *jas_calloc(size_t num_elements, size_t element_size)
{
	void *ptr;
	size_t size;
	if (!jas_safe_size_mul(num_elements, element_size, &size)) {
		return 0;
	}
	if (!(ptr = jas_malloc(size))) {
		return 0;
	}
	memset(ptr, 0, size);
	return ptr;
}

void *jas_alloc2(size_t num_elements, size_t element_size)
{
	size_t size;
	if (!jas_safe_size_mul(num_elements, element_size, &size)) {
		return 0;
	}
	return jas_malloc(size);
}

void *jas_alloc3(size_t num_arrays, size_t array_size, size_t element_size)
{
	size_t size;
	if (!jas_safe_size_mul(array_size, element_size, &size) ||
	  !jas_safe_size_mul(size, num_arrays, &size)) {
		return 0;
	}
	return jas_malloc(size);
}

void *jas_realloc2(void *ptr, size_t num_elements, size_t element_size)
{
	size_t size;
	if (!jas_safe_size_mul(num_elements, element_size, &size)) {
		return 0;
	}
	return jas_realloc(ptr, size);
}
