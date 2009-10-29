#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-bindings.h>
#include <alib/log.h>
#include <alib/thread.h>

#include "echo_service_client.h"

struct echo_client *echo_client = NULL;

struct thread *thread;
int thread_will_stop;
int thread_run_again;
int thread_started;

void *thread_run_method(void *arg)
{
    char *retval;
    int i;
    char *in = "thread";
    int count = 1000;

    thread_comm_lock(thread);
    thread_will_stop = 0;
    thread_comm_unlock(thread);
    while (!thread_will_stop) {
	log(LOG_NOTICE,
	    "echo_calling_service called with (in => '%s' , count => %i) ",
	    in, count);
	char *buffer = malloc(100);
	if (!buffer) {
	    fprintf(stderr, "Failed to alloc data %s\n", __FUNCTION__);
	    pthread_exit(NULL);
	}

	for (i = 0; i < count; i++) {
	    sprintf(buffer, "%s + echo data %i", in, i);
	    retval = echo_client_echo(echo_client, buffer);
	    if (strncmp(buffer, retval, 100)) {
		printf("echo test failed %s %s\n", buffer, retval);
		fprintf(stderr, "echo test failed %s != %s\n", buffer,
			retval);
		//TODO do the quark thing to set error ??
		pthread_exit(NULL);
	    }
	    free(retval);
	}
	thread_comm_lock(thread);
	/** inside the lock we look if where where kicked during our last run and if so 
	we run again */
	if (thread_run_again == 1) {
	    thread_will_stop = 0;
	    thread_run_again = 0;
	} else {
	    thread_will_stop = 1;
	}
	thread_comm_unlock(thread);
    }
    pthread_exit(NULL);
}


gboolean echo_calling_service_call_echo(GObject * obj, char *in, int count,
					GError ** error)
{
    int wait_for_finish = 0;

    thread_comm_lock(thread);
    /* if thread is running notify it must run again we will do that later */
    if (thread_will_stop == 1) {
	wait_for_finish = 1;
    } else {			/* apparently we are still runnning so just update the request update */
	log(LOG_NOTICE, "kicking the echoing thread\n");
	thread_run_again = 1;
    }
    thread_comm_unlock(thread);

    if (wait_for_finish) {
	if (!thread_started) {
	    thread_comm_lock(thread);
	    thread_started = 1;	/* we start "unstarted */
	    thread_comm_unlock(thread);
	    log(LOG_NOTICE, "Starting the prosessing thread\n");
	    thread_start(thread);
	} else {
	    log(LOG_NOTICE, "kicking the prosessing thread\n");
	    int retval = 0;
	    retval = thread_join(thread);
	    if (retval == 0) {
		/* start the thread again */
		thread_start(thread);
	    } else {
		log(LOG_WARNING,
		    "Failed to wait for a thread to be finished\n");
		return FALSE;
	    }
	}
    }
    return TRUE;
}

#include "echo_calling_service_binding_server.h"

int verbose = 0;

DBusGConnection *connection;
DBusGProxy *proxy;
GMainLoop *loop;
GSource *timeout;

int process_args(int argc, char *argv[])
{
    int i, ret = 0;
    for (i = 1; i < argc; i++) {
	if (strcmp(argv[i], "-v") == 0) {
	    verbose = 1;
	} else {
	    fprintf(stderr, "Invalid argument: %s\n", argv[i]);
	    ret = -1;
	}
    }
    return ret;
}

void usage(const char *progname)
{
    fprintf(stderr,
	    "echo_calling_service\n"
	    "Usage: %s [ options... ]\n"
	    "    options:\n"
	    "    -v                  Be verbose\n", progname);
}

int init_glib_dbus(char *namespace, char *path,
		   const DBusGObjectInfo * dBusGMethodInfo)
{
    GError *error = NULL;
    guint32 err, ret;
    GObject *handle;

    g_type_init();
    g_thread_init(NULL);
    dbus_g_thread_init();
    //dbus_threads_init_default(); still needed? */
    handle = g_object_new(G_TYPE_OBJECT, NULL);
    if (handle == NULL) {
	log(LOG_ERR, "Can't create handle\n");
	return -1;
    }

    connection = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error);
    if (connection == NULL) {
	log(LOG_ERR, "Can't connect to system bus: %s\n", error->message);
	g_error_free(error);
	return -1;
    }

    proxy = dbus_g_proxy_new_for_name(connection, DBUS_SERVICE_DBUS,
				      DBUS_PATH_DBUS, DBUS_INTERFACE_DBUS);
    if (proxy == NULL) {
	log(LOG_ERR, "Failed to get a proxy\n");
	return -1;
    }

    err = org_freedesktop_DBus_request_name(proxy, namespace,
					    0, &ret, &error);
    if (!err) {
	log(LOG_ERR, "Error requesting name: %s\n",
	    error ? error->message : "?");
	return -1;
    }

    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
	/* Someone else requested the name before us */
	log(LOG_ERR, "Name already requested?\n");
	return -1;
    }

    dbus_g_object_type_install_info(G_TYPE_OBJECT, dBusGMethodInfo);

    dbus_g_connection_register_g_object(connection, path,
					G_OBJECT(handle));

    return 0;
}

gboolean check_fct(gpointer data)
{
    return TRUE;
}

int run_main_loop()
{
    /* TODO 1 */
    g_timeout_add_seconds(1, check_fct, NULL);

    loop = g_main_loop_new(NULL, FALSE);
    log(LOG_NOTICE, "EchoCallingService is now running...");
    g_main_loop_run(loop);
    return 0;
}

int main(int argc, char *argv[])
{
    int err;

    err = process_args(argc, argv);
    if (err < 0) {
	usage(argv[0]);
	exit(1);
    }

    init_log("echo_service", verbose);

    err =
	init_glib_dbus("com.test.EchoCallingService",
		       "/com/test/Platform",
		       &dbus_glib_echo_calling_service_object_info);
    if (err < 0) {
	log(LOG_ERR, "Can't initialize Dbus subsystem\n");
	exit(1);
    }

    echo_client = echo_client_new();
    thread = thread_new(thread_run_method, NULL);
    thread_will_stop = 1;
    thread_started = 0;
    thread_run_again = 0;


    err = run_main_loop();
    if (err < 0) {
	log(LOG_ERR, "Can't run main loop\n");
	exit(1);
    }

    return 0;
}
