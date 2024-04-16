package nsu.leontev.controllers;

import nsu.leontev.entities.place.Places;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Component;
import org.springframework.web.client.RestTemplate;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.Future;

@Component
public class PlaceController {
    @Value("${place.url}")
    private String url;
    private final RestTemplate restTemplate;

    @Autowired
    public PlaceController(RestTemplate restTemplate) {
        this.restTemplate = restTemplate;
    }

    @Async
    public Future<Places> getPlaces(String coordinates) {
        return CompletableFuture.supplyAsync(() -> restTemplate
                .getForObject(url + coordinates, Places.class));
    }

}
