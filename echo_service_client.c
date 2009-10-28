#include <dbus/dbus-glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-bindings.h>
#include "echo_service_client.h"
#include "echo_service_binding_client.h"

struct echo_client *echo_client_new()
{
    GError *error = NULL;

    struct echo_client *client = malloc(sizeof(struct echo_client));
    if (!client) {
	return NULL;
    }

    g_type_init();

    client->connection = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error);
    if (client->connection == NULL) {
	fprintf(stderr, "Failed to connect to session bus: %s\n",
		error ? error->message : "<no error provided>");
	g_error_free(error);
	free(client);
	return NULL;
    }

    client->proxy =
	dbus_g_proxy_new_for_name(client->connection,
				  "com.test.EchoService",
				  "/com/test/Platform",
				  "com.test.EchoService");
    if (client->proxy == NULL) {
	fprintf(stderr, "Failed to get a proxy (no error avail.)\n");
	free(client->connection);
	free(client);
	return NULL;
    }

    return client;
}

#if 0
int echo_client_stop(struct echo_client *echo_client)
{


/*
keesj@ijssijs:~/projects/dbus_glib_thread$ ./test/test_echo_calling_service keesj j
echo_service: echo_calling_service called with (in => 'keesj' , count => 0) 
process 4601: arguments to dbus_connection_unref() were incorrect, assertion "connection->generation == _dbus_current_generation" failed in file dbus-connection.c line 2658.
This is normally a bug in some application using the D-Bus library.
*** glibc detected *** ./echo_calling_service: free(): invalid pointer: 0x0000000000649640 ***
*/
// TODO : Why can't i unref the connnection
//      if (echo_client->connection) {
//              dbus_connection_unref (echo_client->connection);
//      }

//      if (echo_client->proxy) {
//              g_free(echo_client->proxy);
//      };
    return 0;			/* success */
}
#endif

char *echo_client_echo(struct echo_client *client, char *echo_data)
{
    GError *error = NULL;
    char *retval;


    if (com_test_EchoService_echo
	(client->proxy, echo_data, &retval, &error)) {
    } else {
	fprintf(stderr, "Error while getting the job_list %s\n",
		error ? error->message : "<no error provided>");
	g_error_free(error);
	return NULL;
    }
    return retval;

}
