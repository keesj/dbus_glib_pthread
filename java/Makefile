CLASSPATH?=/usr/share/java/dbus-bin.jar:/usr/share/java/dbus.jar:/usr/share/java/unix.jar:/usr/share/java/debug-disable.jar:classes
export CLASSPATH

all:classes
	javac -d classes src/com/test/*.java

run_service:
	java -Djava.library.path=/usr/lib/jni  com.test.RunEchoService
	
run_service_client:
	java -Djava.library.path=/usr/lib/jni  com.test.RunEchoServiceClient
classes:
	mkdir -p classes

