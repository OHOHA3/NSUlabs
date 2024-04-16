package nsu.leontev.util;

import nsu.leontev.entities.location.Location;
import nsu.leontev.entities.location.Locations;

import java.util.Scanner;

public class UserCommunicatoin {
    public static String printPlace() {
        System.out.println("Input location name");
        Scanner scanner = new Scanner(System.in);
        return scanner.nextLine();
    }

    public static Location choosePlace(Locations locations) {
        System.out.println("Input location number");
        Scanner scanner = new Scanner(System.in);
        while (true) {
            int numberOfLocation = scanner.nextInt();
            if (numberOfLocation < 1 || numberOfLocation > locations.getHits().size()) {
                System.out.println("Invalid number, try again");
            } else {
                scanner.close();
                return locations.getHits().get(numberOfLocation - 1);
            }
        }
    }
}
