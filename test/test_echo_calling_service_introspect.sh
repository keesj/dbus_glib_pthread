#/bin/sh
dbus-send \
	--system \
	--type=method_call \
	--print-reply  \
	--dest=com.test.EchoCallingService \
	/com/test/Platform \
	org.freedesktop.DBus.Introspectable.Introspect
