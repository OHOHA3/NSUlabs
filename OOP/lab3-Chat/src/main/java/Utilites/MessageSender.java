package Utilites;

import java.io.*;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

import org.w3c.dom.Document;

public class MessageSender {
    public static void write(Document message, SocketChannel client) {
        try (ByteArrayOutputStream baos = new ByteArrayOutputStream()) {
            for (int i = 0; i < 4; i++) {
                baos.write(0);
            }
            try (ObjectOutputStream out = new ObjectOutputStream(baos)) {
                out.writeObject(message);
                ByteBuffer wrap = ByteBuffer.wrap(baos.toByteArray());
                wrap.putInt(0, baos.size() - 4);
                client.write(wrap);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static Document read(SocketChannel client) {
        ByteBuffer buffer;
        try {
            ByteBuffer lengthBuffer = ByteBuffer.wrap(new byte[4]);
            while (lengthBuffer.hasRemaining()) {
                client.read(lengthBuffer);
            }
            buffer = ByteBuffer.allocate(lengthBuffer.getInt(0));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        try {
            while (buffer.hasRemaining()) {
                client.read(buffer);
            }
            try (ObjectInputStream in = new ObjectInputStream(new ByteArrayInputStream(buffer.array()))) {
                return (Document) in.readObject();
            } catch (ClassNotFoundException | IOException e) {
                throw new RuntimeException(e);
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
