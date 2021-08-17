/*
 * Copyright (c) 1999-2000 Image Power, Inc. and the University of
 *   British Columbia.
 * Copyright (c) 2001-2003 Michael David Adams.
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
 * @file jas_thread.h
 * @brief Threads
 */

#ifndef JAS_THREAD_H
#define JAS_THREAD_H

/******************************************************************************\
* Includes
\******************************************************************************/

/* The configuration header file should be included first. */
#include <jasper/jas_config.h>

#include "jasper/jas_compiler.h"
#include "jasper/jas_types.h"

#if defined(JAS_ENABLE_MULTITHREADING_SUPPORT)

#include <stdlib.h>
#include <assert.h>

#if defined(JAS_THREADS_C11)
#include <threads.h>
#elif defined(JAS_THREADS_PTHREAD)
#include <pthread.h>
#include <sched.h>
#elif defined(JAS_THREADS_MSVC)
#include <process.h>
#include <windows.h>
#endif

#endif

/******************************************************************************\
\******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup threads
 * @{
 */

#if defined(JAS_ENABLE_MULTITHREADING_SUPPORT)

#if defined(JAS_THREADS_C11)
#	define JAS_THREADS_IMPL "C11"
#elif defined(JAS_THREADS_PTHREAD)
#	define JAS_THREADS_IMPL "PTHREAD"
#elif defined(JAS_THREADS_MSVC)
#	define JAS_THREADS_IMPL "MSVC"
#endif

/*!  Thread type. */
#if defined(JAS_THREADS_C11)
typedef thrd_t jas_thread_t;
#elif defined(JAS_THREADS_PTHREAD)
typedef struct {
	pthread_t id;
	int result;
	void *arg;
	int (*func)(void *);
} jas_thread_t;
#elif defined(JAS_THREADS_MSVC)
typedef HANDLE jas_thread_t;
#endif

/*!  Thread ID type. */
#if defined(JAS_THREADS_C11)
typedef thrd_t jas_thread_id_t;
#elif defined(JAS_THREADS_PTHREAD)
typedef pthread_t jas_thread_id_t;
#elif defined(JAS_THREADS_MSVC)
typedef thrd_t jas_thread_id_t;
#endif

/*!  Mutex type. */
#if defined(JAS_THREADS_C11)
typedef mtx_t jas_mutex_t;
#elif defined(JAS_THREADS_PTHREAD)
typedef pthread_mutex_t jas_mutex_t;
#elif defined(JAS_THREADS_MSVC)
typedef CRITICAL_SECTION jas_mutex_t;
#endif

/*!  Thread-specific storage type. */
#if defined(JAS_THREADS_C11)
typedef tss_t jas_tss_t;
#elif defined(JAS_THREADS_PTHREAD)
typedef pthread_key_t jas_tss_t;
#elif defined(JAS_THREADS_MSVC)
typedef DWORD jas_tss_t;
#endif

/*!  Once-flag type. */
#if defined(JAS_THREADS_C11)
typedef once_flag jas_once_flag_t;
#elif defined(JAS_THREADS_PTHREAD)
typedef pthread_once_t jas_once_flag_t;
#elif defined(JAS_THREADS_MSVC)
typedef struct {
	volatile LONG status;
} jas_once_flag_t;
#endif

/*!  Once-flag initializer. */
#if defined(JAS_THREADS_C11)
#	define JAS_ONCE_FLAG_INIT ONCE_FLAG_INIT
#elif defined(JAS_THREADS_PTHREAD)
#	define JAS_ONCE_FLAG_INIT PTHREAD_ONCE_INIT
#elif defined(JAS_THREADS_MSVC)
#	define JAS_ONCE_FLAG_INIT {0}
#endif

#if defined(JAS_THREADS_PTHREAD)
static void *thread_func_wrapper(void *thread_ptr)
{
	jas_thread_t *thread = JAS_CAST(jas_thread_t *, thread_ptr);
	int result = (thread->func)(thread->arg);
	thread->result = result;
	return thread;
}
#endif

/*!
@brief Compare two thread IDs.
*/
static inline
int jas_thread_compare(jas_thread_id_t x, jas_thread_id_t y)
{
#if defined(JAS_THREADS_C11)
	return thrd_equal(x, y);
#elif defined(JAS_THREADS_PTHREAD)
	return pthread_equal(x, y);
#elif defined(JAS_THREADS_MSVC)
	return GetThreadId(x) == GetThreadId(y);
#endif
}

/*!
@brief Create a thread.
*/
static inline
int jas_thread_create(jas_thread_t *thread, int (*func)(void *), void *arg)
{
#if defined(JAS_THREADS_C11)
	return thrd_create(thread, func, arg) == thrd_success ? 0 : -1;
#elif defined(JAS_THREADS_PTHREAD)
	thread->func = func;
	thread->arg = arg;
	thread->result = 0;
	return pthread_create(&thread->id, 0, thread_func_wrapper, thread);
#elif defined(JAS_THREADS_MSVC)
	/* FIXME - NOT YET IMPLEMENTED. */
	return -1;
#endif
}

/*!
@brief Join a thread.
*/
static inline
int jas_thread_join(jas_thread_t *thread, int *result)
{
#if defined(JAS_THREADS_C11)
	return thrd_join(*thread, result) == thrd_success ? 0 : -1;
#elif defined(JAS_THREADS_PTHREAD)
	void *result_buf;
	int ret = pthread_join(thread->id, &result_buf);
	if (!ret) {
		jas_thread_t *other_thread = JAS_CAST(jas_thread_t *, result_buf);
		if (result) {
			/* A null pointer is probably a bug. */
			assert(other_thread);
			*result = other_thread ? other_thread->result : 0;
		}
	}
	return ret;
#elif defined(JAS_THREADS_MSVC)
	/* FIXME - NOT YET IMPLEMENTED. */
	return -1;
#endif
}

static inline void jas_thread_yield(void)
{
#if defined(JAS_THREADS_C11)
	thrd_yield();
#elif defined(JAS_THREADS_PTHREAD)
	sched_yield();
#elif defined(JAS_THREADS_MSVC)
	SwitchToThread();
#endif
}

#if 0
/* This functionality is not available for all threading support libraries. */
static inline
void jas_thread_exit(int result)
{
#if defined(JAS_THREADS_C11)
	thrd_exit(result);
#elif defined(JAS_THREADS_PTHREAD)
	/* This does not have a trivial implementation, as far as I can see. */
	/* There is no jas_thread_find function. */
	jas_thread_t *thread = jas_thread_find(pthread_self());
	thread->result = result;
	pthread_exit(JAS_CAST(void *, thread));
#endif
}
#endif

#if 0
/*!
@brief Get the ID of the calling thread.
*/
static inline
jas_thread_id_t jas_thread_current(void)
{
#if defined(JAS_THREADS_C11)
	return thrd_current();
#elif defined(JAS_THREADS_PTHREAD)
	return pthread_self();
#elif defined(JAS_THREADS_MSVC)
	/* FIXME - NOT YET IMPLEMENTED. */
	abort();
#endif
}
#endif

/*!
@brief Initialize a mutex.
*/
static inline int jas_mutex_init(jas_mutex_t *mtx)
{
#if defined(JAS_THREADS_C11)
	return mtx_init(mtx, mtx_plain) == thrd_success ? 0 : -1;
#elif defined(JAS_THREADS_PTHREAD)
	return pthread_mutex_init(mtx, 0);
#elif defined(JAS_THREADS_MSVC)
	InitializeCriticalSection(mtx);
	return 0;
#endif
}

/*!
@brief Cleanup a mutex.
*/
static inline int jas_mutex_cleanup(jas_mutex_t *mtx)
{
#if defined(JAS_THREADS_C11)
	mtx_destroy(mtx);
	return 0;
#elif defined(JAS_THREADS_PTHREAD)
	return pthread_mutex_destroy(mtx);
#elif defined(JAS_THREADS_MSVC)
	DeleteCriticalSection(mtx);
	return 0;
#endif
}

/*!
@brief Acquire a mutex.
*/
static inline int jas_mutex_lock(jas_mutex_t *mtx)
{
#if defined(JAS_THREADS_C11)
	return mtx_lock(mtx);
#elif defined(JAS_THREADS_PTHREAD)
	return pthread_mutex_lock(mtx);
#elif defined(JAS_THREADS_MSVC)
	EnterCriticalSection(mtx);
	return 0;
#endif
}

/*!
@brief Release a mutex.
*/
static inline int jas_mutex_unlock(jas_mutex_t *mtx)
{
#if defined(JAS_THREADS_C11)
	return mtx_unlock(mtx);
#elif defined(JAS_THREADS_PTHREAD)
	return pthread_mutex_unlock(mtx);
#elif defined(JAS_THREADS_MSVC)
	LeaveCriticalSection(mtx);
	return 0;
#endif
}

/*!
@brief Create thread-specific storage.
*/
static inline
int jas_tss_create(jas_tss_t *tss, void (*destructor)(void *))
{
#if defined(JAS_THREADS_C11)
	return tss_create(tss, destructor) == thrd_success ? 0 : -1;
#elif defined(JAS_THREADS_PTHREAD)
	return pthread_key_create(tss, destructor);
#elif defined(JAS_THREADS_MSVC)
	if (destructor) {
		return -1;
	}
	*tss = TlsAlloc();
	return (*tss != 0xffffffff);
#endif
}

/*!
@brief Delete thread-specific storage.
*/
static inline
void jas_tss_delete(jas_tss_t tss)
{
#if defined(JAS_THREADS_C11)
	tss_delete(tss);
#elif defined(JAS_THREADS_PTHREAD)
	pthread_key_delete(tss);
#elif defined(JAS_THREADS_MSVC)
	TlsFree(tss);
#endif
}

/*!
@brief Get the thread-specific storage instance for the calling thread.
*/
static inline
void *jas_tss_get(jas_tss_t tss)
{
#if defined(JAS_THREADS_C11)
	return tss_get(tss);
#elif defined(JAS_THREADS_PTHREAD)
	return pthread_getspecific(tss);
#elif defined(JAS_THREADS_MSVC)
	return TlsGetValue(key);
#endif
}

/*!
@brief Set the thread-specific storage instance for the calling thread.
*/
static inline
int jas_tss_set(jas_tss_t tss, void *value)
{
#if defined(JAS_THREADS_C11)
	return tss_set(tss, value) == thrd_success ? 0 : -1;
#elif defined(JAS_THREADS_PTHREAD)
	return pthread_setspecific(tss, value);
#elif defined(JAS_THREADS_MSVC)
	return TlsSetValue(tss, value) ? 0 : -1;
#endif
}

/*!
@brief Register to call a function once.
*/
static inline int jas_call_once(jas_once_flag_t *flag, void (*func)(void))
{
#if defined(JAS_THREADS_C11)
	call_once(flag, func);
	return 0;
#elif defined(JAS_THREADS_PTHREAD)
	return pthread_once(flag, func);
#elif defined(JAS_THREADS_MSVC)
	if (InterlockedCompareExchange(&flag->status, 1, 0) == 0) {
		(func)();
		InterlockedExchange(&flag->status, 2);
	} else {
		while (flag->status == 1) {
			/* Perform a busy wait.  This is ugly. */
			jas_thread_yield();
		}
	}
#endif
}

#else

/******************************************************************************\
* No Threading Support.
\******************************************************************************/

#endif

/*!
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
