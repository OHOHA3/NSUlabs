package ru.nsu.leontev.multicast;

import java.io.IOException;
import java.net.*;
import java.util.concurrent.ConcurrentLinkedQueue;

public class MulticastReceiver extends Thread {
    private final MulticastSocket socket;
    private final InetSocketAddress group;
    private final CloneFinder cloneFinder;

    public MulticastReceiver(String groupAddress, String port, CloneFinder cloneFinder) {
        try {
            this.cloneFinder = cloneFinder;
            socket = new MulticastSocket(Integer.parseInt(port));
            group = new InetSocketAddress(groupAddress, Integer.parseInt(port));
            socket.joinGroup(group, NetworkInterface.getByInetAddress(group.getAddress()));
            start();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public void run() {
        try {
            while (!isInterrupted()) {
                byte[] buffer = new byte[1];
                DatagramPacket receivedPacket = new DatagramPacket(buffer, buffer.length);
                socket.receive(receivedPacket);
                cloneFinder.checkPacket(receivedPacket);
            }
            socket.leaveGroup(group, NetworkInterface.getByInetAddress(group.getAddress()));
            socket.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
