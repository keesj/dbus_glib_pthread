all:liba.so

LN?=ln
LDFLAGS=-lpthread
SOURCES=log.c \
	process.c \
	sb.c \
	thread.c

LIBDIR=.
SOBASE=liba.so
SONAME=$(SOBASE).0
SOFULL=$(SONAME).0.0
LIB_TARGET=$(LIBDIR)/$(SOFULL)

liba.so:$(SOURCES)
	$(CC) -Wall -fPIC  -Wl,-soname,$(SONAME) -shared  *.c  -o $(SOFULL) $(LDFLAGS)
	$(LN) -sf $(SOFULL) $(LIBDIR)/$(SONAME)
	$(LN) -sf $(SONAME) $(LIBDIR)/$(SOBASE)
clean:
	$(RM) *.so*
