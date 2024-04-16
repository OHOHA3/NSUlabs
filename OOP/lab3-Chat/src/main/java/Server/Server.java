package Server;

import Utilites.*;

import java.net.ServerSocket;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;
import java.util.ArrayList;

import org.w3c.dom.Document;

public class Server {
    private final int PORT = 25625;
    private final String ADDRESS = "127.0.0.1";
    private final int HISTORYSIZE = 50;
    private final ArrayList<String> messageHistory = new ArrayList<>();
    private final ArrayList<String> names = new ArrayList<>();
    private final Selector selector;

    public Server() {
        try {
            selector = Selector.open();
            ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
            ServerSocket serverSocket = serverSocketChannel.socket();
            serverSocket.bind(new InetSocketAddress(ADDRESS, PORT));
            serverSocketChannel.configureBlocking(false);
            int ops = serverSocketChannel.validOps();
            serverSocketChannel.register(selector, ops, null);
            while (true) {
                selectorWork(serverSocketChannel);
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    private void selectorWork(ServerSocketChannel serverSocketChannel) throws IOException {
        selector.select();
        Set<SelectionKey> selectedKeys = selector.selectedKeys();
        Iterator<SelectionKey> i = selectedKeys.iterator();
        while (i.hasNext()) {
            SelectionKey key = i.next();
            if (key.isAcceptable()) {
                handleAccept(serverSocketChannel);
            } else if (key.isReadable()) {
                handleRead(key);
            }
            i.remove();
        }
        selector.selectedKeys().clear();
    }

    private void handleAccept(ServerSocketChannel serverSocket) throws IOException {
        SocketChannel client = serverSocket.accept();
        client.configureBlocking(false);
        SelectionKey key = client.register(selector, SelectionKey.OP_READ);
        key.attach(new AttachModule(client));
    }

    private void handleRead(SelectionKey key) {
        AttachModule reader = (AttachModule) key.attachment();
        if (reader.read()) {
            Document msg = reader.getMessage();
            processMessage(msg, key);
        }
    }

    private void processMessage(Document msg, SelectionKey key) {
        switch (MessageParser.getMode(msg)) {
            case JOIN:
                fillHistory(key);
                names.add(MessageParser.getName(msg));
                break;
            case REGLEAVE:
                key.cancel();
                return;
            case LEAVE:
                key.cancel();
                names.remove(MessageParser.getName(msg));
                break;
        }
        for (SelectionKey selectionKey : selector.keys()) {
            if (!(selectionKey.channel() instanceof ServerSocketChannel))
                MessageSender.write(msg, (SocketChannel) selectionKey.channel());
        }
        messageHistory.add(MessageParser.getMessage(msg));
        if (messageHistory.size() > HISTORYSIZE) {
            messageHistory.remove(0);
        }
    }

    private void fillHistory(SelectionKey key) {
        for (String message : messageHistory) {
            Document msg = MessageParser.makeMessage(MessageMode.MESSAGE, "", message);
            MessageSender.write(msg, (SocketChannel) key.channel());
        }
        for (String name : names) {
            Document msg = MessageParser.makeMessage(MessageMode.HISTORY, name, "");
            MessageSender.write(msg, (SocketChannel) key.channel());
        }
    }
}

