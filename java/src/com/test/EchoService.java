package com.test;

import org.freedesktop.dbus.DBusInterface;

public interface EchoService extends DBusInterface{
	public String echo(String echo);
}
