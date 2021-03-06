#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-bindings.h>
#include <alib/log.h>

#include "echo_service_client.h"

struct echo_client *echo_client = NULL;

gboolean echo_calling_service_call_echo(GObject * obj, char *in, int count,
					GError ** error)
{
    char *retval;
    int i;

    log(LOG_NOTICE,
	"echo_calling_service called with (in => '%s' , count => %i) ", in,
	count);
    char *buffer = malloc(100);
    if (!buffer) {
	fprintf(stderr, "Failed to alloc data %s\n", __FUNCTION__);
	//TODO do the quark thing to set error ??
	return FALSE;
    }

    for (i = 0; i < count; i++) {
	sprintf(buffer, "%s + echo data %i", in, i);
	retval = echo_client_echo(echo_client, buffer);
	if (strncmp(buffer, retval, 100)) {
	    printf("echo test failed %s %s\n", buffer, retval);
	    fprintf(stderr, "echo test failed %s != %s\n", buffer, retval);
	    //TODO do the quark thing to set error ??
	    free(buffer);
	    return FALSE;
	}
	free(retval);
    }
    free(buffer);
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

    dbus_threads_init_default();
    g_type_init();

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
    err = run_main_loop();
    if (err < 0) {
	log(LOG_ERR, "Can't run main loop\n");
	exit(1);
    }

    return 0;
}
