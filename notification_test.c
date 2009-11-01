#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-bindings.h>
#include <alib/log.h>


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
		"notification_test \n"
		"Usage: %s [ options... ]\n"
		"    options:\n"
		"    -v                  Be verbose\n", progname);
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
	log(LOG_NOTICE, "notification_test is now running...");
	g_main_loop_run(loop);
	return 0;
}

void test_callback(){
	log(LOG_NOTICE, "CALLED");
}

int main(int argc, char *argv[])
{
	GError *error = NULL;
	gint32 err;
	g_type_init();
	g_thread_init(NULL);


	err = process_args(argc, argv);
	if (err < 0) {
		usage(argv[0]);
		exit(1);
	}

	dbus_g_thread_init();
	dbus_threads_init_default();
	init_log("notifucation_test", verbose);


	connection = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error);
	if (connection == NULL) {
		log(LOG_ERR, "Can't connect to system bus: %s\n",
		    error->message);
		g_error_free(error);
		return -1;
	}

	proxy = dbus_g_proxy_new_for_name(connection, "com.test.Notification",
	proxy = dbus_g_proxy_new_for_name(connection, "com.test.Notification",
					  "/com/test/Notification", "com.test.Notification");
        dbus_g_proxy_add_signal(proxy, "Notify",G_TYPE_INVALID);
        dbus_g_proxy_connect_signal(proxy, "Notify", test_callback, NULL, NULL);

//        proxy = dbus_g_proxy_new_for_name(connection, "org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus");

 //       dbus_g_proxy_add_signal(proxy, "NameOwnerChanged", G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING, G_TYPE_INVALID);
  //      dbus_g_proxy_connect_signal(proxy, "NameOwnerChanged", test_callback, NULL, NULL);

	err = run_main_loop();
	if (err < 0) {
		log(LOG_ERR, "Can't run main loop\n");
		exit(1);
	}

	return 0;
}
