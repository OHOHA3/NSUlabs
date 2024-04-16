package Game.GameObjects;

import Game.Screen.Scale;

import javax.swing.*;
import java.awt.*;

public class FlyingTrap implements DrawAble {
    Image image;
    double scale = Scale.getInstance().getScale();
    private final int WIDTH = (int)(80*scale);
    private final int HIGH = (int)(80*scale);
    int x;
    int y;
    int dx;
    int dy;
    int speed;

    public FlyingTrap(String imageName, int x, int y, int speed, int playerX) {
        image = new ImageIcon(imageName).getImage();
        this.x = (int)(x *scale);
        this.y = (int)(y*scale);
        this.speed = speed;
        dx = playerX - (int)(x*scale);
        dy = (int)(400 *scale - y*scale);
    }

    public boolean draw(Graphics g) {
        if (y < 400*scale) {
            g.drawImage(image, x, y, WIDTH, HIGH, null);
            return true;
        }
        return false;
    }

    public void move() {
        int factor = Math.abs(dx / dy);
        if (factor > 2) {
            factor = 2;
        }
        x = x + (int) (speed * factor * Math.signum(dx));
        y = y + (int) (speed * Math.signum(dy));
    }

    public boolean checkCollision(int playerX, int playerY) {
        int distanceX = (int)(playerX*scale) - x;
        int distanceY = (int)(playerY*scale) - y;
        if (Math.abs(distanceY) < 60 *scale) {
            if (Math.abs(distanceX) < 45 *scale) {
                return true;
            }
        }
        return false;
    }
}
