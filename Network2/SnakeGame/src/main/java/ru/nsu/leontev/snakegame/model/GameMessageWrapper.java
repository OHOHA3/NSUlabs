package ru.nsu.leontev.snakegame.model;
import ru.nsu.leontev.snakegame.model.SnakesProto.GameMessage;

import java.net.InetAddress;

public class GameMessageWrapper {
    private GameMessage gameMessage;

    private InetAddress inetAddress;
    private int port;

    long timesSent;

    public GameMessageWrapper(GameMessage gameMessage, InetAddress inetAddress, int port) {
        this.gameMessage = gameMessage;

        this.inetAddress = inetAddress;
        this.port = port;

        timesSent = 0;
    }

    public GameMessage getGameMessage() {
        return gameMessage;
    }

    public InetAddress getInetAddress() {
        return inetAddress;
    }

    public int getPort() {
        return port;
    }

    public long getTimesSent() {
        return timesSent;
    }

    public void increaseTimesSent() {
        timesSent++;
    }
}
