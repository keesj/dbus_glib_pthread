#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

/* structure definine a process a launched by the proccess_new method */
struct thread {
	void *(*start_routine)(void*);
	void * data;
        /** Mutext for sharing data between The threads
         **/
        pthread_mutex_t thread_lock;
        pthread_mutex_t thread_comm_lock;
        pthread_t pthread;
        pthread_attr_t thread_attr;

};


struct thread *thread_new( void *(*start_routine)(void*) ,void * data);
void thread_destroy(struct thread * thread);

void thread_start(struct thread * thread);
int thread_signal(struct thread * thread , int sig);

/* return 0 on success */
int thread_join(struct thread * thread);
int thread_join_ptr(struct thread * thread, void ** value_ptr);

/* Generic "communication" lock to be used when using variables to determin the 
   flow of applicaions */
void thread_comm_lock(struct thread * thread);
void thread_comm_unlock(struct thread * thread);
/**
 *  This method waits for the thread to be finished
 *  By calling join on the thread. Only call this method
 *  if your thread is actually going to stop.
 *  @return 0 in case of success.
 **/
int protected_thread_stop(struct thread * thread);

void private_thread_lock(struct thread * thread);
void private_thread_unlock(struct thread * thread);

#endif
