#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "thread.h"

struct thread *thread_new(void *(*start_routine) (void *), void *data)
{
    struct thread *thread = malloc(sizeof(struct thread));
    thread->start_routine = start_routine;
    thread->data = data;
    pthread_mutex_init(&thread->thread_lock, NULL);
    pthread_mutex_init(&thread->thread_comm_lock, NULL);
    pthread_attr_init(&thread->thread_attr);
    return thread;
}

void thread_start(struct thread *thread)
{
    private_thread_lock(thread);
    pthread_create(&thread->pthread, &thread->thread_attr,
		   thread->start_routine, thread->data);
    private_thread_unlock(thread);
}

int thread_signal(struct thread *thread, int sig)
{
    return pthread_kill(thread->pthread, sig);
}

int thread_join(struct thread *thread)
{
    void ** value_ptr = NULL;
    int retval = 0;
    retval = thread_join_ptr(thread, value_ptr);
    if (value_ptr != NULL) {	/* UNTESTED!!!!!!!!!!!!!! */
	free(*value_ptr);
    }
    return retval;
}

int thread_join_ptr(struct thread *thread, void **value_ptr)
{
    return pthread_join(thread->pthread, value_ptr);
}

void thread_comm_lock(struct thread * thread){
    pthread_mutex_lock(&thread->thread_comm_lock);
}

void thread_comm_unlock(struct thread * thread){
    pthread_mutex_unlock(&thread->thread_comm_lock);
}

int protected_thread_stop(struct thread *thread)
{
    void *status;
    int retval = 0;
    private_thread_lock(thread);
    pthread_attr_destroy(&thread->thread_attr);
    retval = pthread_join(thread->pthread, &status);
    if (retval) {
	printf("ERROR; return code from pthread_join() is %d\n", retval);
    };
    private_thread_unlock(thread);
    return retval;

}

void thread_destroy(struct thread *thread)
{
    pthread_mutex_destroy(&thread->thread_lock);
}

void private_thread_lock(struct thread *thread)
{
    pthread_mutex_lock(&thread->thread_lock);
}

void private_thread_unlock(struct thread *thread)
{
    pthread_mutex_unlock(&thread->thread_lock);
}
