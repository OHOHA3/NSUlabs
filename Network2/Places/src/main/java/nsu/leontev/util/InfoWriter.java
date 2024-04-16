package nsu.leontev.util;

import nsu.leontev.entities.location.Location;
import nsu.leontev.entities.location.Locations;
import nsu.leontev.entities.place.Place;
import nsu.leontev.entities.place.Places;
import nsu.leontev.entities.weather.Weather;

public class InfoWriter {
    public static void printLocationInfo(Location location) {
        System.out.println("Location:");
        System.out.println(location.getCity() + ", " + location.getName());
    }

    public static void printLocationsInfo(Locations locations) {
        int count = 1;
        for (Location location : locations.getHits()) {
            System.out.print(count + ". ");
            printLocationInfo(location);
            count++;
        }
    }

    public static void printWeatherInfo(Weather weather) {
        System.out.println("Weather:");
        System.out.println(weather.getWeather().get(0).getDescription());
        System.out.println("Temperature - " + weather.getMain().getTemp());
        System.out.println("Feels like - " + weather.getMain().getFeels_like());
    }

    public static void printPlacesInfo(Places places) {
        if (places.getResults().isEmpty()) {
            System.out.println("No places");
            return;
        }
        System.out.println("Places:");
        for (Place place : places.getResults()) {
            System.out.println(place.getTitle() + ", " + place.getAddress());
        }
    }
}
