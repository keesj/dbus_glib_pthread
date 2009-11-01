These files form an experiment to understand how to use dbus in combination with threads.

We have the following setup

We have a EchoService that can echo strings. We also have a EchoCallingService that can be called to call the
EchoService a certain number of time. The EchoCallingService on one side accepts incomming requests to call the EchoService a given number of times.
At the same time it will be calling the EchoService itself. We want to use thread to model this and want one thread that listens for incoming dbus messages(this is the main thread)
and one thread that performs the calls to the EchoService.



The flow of the calls should look something like this.
test/test_echo_calling_service test 1200
  |
  |
  |--callEcho(test,1200)->echo_calling_service_threaded(incomming end)
                           |
                           |
                           |- start or notify--->echo_calling_service_threaded(sending end)
  |<-----------------------|                     |
                                                 |
                                                 |--1200 times echo(test)-->echo_service
                                                                             |
                                                 |<--------------------------|


How to test the current code:
0) compile the code run make
1) modify and copy the configuration file in data/ (echo_calling_service.conf and echo_service.conf) to /etc/dbus/system.d the user name needs changing.
2) run export LD_LIBRARY_PATH=alib
3) start the echo_service ./echo_service -v &
4) run ./echo_client and see it is possible to call the echo service 10000 times witout problem
5) start the echo calling service ./echo_calling_service_threaded -v & (possibly running this under gdb)
6) start the test -> execute ./test/test_echo_calling_service test 1200 a number of times

Expected outcome the service works like charm and doesn't eat memory or crash(but results may vary)

I also implemented the  EchoServices and a EchoServiceClient java to test go to the java directory and either run make echo_service or echo_service_client.



Notification test:
1) start ./notification_test -v
2) run ./test/test_send_notification
3) see nothing happens
4) goto java and execute make run_notification
5) run ./test/test_send_notification and see it works

Notification test2:
1) start ./notification_test2
2) run ./test/test_send_notification
3) see Notify works!
