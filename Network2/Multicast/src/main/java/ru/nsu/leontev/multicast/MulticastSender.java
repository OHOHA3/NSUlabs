package ru.nsu.leontev.multicast;

import java.io.IOException;
import java.net.*;

public class MulticastSender extends Thread {
    private final DatagramSocket socket;
    private final InetAddress group;
    private final int port;
    private final int sleepTime = 5000;

    public MulticastSender(String groupAddress, String port) {
        try {
            socket = new DatagramSocket();
            this.port = Integer.parseInt(port);
            group = InetAddress.getByName(groupAddress);
            start();
        } catch (SocketException | UnknownHostException e) {
            throw new RuntimeException(e);
        }
    }

    public void run() {
        try {
            while (!isInterrupted()) {
                byte[] buffer = {MulticastMessage.REPORT};
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length, group, port);
                socket.send(packet);
                Thread.sleep(sleepTime);
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        } catch (InterruptedException e) {
            try {
                byte[] buffer = {MulticastMessage.LEAVE};
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length, group, port);
                socket.send(packet);
                socket.close();
                interrupt();
            } catch (IOException ex) {
                throw new RuntimeException(ex);
            }
        }
    }
}
