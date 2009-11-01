package com.test;

import org.freedesktop.dbus.DBusConnection;
import org.freedesktop.dbus.DBusSigHandler;
import org.freedesktop.dbus.exceptions.DBusException;




/**
 * dbus-send \
        --system \
        /com/test/Notification \
        com.test.Notification.Notify

 *
 */
public class RunNotification implements DBusSigHandler<Notification.Notify> {


	public static void main(String[] args) throws DBusException {
		RunNotification instance = new RunNotification();
		System.out.println("Creating Connection");
		DBusConnection conn  = DBusConnection.getConnection(DBusConnection.SYSTEM);
		conn.addSigHandler(Notification.Notify.class, instance);
	}

	public void handle(Notification.Notify arg0) {
		// TODO Auto-generated method stub
		System.err.println("Notified");		
	}

}
