/******************************************************************************\
* Includes.
\******************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include <jasper/jasper.h>

/******************************************************************************\
* Some dummy code.
\******************************************************************************/

int main(int argc, char **argv)
{
	jas_conf_clear();
	if (jas_init_library()) {
		fprintf(stderr, "cannot initialize JasPer library\n");
		return EXIT_FAILURE;
	}
	if (jas_init_thread()) {
		fprintf(stderr, "cannot initialize thread\n");
		return EXIT_FAILURE;
	}
	jas_cleanup_thread();
	jas_cleanup_library();
	return 0;
}
