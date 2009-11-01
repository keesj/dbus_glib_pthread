CLASSPATH?=/usr/share/java/dbus-bin.jar:/usr/share/java/dbus.jar:/usr/share/java/unix.jar:/usr/share/java/debug-disable.jar:classes
export CLASSPATH

all:compile


compile:generated/com/test/Notification.java
	javac -d classes src/com/test/*.java generated/com/test/*.java

generated/com/test/Notification.java:../notification.xml
	mkdir -p generated/com/test
	java -Djava.library.path=/usr/lib/jni  org.freedesktop.dbus.bin.CreateInterface  -y $< > $@

run_service:compile
	java -Djava.library.path=/usr/lib/jni  com.test.RunEchoService
	
run_service_client:compile
	java -Djava.library.path=/usr/lib/jni  com.test.RunEchoServiceClient

run_notification:compile
	java -Djava.library.path=/usr/lib/jni  com.test.RunNotification

classes:
	mkdir -p classes
