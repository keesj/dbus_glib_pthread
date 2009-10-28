#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "process.h"


/** 
 * MO Master Out 
 * MI Master In
 * SO Slave out
 * SI Slave In
 **/
#define MO 1
#define MI 0
#define SI 0
#define SO 1

struct process *process_new(char *cmd, ...)
{

    struct process *p = malloc(sizeof(struct process));
    va_list arglist;

    int child_parent_fds[2];
    int parent_child_fds[2];

    pipe(child_parent_fds);
    pipe(parent_child_fds);

    if ((p->pid = fork()) == -1) {
	perror("fork");
	return NULL;
    }

    if (p->pid == 0) {
	close(STDIN_FILENO);
	dup(parent_child_fds[SI]);

	close(STDOUT_FILENO);
	dup(child_parent_fds[SO]);

	close(child_parent_fds[SI]);
	close(parent_child_fds[SO]);

	 /** The following is a bit of a hack but I don't know how to do this better 
	  * We want to call execl using variable args but fail here. This is why we
 	  * convert the list to a static array
   	  **/
	int size = 0;
	va_start(arglist, cmd);
	while (va_arg(arglist, char *) != NULL) {
	    size++;
	};
	va_end(arglist);
	char **data = malloc(sizeof(char *) * size + 1);
	size = 0;
	va_start(arglist, cmd);
	while ((data[size] = va_arg(arglist, char *)) != NULL) {
	    size++;
	};
	va_end(arglist);
	data[size] = NULL;
	execv(cmd, data);
	perror("process");
    }
    close(child_parent_fds[MO]);
    close(parent_child_fds[MI]);
    p->stdin = parent_child_fds[MO];
    p->stdout = child_parent_fds[MI];
    return p;
};
