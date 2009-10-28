#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "sb.h"

void sb_free(struct sb *sb)
{
    if (!sb) {
	 return;
    }
    if (sb->data) {
	 free(sb->data);
    }
    free(sb);
}

struct sb *sb_new(int initial_size)
{
    struct sb *s_buffer = (struct sb *) malloc(sizeof(struct sb));
    if (!s_buffer) {
	 return NULL;
    }
    memset(s_buffer, 0, sizeof(struct sb));
    s_buffer->data = (char *) malloc(initial_size);
    if (!s_buffer->data) {
	 free(s_buffer);
	 return NULL;
    }
    s_buffer->size = initial_size;
    s_buffer->size_written = 0;
    return s_buffer;
}

/* snprintf for a sb */
int sb_write(struct sb *sb, char *fmt, ...)
{
    int written;
    va_list arglist;

    int ok = 0;
    do {
	 va_start(arglist, fmt);
	 written =
	     vsnprintf(&sb->data[sb->size_written],
			 sb->size - sb->size_written, fmt, arglist);
	 /* did we try to write more memory than we have -> grow */
	 if (written > sb->size - sb->size_written) {
	     /* Grow the data array by 2 */
	     sb->data = (char *) realloc(sb->data, sb->size * 2);
	     if (!sb->data) {
		  /* set perror? */
		  return 0;
	     }
	     sb->size *= 2;
	     ok = 0;
	 } else {
	     ok = 1;
	 }
	 va_end(arglist);
    }
    while (!ok);
    sb->size_written += written;

    return written;
}
