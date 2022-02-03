#include <stdio.h>
int main(void)
{
#ifdef __STDC__
	printf("%ldL", __STDC_VERSION__);
	if (fflush(stdout)) {
		return 1;
	}
	return 0;
#else
	return 1;
#endif
}
