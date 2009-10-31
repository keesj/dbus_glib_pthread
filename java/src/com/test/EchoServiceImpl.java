package com.test;

public class EchoServiceImpl implements EchoService {

	@Override
	public String echo(String echo) {
		System.err.println("EchoService called with " + echo );
		return echo;
	}

	@Override
	public boolean isRemote() {
		return true;
	}

}
