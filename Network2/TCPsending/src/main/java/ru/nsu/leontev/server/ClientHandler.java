package ru.nsu.leontev.server;

import java.io.*;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.Map;

public class ClientHandler extends Thread {
    private final int BUFFER_SIZE = 1024;
    private final int MAX_FILE_NAME = 4096;
    private final double BYTE_IN_MB = 1024 * 1024;
    private final Socket clientSocket;

    public ClientHandler(Socket clientSocket) {
        this.clientSocket = clientSocket;
    }

    public void run() {
        try (DataInputStream socketIn = new DataInputStream(clientSocket.getInputStream());
             DataOutputStream socketOut = new DataOutputStream(clientSocket.getOutputStream())) {

            byte[] bufferForMetadata = new byte[Character.SIZE / 8 + MAX_FILE_NAME + Long.SIZE / 8];
            socketIn.read(bufferForMetadata);
            Map.Entry<String, Long> metadata = parseMetadata(bufferForMetadata);
            String fileName = metadata.getKey();
            Long fileSize = metadata.getValue();
            File file = new File("uploads/" + fileName);
            file.createNewFile();
            FileOutputStream fileOut = new FileOutputStream(file);
            socketOut.write(1);
            socketOut.flush();

            byte[] bufferData = new byte[BUFFER_SIZE];
            long threeSecondsCounter = 0;
            int readBytesPerCycle;
            long readSize = 0;
            long startTime = System.nanoTime();
            long startWriteTime = startTime;
            while (readSize < fileSize && (readBytesPerCycle = socketIn.read(bufferData)) != -1) {
                long endWriteTime = System.nanoTime();
                readSize += readBytesPerCycle;
                if (endWriteTime - startTime > threeSecondsCounter * 3 * 1_000_000_000) {
                    threeSecondsCounter++;
                    double momentSpeed = readBytesPerCycle / BYTE_IN_MB / (double) (endWriteTime - startWriteTime) * 1_000_000_000;
                    double averageSpeed = readSize / BYTE_IN_MB / (double) (endWriteTime - startTime) * 1_000_000_000;
                    System.out.println(fileName + ":");
                    System.out.println("Moment speed: " + momentSpeed + " Mb/s");
                    System.out.println("Average speed: " + averageSpeed + " Mb/s");
                }
                fileOut.write(bufferData, 0, readBytesPerCycle);
                startWriteTime = System.nanoTime();
            }

            System.out.println(fileName + " has received successfully");
            fileOut.close();
            clientSocket.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    private Map.Entry<String, Long> parseMetadata(byte[] message) {
        ByteBuffer buffer = ByteBuffer.wrap(message);
        int nameLength = buffer.getChar();
        byte[] nameBuffer = new byte[nameLength];
        buffer.get(nameBuffer);
        String name = new String(nameBuffer);
        long size = buffer.getLong();
        return Map.entry(name, size);
    }
}