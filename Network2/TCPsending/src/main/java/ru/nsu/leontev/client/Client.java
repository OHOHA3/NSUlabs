package ru.nsu.leontev.client;

import java.io.*;
import java.net.Socket;
import java.nio.ByteBuffer;

public class Client {
    private final int BUFFER_SIZE = 1024;

    public void sendFile(String address, int port, String classPath) {
        File file = new File(classPath);
        long fileSize = file.length();
        String fileName = file.getName();
        try (Socket socket = new Socket(address, port);
             DataInputStream socketIn = new DataInputStream(socket.getInputStream());
             DataOutputStream socketOut = new DataOutputStream(socket.getOutputStream());
             FileInputStream fileIn = new FileInputStream(file)) {

            byte[] bufferForMetadata = makeMetadata(fileName, fileSize);
            socketOut.write(bufferForMetadata);
            socketOut.flush();
            socketIn.read();

            byte[] dataBuffer = new byte[BUFFER_SIZE];
            int readBytes;
            while ((readBytes = fileIn.read(dataBuffer)) != -1) {
                socketOut.write(dataBuffer, 0, readBytes);
                socketOut.flush();
            }

            System.out.println("File has sent successful");
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    private byte[] makeMetadata(String name, long size) {
        char nameLength = (char) name.length();
        ByteBuffer buffer = ByteBuffer.allocate(Character.SIZE / 8 + nameLength + Long.SIZE / 8);
        buffer.putChar(nameLength);
        buffer.put(name.getBytes());
        buffer.putLong(size);
        return buffer.array();
    }
}
