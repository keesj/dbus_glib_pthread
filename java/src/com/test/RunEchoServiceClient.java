package com.test;

import org.freedesktop.dbus.DBusConnection;

public class RunEchoServiceClient {
	/**
	 */
	public static void main(String[] args) throws Exception {
		System.err.println("EchoServiceClient starting");
		String payload = "EchoServiceClient";
		
		if (args.length == 1){
			payload = args[0];
		}
	    DBusConnection conn = null;    
        conn = DBusConnection.getConnection(DBusConnection.SYSTEM);
        EchoService s = conn.getPeerRemoteObject("com.test.EchoService","/com/test/Platform", EchoService.class);
        System.err.println("Response = " +s.echo(payload));
        conn.disconnect();
	}

}
