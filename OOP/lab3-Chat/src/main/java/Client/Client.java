package Client;

import Utilites.*;

import javax.swing.*;
import java.awt.event.*;
import java.io.*;
import java.net.InetSocketAddress;
import java.nio.channels.SocketChannel;

import org.w3c.dom.Document;

public class Client extends JFrame {
    private final SocketChannel servSock;
    private final static String HOST = "127.0.0.1";
    private final static int PORT = 25625;
    private String name;
    private final ChatWindow chatWindow;
    private boolean stopFlag = false;

    public Client() {
        try {
            servSock = SocketChannel.open(new InetSocketAddress(HOST, PORT));
            chatWindow = new ChatWindow();
            chatWindow.registration();
            chatWindow.addWindowListener(regCloseListener);
            chatWindow.getRegistrationButton().addActionListener(regButtonListener);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    ActionListener regButtonListener = new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            if (!chatWindow.getRegistrationField().getText().trim().isEmpty()) {
                name = chatWindow.getRegistrationField().getText();
                chatWindow.setName(name);
                Document msg = MessageParser.makeMessage(MessageMode.JOIN, name, name + " зашел в чат");
                MessageSender.write(msg, servSock);
                chatWindow.chatting();
                chatWindow.removeWindowListener(regCloseListener);
                chatWindow.addWindowListener(chatCloseListener);
                chatWindow.getMessageButton().addActionListener(mesButtonListener);
                chatWindow.getRegistrationButton().removeActionListener(regButtonListener);
                chatWindow.getMessageField().grabFocus();
                MessageCatcher.start();
            }
        }
    };
    ActionListener mesButtonListener = new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            if (!chatWindow.getMessageField().getText().trim().isEmpty()) {
                sendMsg();
                chatWindow.getMessageField().grabFocus();
            }
        }
    };
    WindowAdapter regCloseListener = new WindowAdapter() {
        @Override
        public void windowClosing(WindowEvent e) {
            super.windowClosing(e);
            try {
                Document msg = MessageParser.makeMessage(MessageMode.REGLEAVE, "", "");
                MessageSender.write(msg, servSock);
                servSock.close();
            } catch (IOException ex) {
                throw new RuntimeException(ex);
            }
        }
    };
    WindowAdapter chatCloseListener = new WindowAdapter() {
        @Override
        public void windowClosing(WindowEvent e) {
            super.windowClosing(e);
            Document msg = MessageParser.makeMessage(MessageMode.LEAVE, name, name + " вышел из чата");
            MessageSender.write(msg, servSock);
            stopFlag = true;
        }
    };
    Thread MessageCatcher = new Thread(new Runnable() {
        @Override
        public void run() {
            while (!stopFlag) {
                Document message = MessageSender.read(servSock);
                if (message != null)
                    switch (MessageParser.getMode(message)) {
                        case JOIN:
                            chatWindow.addUserName(MessageParser.getName(message));
                            chatWindow.getMessageArea().append(MessageParser.getMessage(message));
                            chatWindow.getMessageArea().append("\n");
                            break;
                        case LEAVE:
                            chatWindow.removeUserName(MessageParser.getName(message));
                            chatWindow.getMessageArea().append(MessageParser.getMessage(message));
                            chatWindow.getMessageArea().append("\n");
                            break;
                        case MESSAGE:
                            chatWindow.getMessageArea().append(MessageParser.getMessage(message));
                            chatWindow.getMessageArea().append("\n");
                            break;
                        case HISTORY:
                            chatWindow.addUserName(MessageParser.getName(message));
                            break;
                    }

            }
            try {
                servSock.close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    });

    public void sendMsg() {
        String message = name + ": " + chatWindow.getMessageField().getText();
        Document msg = MessageParser.makeMessage(MessageMode.MESSAGE, "", message);
        MessageSender.write(msg, servSock);
        chatWindow.getMessageField().setText("");
    }
}

