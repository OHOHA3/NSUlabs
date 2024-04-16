package ru.nsu.leontev;

import ru.nsu.leontev.multicast.CloneFinder;
import ru.nsu.leontev.multicast.MulticastReceiver;
import ru.nsu.leontev.multicast.MulticastSender;

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        CloneFinder cloneFinder = new CloneFinder();
        MulticastReceiver multicastReceiver = new MulticastReceiver(args[0], args[1], cloneFinder);
        MulticastSender multicastSender = new MulticastSender(args[0], args[1]);
        while (true) {
            String command = scanner.nextLine();
            switch (command) {
                case "show" -> cloneFinder.showUsers();
                case "quit" -> {
                    multicastSender.interrupt();
                    multicastReceiver.interrupt();
                    cloneFinder.interrupt();
                    return;
                }
            }
        }
    }
}
