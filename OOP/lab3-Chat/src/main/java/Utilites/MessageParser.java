package Utilites;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;


public class MessageParser {

    public static Document makeMessage(MessageMode messageMode, String name, String message) {
        try {
            DocumentBuilder documentBuilder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            Document document = documentBuilder.newDocument();
            String mode;
            switch (messageMode) {
                case MESSAGE:
                    mode = "message";
                    break;
                case JOIN:
                    mode = "join";
                    break;
                case REGLEAVE:
                    mode = "regleave";
                    break;
                case LEAVE:
                    mode = "leave";
                    break;
                case HISTORY:
                    mode = "history";
                    break;
                default:
                    throw new RuntimeException();
            }
            Element messageModeTag = document.createElement("M");
            messageModeTag.setAttribute("mode", mode);
            Element nameTag = document.createElement("name");
            nameTag.setTextContent(name);
            Element MessageTag = document.createElement("message");
            MessageTag.setTextContent(message);
            messageModeTag.appendChild(nameTag);
            messageModeTag.appendChild(MessageTag);
            document.appendChild(messageModeTag);
            return document;
        } catch (ParserConfigurationException e) {
            throw new RuntimeException(e);
        }
    }

    public static MessageMode getMode(Document document) {
        Node modeNode = document.getDocumentElement();
        String mode = modeNode.getAttributes().item(0).getTextContent();
        switch (mode) {
            case "message":
                return MessageMode.MESSAGE;
            case "join":
                return MessageMode.JOIN;
            case "regleave":
                return MessageMode.REGLEAVE;
            case "leave":
                return MessageMode.LEAVE;
            case "history":
                return MessageMode.HISTORY;
            default:
                throw new RuntimeException();
        }
    }

    public static String getName(Document document) {
        Node root = document.getDocumentElement();
        return root.getFirstChild().getFirstChild().getTextContent();
    }

    public static String getMessage(Document document) {
        Node root = document.getDocumentElement();
        return root.getLastChild().getFirstChild().getTextContent();
    }
}
