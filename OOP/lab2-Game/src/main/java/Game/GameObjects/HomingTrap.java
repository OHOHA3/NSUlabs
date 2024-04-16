package Game.GameObjects;

import Game.Screen.Scale;

import javax.swing.*;
import java.awt.*;

public class HomingTrap implements DrawAble {
    Image image;
    final double scale = Scale.getInstance().getScale();
    private final int WIDTH = (int)(60*scale);
    private final int HIGH = (int)(60*scale);
    int x;
    int y;
    int speed;
    int time = 0;

    public HomingTrap(String imageName, int x, int y, int speed) {
        image = new ImageIcon(imageName).getImage();
        this.x = x;
        this.y = y;
        this.speed = speed;
    }

    public boolean draw(Graphics g) {
        if (time < 700) {
            g.drawImage(image, x, y, WIDTH, HIGH, null);
            return true;
        }
        return false;
    }

    public void move(int playerX, int playerY) {
        time++;
        double thisX = x;
        double thisY = y;
        double dx = playerX - thisX;
        double dy = playerY - thisY;
        double factor = Math.abs(dx / dy);
        if (factor > 2) {
            factor = 2;
        }
        x = x + (int) (speed * factor * Math.signum(dx));
        y = y + (int) (speed * Math.signum(dy));
    }

    public boolean checkCollision(int playerX, int playerY) {
        int distanceX = playerX - x;
        int distanceY = playerY - y;
        if (Math.abs(distanceY) < 60) {
            if (Math.abs(distanceX) < 45) {
                return true;
            }
        }
        return false;
    }
}
