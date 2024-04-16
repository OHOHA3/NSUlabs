package nsu.leontev;

import nsu.leontev.controllers.LocationController;
import nsu.leontev.controllers.PlaceController;
import nsu.leontev.controllers.WeatherController;
import nsu.leontev.entities.location.Location;
import nsu.leontev.entities.location.Locations;
import nsu.leontev.entities.place.Places;
import nsu.leontev.entities.weather.Weather;
import nsu.leontev.util.InfoWriter;
import nsu.leontev.util.UserCommunicatoin;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ConfigurableApplicationContext;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;

@SpringBootApplication
public class PlacesApp {
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        ConfigurableApplicationContext context = SpringApplication.run(PlacesApp.class, args);

        String locationName = UserCommunicatoin.printPlace();

        LocationController locationController = context.getBean("locationController", LocationController.class);
        Future<Locations> locationsFuture = locationController.getLocations(locationName);
        Locations locations = locationsFuture.get();

        InfoWriter.printLocationsInfo(locations);
        Location location = UserCommunicatoin.choosePlace(locations);

        String coordinates = "lat=" + location.getPoint().getLat() + "&"
                + "lon=" + location.getPoint().getLng();

        WeatherController weatherController = context.getBean("weatherController", WeatherController.class);
        Future<Weather> weatherFuture = weatherController.getWeather(coordinates);

        PlaceController placeController = context.getBean("placeController", PlaceController.class);
        Future<Places> placesFuture = placeController.getPlaces(coordinates);

        Weather weather = weatherFuture.get();
        Places places = placesFuture.get();

        InfoWriter.printLocationInfo(location);
        InfoWriter.printWeatherInfo(weather);
        InfoWriter.printPlacesInfo(places);

        SpringApplication.exit(context);
    }
}
