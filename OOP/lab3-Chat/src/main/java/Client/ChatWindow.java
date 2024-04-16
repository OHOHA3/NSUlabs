package Client;

import javax.swing.*;
import java.awt.*;
import java.util.HashMap;
import java.util.Map;


public class ChatWindow extends JFrame {
    private JTextField registrationField;
    private JPanel registrationPanel;
    private JLabel registrationLabel;
    private JButton registrationButton;
    private JTextArea messageArea;
    private JTextField messageField;
    private JButton messageButton;
    private JMenu userList;
    private final Map<String, JMenuItem> userNames = new HashMap<>();
    private String name;

    public void registration() {
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        setBounds(810, 390, 300, 300);
        setTitle("Registration");
        registrationPanel = new JPanel(new BorderLayout());
        add(registrationPanel, BorderLayout.NORTH);
        registrationField = new JTextField();
        registrationPanel.add(registrationField, BorderLayout.CENTER);
        registrationLabel = new JLabel("Введите свое имя: ");
        registrationPanel.add(registrationLabel, BorderLayout.WEST);
        registrationButton = new JButton("Войти");
        add(registrationButton, BorderLayout.CENTER);
        setVisible(true);
    }

    public void chatting() {
        remove(registrationPanel);
        remove(registrationButton);
        remove(registrationField);
        remove(registrationLabel);
        setBounds(660, 290, 600, 500);
        setTitle("Chat - " + name);
        messageArea = new JTextArea();
        messageArea.setEditable(false);
        messageArea.setLineWrap(true);
        JScrollPane messageScroll = new JScrollPane(messageArea);
        add(messageScroll, BorderLayout.CENTER);
        JPanel bottomPanel = new JPanel(new BorderLayout());
        add(bottomPanel, BorderLayout.SOUTH);
        JLabel messageLabel = new JLabel("Введите сообщение: ");
        bottomPanel.add(messageLabel, BorderLayout.WEST);
        messageButton = new JButton("Отправить");
        bottomPanel.add(messageButton, BorderLayout.EAST);
        messageField = new JTextField();
        bottomPanel.add(messageField, BorderLayout.CENTER);
        JMenuBar menuBar = new JMenuBar();
        add(menuBar, BorderLayout.NORTH);
        userList = new JMenu("Пользователи");
        menuBar.add(userList);
        setVisible(true);
    }

    public JTextField getRegistrationField() {
        return registrationField;
    }

    public JTextField getMessageField() {
        return messageField;
    }

    public JTextArea getMessageArea() {
        return messageArea;
    }

    public JButton getRegistrationButton() {
        return registrationButton;
    }

    public JButton getMessageButton() {
        return messageButton;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void addUserName(String userName) {
        userNames.put(userName, new JMenuItem(userName));
        userList.add(userNames.get(userName));
    }

    public void removeUserName(String userName) {
        userList.remove(userNames.get(userName));
        userNames.remove(userName);
    }
}
