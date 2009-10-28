#ifndef __PROCESS_H__
#define __PROCESS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* structure definine a process a launched by the proccess_new method */
struct process {
    int stdin;
    int stdout;
    pid_t pid;
};


/**
 * returns a new process struct.
 * cmd the command name (can be a command on the path)
 * the arguments of the program the fist being the program name
 * ane the rest arguments (NULL terminated)
 **/
struct process *process_new(char *cmd, ...);
#ifdef __cplusplus
}
#endif
#endif
