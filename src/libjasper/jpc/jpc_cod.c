#include <jasper/jas_config.h>
#include "jasper/jas_thread.h"
#include "jasper/jas_debug.h"

#include "jpc_cod.h"

static void jpc_init_helper(void);

#if defined(JAS_THREADS)
jas_once_flag_t jpc_init_once = JAS_ONCE_FLAG_INIT;
#else
int jpc_init_once = 0;
#endif

void jpc_init(void)
{
#if defined(JAS_THREADS)
	jas_call_once(&jpc_init_once, jpc_init_helper);
#else
	if (!jpc_init_once) {
		jpc_init_helper();
		jpc_init_once = 1;
	}
#endif
}

static void jpc_init_helper(void)
{
	JAS_LOGDEBUGF(10, "jpc_init_helper called\n");
	jpc_initmqctxs();
	jpc_initluts();
}
