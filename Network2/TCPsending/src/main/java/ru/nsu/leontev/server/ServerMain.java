package ru.nsu.leontev.server;

public class ServerMain {
    public static void main(String[] args) {
        Server server = new Server(Integer.parseInt(args[0]));
        server.start();
    }
}
