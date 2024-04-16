package Game.GameObjects;

import javax.swing.*;
import java.awt.*;

public class MovingBack implements DrawAble {
    Image image;
    private final int WIDTH = 80;
    private final int HIGH = 80;
    int x;
    int y;
    int speed;

    public MovingBack(String imageName, int x, int y, int speed) {
        image = new ImageIcon(imageName).getImage();
        this.x = x;
        this.y = y;
        this.speed = speed;
    }

    public boolean draw(Graphics g) {
        if (x >= -60 && x <= 960) {
            g.drawImage(image, x, y, WIDTH, HIGH, null);
            return true;
        }
        return false;
    }

    public void move() {
        x += speed;
    }
}
