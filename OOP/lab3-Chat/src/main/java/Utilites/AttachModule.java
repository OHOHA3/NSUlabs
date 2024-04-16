package Utilites;

import org.w3c.dom.Document;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class AttachModule {
    private final int INTLEN = 4;
    private boolean isMessage = false;
    private final SocketChannel client;
    private final ByteBuffer intBuffer = ByteBuffer.allocate(INTLEN);
    private ByteBuffer messageBuffer;
    private Document message;

    public AttachModule(SocketChannel client) {
        this.client = client;
    }

    public Document getMessage() {
        return message;
    }

    public boolean read() {
        if (!readInt()) {
            return false;
        }
        return readMessage();
    }

    private boolean readInt() {
        if (!isMessage) {
            try {
                if (intBuffer.hasRemaining()) {
                    client.read(intBuffer);
                    if (intBuffer.hasRemaining()) {
                        return false;
                    }
                }
                messageBuffer = ByteBuffer.allocate(intBuffer.getInt(0));
                isMessage = true;
                intBuffer.clear();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
        return true;
    }

    private boolean readMessage() {
        try {
            if (messageBuffer.hasRemaining()) {
                client.read(messageBuffer);
                if (messageBuffer.hasRemaining()) {
                    return false;
                }
            }
            try (ObjectInputStream in = new ObjectInputStream(new ByteArrayInputStream(messageBuffer.array()))) {
                message = (Document) in.readObject();
                isMessage = false;
                messageBuffer.clear();
            } catch (ClassNotFoundException | IOException e) {
                throw new RuntimeException(e);
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        return true;
    }
}
