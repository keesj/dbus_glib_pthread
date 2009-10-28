#include <dbus/dbus-glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-bindings.h>


struct echo_client {
    DBusGConnection *connection;
    DBusGProxy *proxy;
};

struct echo_client *echo_client_new();
/* return a new joblist from the echo */
char *echo_client_echo(struct echo_client *client, char *echo);
