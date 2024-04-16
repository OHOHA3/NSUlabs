package Game.Controllers;

import Game.GameObjects.HorisontalTrap;
import Game.Screen.Scale;

import java.util.ArrayList;
import java.util.Random;

public class HorisontalTrapController {
    String leftSideImage;
    String rightSideImage;
    final double scale = Scale.getInstance().getScale();
    final int STARTX = (int) (-60*scale);
    final int ENDX = (int) (960*scale);
    final int EXTRAY = (int) (250*scale);
    final int VARY = (int) (150*scale);

    public HorisontalTrapController(String leftDir, String rightDir) {
        leftSideImage = leftDir;
        rightSideImage = rightDir;
    }

    public void create(ArrayList<HorisontalTrap> models) {
        Random random = new Random(System.currentTimeMillis());
        if (random.nextInt(1000) < 5) {
            if (random.nextInt(2) == 0) {
                models.add(new HorisontalTrap(leftSideImage, STARTX, random.nextInt(VARY) + EXTRAY, random.nextInt(6) + 4));
            } else {
                models.add(new HorisontalTrap(rightSideImage, ENDX, random.nextInt(VARY) + EXTRAY, (random.nextInt(6) + 4) * -1));
            }
        }
    }


    public void move(ArrayList<HorisontalTrap> models) {
        for (HorisontalTrap model : models) {
            model.move();
        }
    }

    public boolean checkCollisions(ArrayList<HorisontalTrap> models, int playerX, int playerY) {
        for (HorisontalTrap model : models) {
            if (model.checkCollision(playerX, playerY)) {
                return true;
            }
        }
        return false;
    }
}
