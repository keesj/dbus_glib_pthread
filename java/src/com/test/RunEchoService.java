package com.test;

import org.freedesktop.dbus.DBusConnection;

public class RunEchoService {

	/**
	 * @param args
	 * @throws InterruptedException 
	 */
	public static void main(String[] args) throws Exception {
		System.err.println("EchoService starting");
	    DBusConnection conn = null;    
        conn = DBusConnection.getConnection(DBusConnection.SYSTEM);
        conn.requestBusName("com.test.EchoService");
        conn.exportObject("/com/test/Platform",new EchoServiceImpl());
         
        //EchoService s = conn.getPeerRemoteObject("com.test.EchoService","/com/test/Platform", EchoService.class);
        //System.err.println(s.echo("HAHA"));
	}

}
