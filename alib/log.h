#ifndef __LOG_H__
#define __LOG_H__
#include <syslog.h>

#ifdef __cplusplus
extern "C" {
#endif


void init_log(const char *name, int verbose);
#define log syslog

#ifdef __cplusplus
}
#endif
#endif
