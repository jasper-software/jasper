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

/*
 * JasPer Transcoder Program
 *
 * $Id$
 */

/******************************************************************************\
* Includes.
\******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <unistd.h>

#include <jasper/jasper.h>

/******************************************************************************\
*
\******************************************************************************/

typedef struct {
	int id;
	const char *in_file;
	const char *out_format_str;
	const char *out_file;
	jas_thread_t thread;
	size_t num_iters;
	int debug_level;
	int verbose;
} job_t;

/******************************************************************************\
* Code.
\******************************************************************************/

#define max_jobs 256
int num_jobs = 0;
jas_thread_t threads[max_jobs];
job_t jobs[max_jobs];

void usage(void);
int process_job(void *job_handle);

void usage()
{
	fprintf(stderr, "bad usage\n");
	exit(1);
}

int process_job(void *job_handle)
{
	job_t *job = JAS_CAST(job_t *, job_handle);
	jas_image_t *image;

	int result = 0;

	jas_context_t context = 0;
	if (!(context = jas_context_create())) {
		fprintf(stderr, "cannot create context\n");
		exit(EXIT_FAILURE);
	}
	jas_context_set_debug_level(context, job->debug_level);
	jas_context_set_vlogprintf(context, jas_vlogprintf_stderr);
	jas_set_context(context);

	for (int iter_no = 0; iter_no < JAS_CAST(int, job->num_iters); ++iter_no) {

		if (job->verbose >= 1) {
			fprintf(stderr, "iteration %d\n", iter_no);
		}

		/* Get the input image data. */
		jas_stream_t *in;
		jas_stream_t *out;
		if (!(in = jas_stream_fopen(job->in_file, "r"))) {
			fprintf(stderr, "cannot open file %s\n", job->in_file);
			result = -1;
			goto done;
		}
		if (!(image = jas_image_decode(in, -1, ""))) {
			fprintf(stderr, "error: cannot load image data\n");
			result = -1;
			goto done;
		}

		if (!(out = jas_stream_fopen(job->out_file, "w"))) {
			fprintf(stderr, "cannot open output file %s\n", job->out_file);
			result = -1;
			goto done;
		}
		int out_format = jas_image_strtofmt(job->out_format_str);
		if (out_format < 0) {
			fprintf(stderr, "error: cannot get image format\n");
			result = -1;
			goto done;
		}
		if (jas_image_encode(image, out, out_format, "")) {
			fprintf(stderr, "error: cannot encode image\n");
			result = -1;
			goto done;
		}
		jas_stream_flush(out);
		(void) jas_stream_close(in);

		if (jas_stream_close(out)) {
			fprintf(stderr, "error: cannot close output image file\n");
			result = -1;
			goto done;
		}
		jas_image_destroy(image);
	}

done:

	if (context) {
		jas_set_context(0);
		jas_context_destroy(context);
		context = 0;
	}

	return result;
}

int main(int argc, char **argv)
{
	int verbose = 0;
	int debug_level = 0;
	size_t max_mem = 1024ull * 1024ull * 1024ull;
	size_t num_iters = 10;
	int repeat = 1;
	const char *out_format = "jp2";
	bool use_wrapper = true;

	int c;
	while ((c = getopt(argc, argv, "wvl:n:r:Xf:")) != EOF) {
		switch (c) {
		case 'w':
			use_wrapper = false;
			break;
		case 'f':
			out_format = optarg;
			break;
		case 'X':
			num_iters = 10000;
			repeat = 4;
			break;
		case 'v':
			++verbose;
			break;
		case 'r':
			repeat = atoi(optarg);
			break;
		case 'n':
			num_iters = atoi(optarg);
			break;
		case 'l':
			debug_level = atoi(optarg);
			break;
		default:
			usage();
			break;
		}
	}

	fprintf(stderr, "THREADS: %s\n", JAS_THREADS_IMPL);

	job_t *job = jobs;
	num_jobs = 0;
	int i = optind;
	while (i < argc && num_jobs < max_jobs) {
		for (int j = 0; j < repeat; ++j) {
			if (i >= argc) {
				break;
			}
			job->in_file = argv[i];
			job->out_file = "/dev/null";
			job->out_format_str = out_format;
			job->num_iters = num_iters;
			job->id = num_jobs;
			job->debug_level = (num_jobs == 0) ? debug_level : 0;
			if (job->verbose >= 2) {
				job->debug_level = debug_level;
			}
			job->verbose = verbose;
			++job;
			++num_jobs;
		}
		++i;
	}

	fprintf(stderr, "number of jobs: %d\n", num_jobs);
	fprintf(stderr, "number of iterations: %zu\n", num_iters);

#if defined(JAS_USE_JAS_INIT)

	JAS_UNUSED(use_wrapper);
	if (jas_init()) {
		fprintf(stderr, "cannot initialize JasPer library\n");
		exit(EXIT_FAILURE);
	}
	jas_set_max_mem_usage(max_mem);
	jas_context_t context = jas_get_context();
	assert(context);
	jas_context_set_debug_level(context, 0);

#else

	jas_conf_clear();
	if (use_wrapper) {
		jas_conf_set_allocator_wrapper(true);
		jas_conf_set_max_mem(max_mem);
	} else {
		jas_conf_set_allocator_wrapper(false);
	}
	static jas_std_allocator_t allocator;
	jas_std_allocator_init(&allocator);
	jas_conf_set_allocator(&allocator.base);
	jas_conf_set_debug_level(0);

	if (jas_initialize()) {
		fprintf(stderr, "cannot initialize JasPer library\n");
		exit(EXIT_FAILURE);
	}
	atexit(jas_cleanup);

#endif

	for (int i = 0; i < num_jobs; ++i) {
		job_t *job = &jobs[i];
		if (jas_thread_create(&job->thread, process_job, JAS_CAST(void *,
		  job))) {
			fprintf(stderr, "cannot create thread\n");
			abort();
		}
	}

	int error_count = 0;
	for (int i = 0; i < num_jobs; ++i) {
		job_t *job = &jobs[i];
		int result;
		if (jas_thread_join(&job->thread, &result)) {
			fprintf(stderr, "cannot join thread\n");
			abort();
		}
		if (result) {
			fprintf(stderr, "job %d failed\n", i);
			++error_count;
		}
	}

	jas_set_context(0);
	//jas_cleanup();

	if (error_count) {
		fprintf(stderr, "ERRORS WERE DETECTED!\n");
		fprintf(stderr, "error count: %d\n", error_count);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
