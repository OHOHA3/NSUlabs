package Game.Controllers;

import Game.GameObjects.MovingBack;
import Game.Screen.Scale;

import java.util.ArrayList;
import java.util.Random;

public class MovingBackController {
    String leftDirectionImage;
    String rightDirectionImage;
    final double scale = Scale.getInstance().getScale();
    final int STARTX = (int) (-60*scale);
    final int ENDX = (int) (960*scale);
    final int ENDY = (int) (200*scale);

    public MovingBackController(String leftDir, String rightDir) {
        leftDirectionImage = leftDir;
        rightDirectionImage = rightDir;
    }

    public void create(ArrayList<MovingBack> models) {
        Random random = new Random(System.currentTimeMillis() + 1);
        if (random.nextInt(1000) < 5) {
            if (random.nextInt(2) == 0) {
                models.add(new MovingBack(leftDirectionImage, STARTX, random.nextInt(ENDY), random.nextInt(10) + 1));
            } else {
                models.add(new MovingBack(rightDirectionImage, ENDX, random.nextInt(ENDY), (random.nextInt(10) + 1) * -1));
            }
        }
    }

    public void move(ArrayList<MovingBack> models) {
        for (MovingBack model : models) {
            model.move();
        }
    }
}
