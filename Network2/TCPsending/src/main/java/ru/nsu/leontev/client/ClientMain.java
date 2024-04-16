package ru.nsu.leontev.client;

public class ClientMain {
    public static void main(String[] args) {
        Client client = new Client();
        client.sendFile(args[0], Integer.parseInt(args[1]), args[2]);
    }
}
