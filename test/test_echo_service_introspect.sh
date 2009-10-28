#/bin/sh
dbus-send \
	--system \
	--type=method_call \
	--print-reply  \
	--dest=com.test.EchoService \
	/com/test/Platform \
	org.freedesktop.DBus.Introspectable.Introspect
