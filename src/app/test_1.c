/*
 * Copyright (c) 2022 Michael David Adams.
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <stdint.h>

#include <jasper/jasper.h>

int alloc_test_main(int argc, char **argv);
int string_test_main(int argc, char **argv);

/******************************************************************************\
* Main program.
\******************************************************************************/

int main(int argc, char **argv)
{
	int ret = 0;
	int debug = 0;

	jas_conf_clear();
	static jas_std_allocator_t allocator;
	jas_std_allocator_init(&allocator);
	jas_conf_set_allocator(&allocator.base);
	//jas_conf_set_max_mem_usage(max_mem);
	jas_conf_set_debug_level(debug);
	if (jas_init_library()) {
		fprintf(stderr, "cannot initialize JasPer library\n");
		return EXIT_FAILURE;
	}
	if (jas_init_thread()) {
		fprintf(stderr, "cannot initialize thread\n");
		return EXIT_FAILURE;
	}

	if (argc < 2) {
		abort();
	}

	if (!strcmp(argv[1], "alloc")) {
		ret = alloc_test_main(argc - 1, &argv[1]);
	} else if (!strcmp(argv[1], "string")) {
		ret = string_test_main(argc - 1, &argv[1]);
	} else {
	}

	jas_cleanup_thread();
	jas_cleanup_library();

	return ret ? EXIT_FAILURE : EXIT_SUCCESS;
}

int alloc_test_main(int argc, char **argv)
{
	JAS_UNUSED(argc);
	JAS_UNUSED(argv);
	char *buffer;
	char *new_buffer;
	buffer = jas_malloc(7);
	assert(buffer);
	strcpy(buffer, "Hello!");
	printf("%s\n", buffer);
	new_buffer = jas_realloc(buffer, 100);
	assert(new_buffer);
	buffer = new_buffer;
	printf("%s\n", buffer);
	assert(buffer);
	jas_free(buffer);
	
	return 0;
}

int string_test_main(int argc, char **argv)
{
	JAS_UNUSED(argc);
	JAS_UNUSED(argv);
	char **tokens;
	size_t max_tokens;
	size_t num_tokens;
	char delim[] = " ";
	if (jas_string_tokenize("This is a test.", delim, &tokens, &max_tokens,
	  &num_tokens)) {
		return 1;
	};
	printf("tokens %p; max_tokens %zu; num_tokens %zu\n",
	  JAS_CAST(void *, tokens), max_tokens, num_tokens);
	if (tokens) {
		for (int i = 0; i < num_tokens; ++i) {
			printf("%s\n", tokens[i]);
			jas_free(tokens[i]);
		}
		jas_free(tokens);
	}
	return 0;
}
