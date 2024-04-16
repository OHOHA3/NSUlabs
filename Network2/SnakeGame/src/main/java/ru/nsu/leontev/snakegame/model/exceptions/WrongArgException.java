package ru.nsu.leontev.snakegame.model.exceptions;

public class WrongArgException extends RuntimeException {
    public WrongArgException(String message) {
        super(message);
    }
}