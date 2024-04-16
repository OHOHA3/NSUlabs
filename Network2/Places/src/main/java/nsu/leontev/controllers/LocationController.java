package nsu.leontev.controllers;

import nsu.leontev.entities.location.Locations;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Component;
import org.springframework.web.client.RestTemplate;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.Future;

@Component
public class LocationController {
    @Value("${location.url}")
    private String url;
    private final RestTemplate restTemplate;

    @Autowired
    public LocationController(RestTemplate restTemplate) {
        this.restTemplate = restTemplate;
    }

    @Async
    public Future<Locations> getLocations(String location) {
        return CompletableFuture.supplyAsync(() -> restTemplate
                .getForObject(url + location, Locations.class));
    }
}
