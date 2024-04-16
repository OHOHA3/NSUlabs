package ru.nsu.leontev.multicast;

import java.net.DatagramPacket;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.concurrent.ConcurrentLinkedQueue;

public class CloneFinder extends Thread {
    private final HashSet<String> users = new HashSet<>();
    private final Map<String, Boolean> timeOutCheck = new HashMap<>();
    private final int sleepTime = 10000;

    public CloneFinder() {
        start();
    }

    public void run() {
        try {
            while (!isInterrupted()) {
                synchronized (timeOutCheck) {
                    Map<String, Boolean> copyTimeOutCheck = new HashMap<>(timeOutCheck);
                    for (Map.Entry<String, Boolean> pair : copyTimeOutCheck.entrySet()) {
                        if (pair.getValue()) {
                            timeOutCheck.put(pair.getKey(), false);
                        } else {
                            timeOutCheck.remove(pair.getKey());
                            synchronized (users) {
                                users.remove(pair.getKey());
                                System.out.println(pair.getKey() + " leaved from group");
                            }
                        }
                    }
                }
                sleep(sleepTime);
            }
        } catch (InterruptedException e) {
            interrupt();
        }
    }

    public void checkPacket(DatagramPacket packet) {
        byte message = packet.getData()[0];
        String address = packet.getAddress() + ":" + packet.getPort();
        synchronized (users) {
            synchronized (timeOutCheck) {
                switch (message) {
                    case MulticastMessage.REPORT -> {
                        timeOutCheck.put(address, true);
                        if (!users.contains(address)) {
                            System.out.println(address + " in group");
                        }
                        users.add(address);
                    }
                    case MulticastMessage.LEAVE -> {
                        timeOutCheck.remove(address);
                        users.remove(address);
                        System.out.println(address + " leaved from group");
                    }
                }
            }
        }

    }

    public void showUsers() {
        synchronized (users) {
            System.out.println("----------------------");
            for (String user : users) {
                System.out.println(user);
            }
            System.out.println("----------------------");
        }
    }

}
