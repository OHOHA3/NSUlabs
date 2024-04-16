package Game.Controllers;

import Game.GameObjects.FlyingTrap;
import Game.Screen.Scale;

import java.util.ArrayList;
import java.util.Random;

public class FlyingTrapController {
    String leftSideImage;
    String rightSideImage;
    String upSideImage;
    final double scale = Scale.getInstance().getScale();
    final int STARTX = (int) (-80*scale);
    final int ENDX = (int) (960*scale);
    final int STARTY = (int) (-80*scale);
    final int ENDY = (int) (250*scale);

    public FlyingTrapController(String leftDir, String rightDir, String upDir) {
        leftSideImage = leftDir;
        rightSideImage = rightDir;
        upSideImage = upDir;
    }

    public void create(ArrayList<FlyingTrap> models, int playerX) {
        Random random = new Random(System.currentTimeMillis());
        if (random.nextInt(1000) < 5) {
            switch (random.nextInt(3)) {
                case 0:
                    models.add(new FlyingTrap(leftSideImage, STARTX, random.nextInt(ENDY),
                            random.nextInt(3) + 1, playerX));
                    break;
                case 1:
                    models.add(new FlyingTrap(rightSideImage, ENDX, random.nextInt(ENDY),
                            random.nextInt(3) + 1, playerX));
                    break;
                case 2:
                    models.add(new FlyingTrap(upSideImage, random.nextInt(ENDX), STARTY,
                            random.nextInt(3) + 1, playerX));
                    break;
            }
        }
    }

    public void move(ArrayList<FlyingTrap> models) {
        for (FlyingTrap model : models) {
            model.move();
        }
    }

    public boolean checkCollisions(ArrayList<FlyingTrap> models, int playerX, int playerY) {
        for (FlyingTrap model : models) {
            if (model.checkCollision(playerX, playerY)) {
                return true;
            }
        }
        return false;
    }
}
