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


/*!
 * @file jas_malloc.h
 * @brief JasPer Memory Allocator
 */

#ifndef JAS_MALLOC_H
#define JAS_MALLOC_H

/******************************************************************************\
* Includes.
\******************************************************************************/

/* The configuration header file should be included first. */
#include <jasper/jas_config.h>

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup malloc
 * @{
 */

/*!
@brief A memory allocator.
*/
typedef struct {

	/*!
	Function to allocate memory.
	This function should have behavior similar to malloc.
	*/
	void *(*alloc)(size_t);

	/*!
	Function to deallocate memory.
	This function should have behavior similar to free.
	*/
	void (*free)(void*);

	/*!
	Function to reallocate memory.
	This function should have behavior similar to realloc.
	*/
	void *(*realloc)(void*, size_t);

	/*! For future use. */
	void (*(reserved[5]))(void);

} jas_allocator_t;

/******************************************************************************\
* Functions.
\******************************************************************************/

/*
This function is for internal library use only.
It should never be called directly by a library user.
*/
void jas_set_allocator(const jas_allocator_t* allocator);

/*!
@brief
Get the memory allocator being used by the library.

@param allocator
a pointer to a buffer to hold the allocator information
*/
JAS_DLLEXPORT
void jas_get_allocator(jas_allocator_t* allocator);

/*!
@brief
Allocate memory.
*/
JAS_DLLEXPORT
void *jas_malloc(size_t size);

/*!
@brief
Free memory.
*/
JAS_DLLEXPORT
void jas_free(void *ptr);

/*!
@brief
Resize a block of allocated memory.
*/
JAS_DLLEXPORT
void *jas_realloc(void *ptr, size_t size);

/*!
@brief
Allocate a block of memory and initialize the contents to zero.
*/
JAS_DLLEXPORT
void *jas_calloc(size_t num_elements, size_t element_size);

/*!
@brief
Allocate array (with overflow checking).
*/
JAS_DLLEXPORT void *jas_alloc2(size_t num_elements, size_t element_size);

/*!
@brief
Allocate array of arrays (with overflow checking).
*/
JAS_DLLEXPORT void *jas_alloc3(size_t num_arrays, size_t array_size, size_t element_size);

/*!
@brief
Resize a block of allocated memory (with overflow checking).
*/
JAS_DLLEXPORT void *jas_realloc2(void *ptr, size_t num_elements, size_t element_size);

JAS_DLLEXPORT void jas_set_max_mem_usage(size_t max_mem);

JAS_ATTRIBUTE_PURE
JAS_DLLEXPORT
size_t jas_get_mem_usage(void);

/*
Basic memory allocator (BMA).
Allocate memory.
*/
JAS_DLLEXPORT
void *jas_bma_alloc(size_t size);

/*
Basic memory allocator (BMA).
Deallocate memory.
*/
JAS_DLLEXPORT
void jas_bma_free(void *ptr);

/*
Basic memory allocator (BMA).
Reallocate memory.
*/
JAS_DLLEXPORT
void *jas_bma_realloc(void *ptr, size_t size);

/*!
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
