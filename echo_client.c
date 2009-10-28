#include <dbus/dbus-glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-bindings.h>
#include "echo_service_client.h"

int main(int argc, char **argv)
{
    int counter = 0;
    char *retval;
    struct echo_client *client = echo_client_new();

    char *buffer = malloc(100);
    if (!buffer) {
	fprintf(stderr, "Failed to alloc data %s\n", __FUNCTION__);
	return 1;		/* */
    }

    for (counter = 0; counter < 10000; counter++) {
	sprintf(buffer, "echo data %i", counter);
	retval = echo_client_echo(client, buffer);
	if (strncmp(buffer, retval, 100)) {
	    printf("echo test failed %s %s\n", buffer, retval);
	    fprintf(stderr, "echo test failed %s != %s\n", buffer, retval);
	    return 1;		/* */
	}
	free(retval);
    }
    return 0;			/* success */
}
