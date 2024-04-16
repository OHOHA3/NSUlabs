package nsu.leontev.entities.location;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import lombok.Getter;

@Getter
@JsonIgnoreProperties(ignoreUnknown = true)
public class Location {
    private String city;
    private String name;
    private Coordinates point;
}
