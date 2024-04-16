package Game.Controllers;

import Game.GameObjects.FlyingTrap;
import Game.GameObjects.HomingTrap;
import Game.Screen.Scale;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.Random;

public class HomingTrapController {
    String imageName;
    final double scale = Scale.getInstance().getScale();
    final int STARTX = (int) (-80*scale);
    final int ENDX = (int) (960*scale);
    final int STARTY = (int) (-80*scale);
    final int ENDY = (int) (250*scale);

    public HomingTrapController(String imageName) {
        this.imageName = imageName;
    }

    public void create(ArrayList<HomingTrap> models) {
        Random random = new Random(System.currentTimeMillis());
        if (models.isEmpty()) {
            if (random.nextInt(1000) < 5) {
                if (random.nextInt(2) == 0) {
                    models.add(new HomingTrap(imageName, STARTX, STARTY, 1));
                } else {
                    models.add(new HomingTrap(imageName, ENDX, STARTY, 1));
                }
            }
        }
    }

    public void move(ArrayList<HomingTrap> models, int playerX, int playerY) {
        for (HomingTrap model : models) {
            model.move(playerX, playerY);
        }
    }

    public boolean checkCollisions(ArrayList<HomingTrap> models, int playerX, int playerY) {
        for (HomingTrap model : models) {
            if (model.checkCollision(playerX, playerY)) {
                return true;
            }
        }
        return false;
    }
}
