#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>

#include "log.h"

void init_log(const char *name, int verbose)
{
    int flags = LOG_CONS;	    /* Log to console if we failed to open syslog */
    if (verbose) {
	 /* If verbose mode, log ALSO to stderr */
	 flags |= LOG_PERROR;
    }
    openlog(name, flags, LOG_DAEMON);
}
