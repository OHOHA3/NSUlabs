package nsu.leontev.entities.location;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import lombok.Getter;

@Getter
@JsonIgnoreProperties(ignoreUnknown = true)
public class Coordinates {
    private double lng;
    private double lat;
}
