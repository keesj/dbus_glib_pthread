CLASSPATH?=/usr/share/java/dbus-bin.jar:/usr/share/java/dbus.jar:/usr/share/java/unix.jar:/usr/share/java/debug-disable.jar:classes
export CLASSPATH

all:classes generated/com/test generated/com/test/Notification.java
	javac -d classes src/com/test/*.java generated/com/test/*.java

generated/com/test/Notification.java:../notification.xml
	java -Djava.library.path=/usr/lib/jni  org.freedesktop.dbus.bin.CreateInterface  -y $< > $@

run_service:
	java -Djava.library.path=/usr/lib/jni  com.test.RunEchoService
	
run_service_client:
	java -Djava.library.path=/usr/lib/jni  com.test.RunEchoServiceClient

run_notification:generated/com/test/Notification.java
	java -Djava.library.path=/usr/lib/jni  com.test.RunNotification
classes:
	mkdir -p classes
generated/com/test:
	mkdir -p generated/com/test

