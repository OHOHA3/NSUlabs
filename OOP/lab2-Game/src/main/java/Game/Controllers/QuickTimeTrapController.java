package Game.Controllers;

import Game.GameObjects.QuickTimeTrap;
import Game.Screen.Scale;

import java.util.ArrayList;
import java.util.Random;

public class QuickTimeTrapController {
    String firstPart;
    String secondPart;
final double scale = Scale.getInstance().getScale();

    public QuickTimeTrapController(String firstPart, String secondPart) {
        this.firstPart = firstPart;
        this.secondPart = secondPart;
    }

    public void create(ArrayList<QuickTimeTrap> models) {
        Random random = new Random(System.currentTimeMillis());
        if (models.isEmpty()) {
            if (random.nextInt(1000) < 5) {
                if (random.nextInt(3) == 0) {
                    int startPoint = (int)(random.nextInt(2) * 480 *scale) ;
                    for (int i = 0; i < 6; i++) {
                        models.add(new QuickTimeTrap(firstPart, secondPart, (int)(startPoint + 80 * i*scale)));
                    }
                } else {
                    for (int i = random.nextInt(2); i < 12; i += 2) {
                        if (random.nextInt(2) == 0) {
                            models.add(new QuickTimeTrap(firstPart, secondPart, (int)(80 * i*scale)));
                        }
                    }
                }
            }
        }
    }

    public void move(ArrayList<QuickTimeTrap> models) {
        for (QuickTimeTrap model : models) {
            model.move();
        }
    }

    public boolean checkCollisions(ArrayList<QuickTimeTrap> models, int playerX) {
        for (QuickTimeTrap model : models) {
            if (model.checkCollision(playerX)) {
                return true;
            }
        }
        return false;
    }
}
