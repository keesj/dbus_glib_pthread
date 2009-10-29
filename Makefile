CFLAGS ?= -g
CFLAGS += -Wall
 
PKG_CONFIG ?= pkg-config
PKGS = glib-2.0 gthread-2.0 dbus-1 dbus-glib-1

CFLAGS += `$(PKG_CONFIG) --cflags $(PKGS)`
LDLIBS += `$(PKG_CONFIG) --libs $(PKGS)`


LDLIBS +=  -Lalib -la
CFLAGS += -I.
all:alib/alib.so echo_service echo_client echo_calling_service echo_calling_service_threaded

alib/alib.so:
	$(MAKE) -C alib

echo_service:echo_service_binding_server.h 
echo_client:echo_service_binding_client.h  echo_service_client.o echo_client.c
echo_client_obj=echo_service_client.o

echo_calling_service:echo_calling_service_binding_server.h echo_service_client.o
echo_calling_service_obj=echo_service_client.o

echo_calling_service_threaded:echo_calling_service_binding_server.h echo_service_client.o
echo_calling_service_threaded_obj=echo_service_client.o

%_binding_server.h:%.xml
	dbus-binding-tool --mode=glib-server --prefix=$(basename $<) $< --output=$@

%_binding_client.h:%.xml
	dbus-binding-tool --mode=glib-client --prefix=$(basename $<) $< --output=$@

sinclude .deps
.deps: *.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -MM *.c > .deps
%.o:%.c
	$(CC) -c $(CFLAGS) $(LDLIBS) -o $@  $<
%:%.c
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ ${${@}_obj} $< 
clean:
	rm -rf *.o echo_service echo_client echo_calling_service echo_calling_service_threaded
	$(MAKE) -C alib clean
